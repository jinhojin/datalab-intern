#include "my_kvs.h"
#define XXH_INLINE_ALL
#include <stdio.h>
#include <cstring>
#include <stdbool.h>
#include "xxhash.h"

node *root = NULL;
int count = 0 ;

void displayBST(node * p)
{

        if (p) {
                displayBST(p->left_node);
                printf("%lu\n", p->hash);
                displayBST(p->right_node);
        }
}

node *insertion(node * n, char * input_key, char *  input_value, kvs_key_t klen, kvs_value_t vlen, uint64_t hash)
{
        node *New;
        if (n == NULL) {
                New = (node *) malloc(sizeof(node));
                New -> key = (char *) malloc(sizeof(char)*klen );
                New -> value = (char *) malloc(sizeof(char)*vlen);
		strcpy(New -> key ,input_key);	
		strcpy(New -> value , input_value);
                New -> left_node = NULL;
                New -> right_node = NULL;
		New -> hash = hash;

                return New;
        }

        else if (hash < n -> hash) {
                n->left_node = insertion (n->left_node, input_key, input_value, klen ,vlen,hash);
        }

        else if (hash > n-> hash) {
                n->right_node = insertion(n->right_node, input_key, input_value, klen, vlen, hash);
        }

        else {
                strcpy(n -> value , input_value);
        }
	//displayBST(root);
        //printf("\n");
        return n;
}

void free_func(node * n){
	free(n -> key);
	free(n -> value);
	free(n);
}
	

bool deletion(char * input, uint64_t hash)
{
	printf("deletion start\n");
        node *parent, *child, *p, *suc, *suc_p;
        parent = NULL;
        p = root;
	
        while ((p != NULL) && (p-> hash != hash)) {
                parent = p;
                if (hash < p->hash) {
                        p = p->left_node;
                }

                else {
                        p = p->right_node;
                }
        }
        if (p == NULL) {
                return false;
        }

        if ((p->left_node == NULL) && (p->right_node == NULL)) {
                if (parent != NULL) {
                        if (parent->left_node == p) {
                                parent->left_node = NULL;
                        } else
                                parent->right_node = NULL;
                } else {
                        printf("free function will function 4\n");
		       	free_func(root);
                        root = NULL;
			return true;
                }
		printf("free function will function 5\n");
                free_func(p);
		return true;
        }

        else if ((p->left_node == NULL) || (p->right_node == NULL)) {
                if (p->left_node != NULL) {
                        child = p->left_node;
                } else
                        child = p->right_node;

                if (parent == NULL) {
                        p = child;
			printf("free_func will function\n");
                        free_func(root);
			printf("Double free?\n");
                        root = p;
			displayBST(root);
		        printf("\n");
			printf("return true");
			return true;
                } else {
                        if (parent->left_node == p) {
				printf("hiii\n");
                                parent->left_node = child;
                        } else {
				printf("iiiih\n");
                                parent->right_node = child;
			}

                }
		printf("free function will function 3\n");
                free_func(p);
		return true;
        } else {
                suc_p = p;
                suc = p->left_node;

                while (suc->right_node != NULL) {
                        suc_p = suc;
                        suc = suc->right_node;
                }

                if (suc_p->left_node == suc) {
                        suc_p->left_node = suc->left_node;
                } else {
                        suc_p->right_node = suc->left_node;
		}
                p->value = suc->value;
                p = suc;
		printf("free function will function 2\n");
                printf("%lu\n\n", p -> hash);
		free_func(p);
		displayBST(root);
                printf("\n");
		printf("\n%lu\n", p -> 
		return true;
        }
}


node *search_by_key(node * root, char * key, uint64_t hash)
{
        node *n;
        n = root;

        while (n != NULL) {
                if (hash < (n->hash)) {
                        n = n->left_node;
                } else if (hash > (n->hash)) {
                        n = n->right_node;
                } else
                        return n;
        }
        return n;

}

void  my_kvs_set_env (struct my_kvs *my_kvs, KVS_SET my_kvs_set, KVS_GET my_kvs_get, KVS_DEL my_kvs_del) {
	my_kvs->my_kvs = my_kvs;
	my_kvs->set = my_kvs_set;
	my_kvs->get = my_kvs_get;
	my_kvs->del = my_kvs_del;
	return;
}

int my_kvs_init (struct my_kvs **my_kvs) {
	*my_kvs = (struct my_kvs *)calloc(1, sizeof (struct my_kvs));
	return 0;
}

int my_kvs_destroy (struct my_kvs *my_kvs) {
	free(my_kvs);
	return 0;
}

int my_kvs_set (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx) {
	uint64_t hash;
	hash = XXH64(key -> key, key -> klen, 0);	
	if (count == 0){
		root = insertion(root, key -> key, value -> value, key -> klen, value -> vlen, hash);
	} else{
		insertion(root, key -> key, value -> value, key -> klen, value -> vlen, hash);
	}
	count++;
	return 0;
}

int my_kvs_get (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx) {
	uint64_t hash;
        hash = XXH64(key -> key, key -> klen, 0);
	node * searched_value=search_by_key(root, key -> key, hash);
	strcpy(value -> value, searched_value -> value);
	return 0;
}

int my_kvs_del (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_context *ctx) {
	uint64_t hash;
        hash = XXH64(key -> key, key -> klen, 0);

	if (deletion (key -> key, hash)){
		return 0;
	} else {
		return -1;
	}
	
}


