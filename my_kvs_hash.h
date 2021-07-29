#ifndef __MY_KVS_H__
#define __MY_KVS_H__

#include "datalab_store.h"


struct my_kvs {
	struct my_kvs *my_kvs;
	KVS_SET set;
	KVS_GET get;
	KVS_DEL del;
	void (*set_env) (struct my_kvs *my_kvs, KVS_SET my_kvs_set, KVS_GET my_kvs_get, KVS_DEL my_kvs_del); 
};

typedef struct _node {

	char * key;
        char * value;
	uint64_t hash;
        struct _node *left_node;
        struct _node *right_node;

} node;


int my_kvs_init (struct my_kvs **my_kvs);
int my_kvs_destroy (struct my_kvs *my_kvs);
void  my_kvs_set_env (struct my_kvs *my_kvs, KVS_SET my_kvs_set, KVS_GET my_kvs_get, KVS_DEL my_kvs_del);

int my_kvs_set (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx);
int my_kvs_get (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_value *value, struct kvs_context *ctx);
int my_kvs_del (struct my_kvs *my_kvs, struct kvs_key *key, struct kvs_context *ctx);

#endif
