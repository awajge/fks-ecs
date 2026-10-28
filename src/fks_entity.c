#include "fks_entity.h"

#include <stdlib.h>

#define FKS_ID_BITS 32
#define FKS_ID_LIMIT (1ull << FKS_ID_BITS)
#define FKS_ID_MASK (FKS_ID_LIMIT - 1)

#define FKS_PAGE_BITS 20
#define FKS_PAGE_SIZE (1ull << FKS_PAGE_BITS)
#define FKS_PAGE_LIMIT (FKS_ID_LIMIT / FKS_PAGE_SIZE)
#define FKS_PAGE_MASK (FKS_PAGE_SIZE - 1)

#define FKS_ESL_S(wrld, id) ((wrld)->esl[(id) >> FKS_PAGE_BITS][(id) & FKS_PAGE_MASK])

typedef struct fks_world_t {
    fks_entity_t* esl[FKS_PAGE_LIMIT];
    uint32_t nfe, hwm, nttc;
} fks_world_t;

static inline bool inst_alloc(fks_world_t* wrld) {
    return (wrld->esl[wrld->hwm >> FKS_PAGE_BITS] = calloc(FKS_PAGE_SIZE, sizeof **wrld->esl))
        ? true
        : (--wrld->hwm, false);
}

fks_world_t* fks_init(void) {
    fks_world_t* wrld;
    return (wrld = calloc(1, sizeof(fks_world_t)))
        ? inst_alloc(wrld)
            ? wrld
            : (free(wrld), NULL)
        : NULL;
}

void fks_fini(fks_world_t* wrld) {
    for (uint32_t p = 0; p <= wrld->hwm >> FKS_PAGE_BITS; p++)
        free(wrld->esl[p]), wrld->esl[p] = NULL;
    free(wrld);
}

fks_entity_t fks_create(fks_world_t* wrld) {
    uint32_t ce = wrld->nfe & FKS_ID_MASK;
    return wrld->nfe
        ? (
            wrld->nttc++,
            wrld->nfe = (uint32_t) FKS_ESL_S(wrld, ce),
            (FKS_ESL_S(wrld, ce) & ~FKS_ID_MASK) | ce
        ) : (
            ~wrld->hwm && (++wrld->hwm & FKS_PAGE_MASK || inst_alloc(wrld))
                ? wrld->nttc++, wrld->hwm
                : FKS_NULL_ENTITY
        );
}

bool fks_destroy(fks_world_t* wrld, fks_entity_t ntt) {
    fks_entity_t ntto = ntt;
    return !(ntt &= FKS_ID_MASK) || ntt > wrld->hwm || (ntto ^ FKS_ESL_S(wrld, ntt)) & ~FKS_ID_MASK
        ? false
        : (
            FKS_ESL_S(wrld, ntt) = ((FKS_ESL_S(wrld, ntt) & ~FKS_ID_MASK) + FKS_ID_LIMIT) | wrld->nfe,
            wrld->nfe = (uint32_t) ntt,
            wrld->nttc--,
            true
        );
}

uint32_t fks_count(const fks_world_t* wrld) {
    return wrld->nttc;
}


bool fks_vital(const fks_world_t* wrld, fks_entity_t ntt) {
    fks_entity_t ntto = ntt;
    return (ntt &= FKS_ID_MASK) && ntt <= wrld->hwm && !((ntto ^ (FKS_ESL_S(wrld, ntt))) & ~FKS_ID_MASK);
}