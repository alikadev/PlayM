#ifndef PM_SYS_COMMAND_H
#define PM_SYS_COMMAND_H

#include <pm/sys/list.h>
#include <pm/ui/cli/func.h>

typedef struct {
    Function fn;
    LinkedList *tokens;
} Command;

void  command_create(Command *command, char *request);
void  command_destroy(Command *command);

#endif
