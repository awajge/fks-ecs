# frankenstein-ecs
a project meant to develop into an ecs. currently only a few, but optimized, features  
current state: 32 bit unique entity (identifier) factory

### features:
 - 2^32 entity limit (~4 billion)
 - 32 generation bits -> stale entities easily identifiable
 - low fixed memory cost -> memory cost scales with entity count -> only pay for what you need
 - O(1) entity creation & deletion -> no extra overhead for high entity count, etc.
 - optimized preprocessor utilization implementation -> no extra runtime overhead
 - easy to use -> no settings & simply a few comprehensive functions
 - extreme portability: c99 + stdint & stdlib, no external libs
---
startup:
include fks.h (in ../include/) then run fks_init() to get your entity allocator. use fks_create() and fks_destroy() for creating/destroying entity ids, fks_entity_count() to get the total entity#, and fks_vital() to check if an id is alive anymore. make sure to run fks_fini() to avoid memory leaks.

#### I recommend installing [Makefile](https://www.gnu.org/software/make/) to build & run.