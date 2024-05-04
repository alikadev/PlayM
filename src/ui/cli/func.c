#include <stdio.h>
#include <string.h>
#include <pm/ui/cli/func.h>
#include <pm/sys/asserts.h>

Function *str_to_function(char *name, Function *fns, size_t size)
{
    ARG_ASSERT(name);
    ARG_ASSERT(fns);
    ARG_ASSERT(size > 0);

    Function *fn;
    for (size_t i = 0; i < size; ++i)
    {
        fn = &fns[i];
        if (strcmp(fn->name, name) == 0)
            return fn;
    }
    return NULL;
}
