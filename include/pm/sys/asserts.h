#ifndef PM_SYS_ASSERTS_H
#define PM_SYS_ASSERTS_H

#include <assert.h>

#define ALLOC_ASSERT(value) assert((value) && "Unable to allocate memory")
#define ARG_ASSERT(arg) assert((arg) && "Bad argument: `" #arg "`")
#define OUT_OF_BOUNCE_ASSERT(it, size) assert((it <= size) && "Out of bounce: ")

#endif // PM_SYS_ASSERTS_H
