#include "../include/fks.h"

#include <stdio.h>
#include <time.h>

#define ID_BITS 32
#define ID_LIMIT (1ull << ID_BITS)
#define ID_MASK (ID_LIMIT - 1)

#define MIN_ENTITY_ID 1ull
#define NULL_ENTITY 0ull

#define PAGE_BITS 12
#define PAGE_SIZE (1ull << PAGE_BITS)
#define PAGE_LIMIT (ID_LIMIT / PAGE_SIZE)
#define PAGE_MASK (PAGE_SIZE - 1)

#define ESL_S(inst, id) ((inst)->esl[(id) >> PAGE_BITS][(id) & PAGE_MASK])

typedef struct fks_instance_t {
    fks_entity_t* esl[PAGE_LIMIT];
    fks_id_t nfe, hwm, nttc;
} fks_instance_t;

static inline fks_status_t inst_alloc(fks_instance_t* inst) {
    return !!(inst->esl[inst->hwm >> PAGE_BITS] = calloc(PAGE_SIZE, sizeof **inst->esl));
}

/**
 * returns new fks_instance_t instance on succesfull calloc(), if it fails, returns NULL.
 */
static inline fks_instance_t* fks_init(void) {
    fks_instance_t* inst;
    return (inst = calloc(1, sizeof(fks_instance_t)))
        ? inst_alloc(inst)
            ? (
                inst->hwm = MIN_ENTITY_ID,
                inst
            ) : (free(inst), NULL)
        : NULL;
}

/**
 * frees an fks_instance_t instance. make sure to run this after you're done.
 */
static inline void fks_fini(fks_instance_t* inst) {
    for (fks_entity_t p = 0; p < (inst->hwm + PAGE_MASK) >> PAGE_BITS; p++)
        free(inst->esl[p]), inst->esl[p] = NULL;
    free(inst);
}

/**
 * returns a free entity id. if entities have been removed and not reassigned to a 
 * new entity, this function grabs the next free id from an intrusive free list in the
 * entity state list. otherwise, it employs a much faster high watermark system that 
 * simply gets the next numerical id. a consequence of this is that the lowest free id
 * will always be returned. if entity limit has been reached, this function will return 
 * the null entity.
 */
static inline fks_entity_t fks_create(fks_instance_t* inst) {
    fks_id_t ce = inst->nfe & ID_MASK;
    return inst->nfe
        ? (
            inst->nttc++,
            inst->nfe = (fks_id_t) ESL_S(inst, ce & ID_MASK),
            (ESL_S(inst, ce) & ~ID_MASK) | ce
        ) : (
            inst->hwm && (inst->hwm & PAGE_MASK || inst_alloc(inst))
                ? inst->nttc++, inst->hwm++
                : NULL_ENTITY
        );
}

/**
 * destroys the given entity. this id will be next up for entity creation unless another
 * entity is destroyed. This function returns FKS_FAILURE if the entity is the null entity,
 * not been allocated yet, or a stale id. Otherwise, it returns FKS_SUCCESS.
 */
static inline fks_status_t fks_destroy(fks_instance_t* inst, fks_entity_t ntt) {
    fks_entity_t ntto = ntt;
    return !(ntt &= ID_MASK) || ntt >= inst->hwm || (ntto ^ ESL_S(inst, ntt)) & ~ID_MASK
        ? FKS_FAILURE
        : (
            ESL_S(inst, ntt) = ((ESL_S(inst, ntt) & ~ID_MASK) + ID_LIMIT) | inst->nfe,
            inst->nfe = (fks_id_t) ntt,
            inst->nttc--,
            FKS_SUCCESS
        );
}

/**
 * returns entity count of given fks_instance_t instance.
 */
static inline fks_id_t fks_entity_count(const fks_instance_t* inst) {
    return inst->nttc;
}

/**
 * returns whether an id is "alive" or not - in other words, if it has been destroyed
 * or not. This function returns 1 if the entity is alive and 0 if the entity has been
 * destroyed or is the null entity.
 */
static inline bool fks_vital(const fks_instance_t* inst, fks_entity_t ntt) {
    fks_entity_t ntto = ntt;
    return (ntt &= ID_MASK) && ntt < inst->hwm && !((ntto ^ (ESL_S(inst, ntt))) & ~ID_MASK);
}

int main() {
    return 0;
}