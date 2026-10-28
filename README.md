# frankenstein-ecs
a project meant to develop into an ecs. currently only a few, but optimized, features  
current state: 32 bit unique entity (identifier) factory

### features:
 - 2^32 entity limit (~4 billion)
 - 32 generation bits -> no stale entity issues
 - low memory overhead + memory cost scales with entity count -> only pay for what you need
 - O(1) entity creation & deletion -> no extra overhead for high entity count, etc.
 - smart entity recycling -> memory won't skyrocket with elongated usage
 - optimized preprocessor utilization implementation -> no extra runtime overhead
 - easy to use -> self-explanatory settings & functions
 - portability: c99 + stdint & stdlib, no external libs, no gcc/clang builtins

### startup:
include fks.h then run fks_init() to get your entity allocator. make sure to run fks_fini() once you're done to avoid memory leaks (8MB+).
 - use fks_create() to create an fks_entity_t (can return FKS_NULL_ENTITY)
 - use fks_destroy() to destroy an fks_entity_t
 - use fks_count() to get the current entity count
 - use fks_vital() to see if an fks_entity_t is alive or not
 - it is HIGHLY recommended to never explicitly create an fks_entity_t
 - passing a NULL fks_world_t will result in underinfed behavior

### changes (v1):
 - minor bugfixes, cleaned up code naming (fks_instance_t -> fks_world_t etc.)
 - removed fks_status_t to use stdbool true & false
 - fun

### wip:
 - simple sparse/dense set type component system
---
#### I recommend installing [Makefile](https://www.gnu.org/software/make/) to build & run.