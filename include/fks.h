#ifndef FKS_H
#define FKS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t fks_id_t;
typedef uint64_t fks_entity_t;
typedef struct fks_instance_t fks_instance_t;
typedef enum { FKS_FAILURE, FKS_SUCCESS } fks_status_t;

static inline fks_instance_t* fks_init(void);
static inline void fks_fini(fks_instance_t* inst);
static inline fks_entity_t fks_create(fks_instance_t* inst);
static inline fks_status_t fks_destroy(fks_instance_t* inst, fks_entity_t ntt);
static inline fks_id_t fks_entity_count(const fks_instance_t* inst);
static inline bool fks_vital(const fks_instance_t* inst, fks_entity_t ntt);

#endif