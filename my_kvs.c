#include "my_kvs.h"
#include <stdio.h>
#include <cstring>
#include <stdbool.h>

node *root = NULL;
int count = 0 ;

void displayBST(node * p)
{

        if (p) {
                displayBST(p->left_node);
                printf("%s\n", p->value);
                displayBST(p->right_node);
        }
}

node *insertion(node * n, char * input_key, char *  input_value, kvs_key_t klen, kvs_value_t vlen)
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

                return New;
        }

        else if (strcmp(input_key , n -> key)<0) {
                n->left_node = insertion (n->left_node, input_key, input_value, klen ,vlen);
        }

        else if (strcmp(input_key , n-> key)> 0) {
                n->right_node = insertion(n->right_node, input_key, input_value, klen, vlen);
        }

        else {
                strcpy(n -> value , input_value);
        }
        return n;
}

void free_func(node * n){
	free(n -> key);
	free(n -> value);
	free(n);
}
	

bool deletion(char * input)
{

        node *parent, *child, *p, *suc, *suc_p;
        parent = NULL;
        p = root;
	
        while ((p != NULL) && (strcmp(p->key, input) != 0)) {
                parent = p;
                if (strcmp(input , p->key) < 0) {
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
                        free_func(root);
                        root = NULL;
			return true;
                }
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
                        free_func(root);
                        root = p;
			//displayBST(root);
		        //printf("\n");

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
                free_func(p);
		return true;
        }
}


node *search_by_key(node * root, char * key)
{
        node *n;
        n = root;

        while (n != NULL) {
                if (key < (n->key)) {
                        n = n->left_node;
                } else if (key > (n->key)) {
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
	
	if (count == 0){
		root = insertion(root, key -> key, value -> value, key -> klen, value -> vlen);
	} else{
		insertion(root, key -> key, value -> value, key -> klen, value -> vlen);
	}
	count++;
	return 0;
}

int my_kvs_get (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx) {
	node * searched_value=search_by_key(root, key -> key);
	strcpy(value -> value, searched_value -> value);
	return 0;
}

int my_kvs_del (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_context *ctx) {
	
	if (deletion (key -> key)){
		return 0;
	} else {
		return -1;
	}
	
}


