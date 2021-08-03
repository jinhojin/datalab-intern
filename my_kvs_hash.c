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
                printf("%s\n", p->key);
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
		free( n -> value);
		n -> value = (char*) malloc(sizeof(char)*vlen);
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

node * findmin(node *input){
	node *min = input;
	while(min->left_node != NULL){
		min= min -> left_node;
	}
	return min;
}

node * deletion(node *Node, uint64_t hash, bool *success, bool pass = false)
{	
	//displayBST(root);
	//printf("\n");
	
	node* n = NULL;
	if(Node == NULL){
		return NULL;
	}

	if (Node -> hash > hash){
		Node -> left_node = deletion( Node -> left_node, hash, success, pass);
	}
	else if (Node -> hash < hash){
		Node -> right_node = deletion( Node -> right_node, hash, success, pass );
	}
	else {
		if (Node -> right_node != NULL && Node -> left_node != NULL)
		{
			n = findmin(Node -> right_node);
			Node -> hash = n -> hash;
			free(Node->key);
			free(Node -> value);
			Node -> key = n -> key;
			Node -> value = n -> value;
			Node -> right_node = deletion(Node -> right_node, n -> hash , success, true);
		}
		else{
			n = (Node -> left_node == NULL) ? Node -> right_node : Node -> left_node; 
			if (pass){
				free(Node);
			}
			else{	
				free_func(Node);
			}
			*success = true;
			return n;
		}
	}
	return Node;
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
	bool success = false;
	root = deletion(root, hash, &success, false );
	if (success == true){
		return 0;
	} else {
		return -1;
	}
	
}
