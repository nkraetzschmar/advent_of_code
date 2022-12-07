#ifndef TYPE
#define TYPE uint8_t
#endif
#ifndef INIT_CAP
#define INIT_CAP 0x100
#endif
#define _CAT(A, B) A ## _ ## B
#define CAT(A, B) _CAT(A, B)

#define VEC_STRUCT CAT(vec, TYPE)
#define VEC_FUNCTION(F) CAT(VEC_STRUCT, F)

typedef struct VEC_STRUCT VEC_STRUCT;

VEC_STRUCT * VEC_FUNCTION(alloc)();
void VEC_FUNCTION(free)(VEC_STRUCT *vec);
uint64_t VEC_FUNCTION(len)(const VEC_STRUCT *vec);
TYPE * VEC_FUNCTION(buf)(const VEC_STRUCT *vec);
int32_t VEC_FUNCTION(push)(VEC_STRUCT *vec, TYPE x);
TYPE VEC_FUNCTION(pop)(VEC_STRUCT *vec);
