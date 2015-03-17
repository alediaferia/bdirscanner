#ifndef FNV_WRAPPER_H
#define FNV_WRAPPER_H
extern "C" {
#include <fnv.h>

#if defined(HAVE_64BIT_LONG_LONG)
#define FNV1A_INIT FNV1A_64_INIT
typedef Fnv64_t Fnv_t;
static inline Fnv_t fnv_buf(void *buf, size_t len, Fnv_t hashval) {
    return fnv_64a_buf(buf, len, hashval);
}
#else
#define FNV1A_INIT FNV1_32A_INIT
typedef Fnv32_t Fnv_t;
static inline Fnv_t fnv_buf(void *buf, size_t len, Fnv_t hashval) {
    return fnv_32a_buf(buf, len, hashval);
}
#endif
} // extern "C"

#endif // FNV_WRAPPER_H

