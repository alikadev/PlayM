#ifndef PM_DEBUG_H
#define PM_DEBUG_H

#include <stdio.h>

#ifdef DEBUG
  #define debug(a ...) printf("[DEBUG] " a)
#else // NOT DEBUG
  #define debug(...)
#endif // DEBUG

#endif