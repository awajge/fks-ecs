#ifndef FKS_H
#define FKS_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t fks_id_t;
typedef uint64_t fks_entity_t;
typedef struct fks_instance_t fks_instance_t;
typedef enum { FKS_SUCCESS, FKS_FAILURE } fks_status_t;

fks_instance_t* fks_init(void);
void fks_fini(fks_instance_t* inst);
fks_entity_t fks_create(fks_instance_t* inst);
fks_status_t fks_destroy(fks_instance_t* inst, fks_entity_t ntt);
fks_id_t fks_entity_count(const fks_instance_t* inst);
bool fks_vital(const fks_instance_t* inst, fks_entity_t ntt);

#endif