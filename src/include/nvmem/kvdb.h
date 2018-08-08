#ifndef __KVDB_H__
#define __KVDB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <xboot.h>

struct kvdb_t {
	struct hlist_head * hash;
	struct list_head list;
	int max_size;
	int hash_size;
	int store_size;
	spinlock_t lock;
};

struct kvdb_t * kvdb_alloc(int size);
void kvdb_free(struct kvdb_t * db);
void kvdb_clear(struct kvdb_t * db);
void kvdb_set(struct kvdb_t * db, const char * key, const char * value);
char * kvdb_get(struct kvdb_t * db, const char * key, const char * def);
void kvdb_from_string(struct kvdb_t * db, char * str);
char * kvdb_to_string(struct kvdb_t * db);
int kvdb_summary(struct kvdb_t * db, void * buf);

#ifdef __cplusplus
}
#endif

#endif /* __KVDB_H__ */
