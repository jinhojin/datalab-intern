#include "my_kvs.h"
#define XXH_INLINE_ALL
#include <stdio.h>
#include <cstring>
#include <stdbool.h>
#include "xxhash.h"


node* head_node =(node*) malloc(sizeof(node));
int count = 0;

void show_nodes(node *list){
        node *current_node;
        current_node = list;
        if (current_node == NULL){
                printf("data is not in here");
        }
        else{

                while( (current_node -> next) != NULL){
		  	printf("[");
                        printf("%s", current_node -> next -> key);
                        printf("]-");
                        current_node = current_node -> next;
                }
		printf("[end]");
		printf("\n");
		
        }
}

node *insertion(node * list, char * input_key, char *  input_value, kvs_key_t klen, kvs_value_t vlen, uint64_t hash)
{

	if(count == 0){
		head_node -> next = NULL;
		head_node -> prev = NULL;
	} count++;


	if(list -> next == NULL){
                node * new_node =(node*) malloc(sizeof(node));
		new_node -> key = (char*)malloc(sizeof(char)*klen);
		new_node -> value = (char *) malloc(sizeof(char)*vlen);
                strcpy(new_node -> key, input_key);
		strcpy(new_node -> value, input_value);
                list -> next = new_node;
                new_node-> prev = list;
                new_node -> next = NULL;
		new_node -> hash = hash;
		//show_nodes(list);
		return head_node;
        }

        else{
                node *current_node;
                current_node = list;
                while( (current_node -> next) -> hash <= hash){
                        current_node = current_node -> next;
                        if(current_node -> next == NULL){
                                break;
                        }

                }
                if(current_node -> next != NULL){

                        node * new_node =(node*) malloc(sizeof(node));
			new_node -> key = (char*)malloc(sizeof(char)*klen);
			new_node -> value = (char*) malloc(sizeof(char)*vlen);
			new_node -> hash = hash;
                        strcpy(new_node -> key, input_key);
			strcpy(new_node -> value, input_value);

                        new_node -> next = current_node -> next;
                        (current_node -> next) -> prev = new_node;

                        current_node -> next = new_node;
                        new_node -> prev = current_node;
			//show_nodes(list);
			return head_node; 
                }
                else{
                        node * new_node =(node*) malloc(sizeof(node));
                        new_node -> key = (char*)malloc(sizeof(char)*klen);
			new_node -> value = (char*)malloc(sizeof(char)*vlen);
			new_node -> hash = hash;
			strcpy(new_node -> key, input_key);
			strcpy(new_node-> value, input_value);

                        new_node -> next = NULL;

                        current_node -> next = new_node;
                        new_node -> prev = current_node;
			//show_nodes(list);
			return head_node;
                }
        }


}
void freefunc(node *n){
	free(n -> key);
	free(n -> value);
	free(n);
}

bool deletion(node *list, uint64_t hash)
{	
	node *current_node;
        current_node = list;
        if ( current_node -> next == NULL){
		free(list);
		return false;
	}
		else{
		while( (current_node -> next) -> hash < hash){
        		current_node = current_node -> next;
			if (current_node -> next == NULL){
				return false;
			}
        	}
		if( (current_node -> next) -> hash == hash){
			node *tmp = current_node -> next;
			if((current_node -> next) -> next != NULL){
				current_node -> next = (current_node -> next) -> next;
				(current_node -> next) -> prev = current_node;
				//show_nodes(list);
			}
			else{
				current_node -> next = NULL;
				//show_nodes(list);
			}
			freefunc(tmp);
			return true;
		}
		else{	
			return false;
		}
	}
	return true;	
}



node *search_by_key(node * root, char * key, uint64_t hash)
{
        node *n;
        n = head_node;

        while (n -> next != NULL) {
                if (n->hash == hash) {
                        return n;
                } else{
                        n = n -> next;
		}
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
	insertion(head_node, key -> key, value -> value, key -> klen, value -> vlen, hash);
	return 0;
}

int my_kvs_get (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx) {
	uint64_t hash;
        hash = XXH64(key -> key, key -> klen, 0);
	node * searched_value=search_by_key(head_node, key -> key, hash);
	strcpy(value -> value, searched_value -> value);
	return 0;
}

int my_kvs_del (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_context *ctx) {
	uint64_t hash;
        hash = XXH64(key -> key, key -> klen, 0);
	if (deletion(head_node, hash)){
		return 0;
	} else {
		return -1;
	}
	
}
