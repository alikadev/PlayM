#ifndef PM_UI_CLI_H
#define PM_UI_CLI_H

#include <pm/ui/cli/commands.h>
#include <pm/ui/cli/proc.h>

void cli_start(AppState *state);
void cli_run(AppState *state);
void cli_stop(AppState *state);

#endif // PM_UI_CLI_H
