#ifndef PM_DEBUG_H
#define PM_DEBUG_H

#if defined DEBUG || defined DEBUG_TRACE 
  #include <stdio.h>
  #define debug(a ...) printf("[DEBUG] " a)
  #define debugn(a ...) printf(a) // Debug Next
#else // NOT DEBUG
  #define debug(...)
  #define debugn(...)
  #define debugfn()
#endif // DEBUG


#ifdef DEBUG_TRACE
  #define trace(a ...) printf("[TRACE] " a)
  #define debugfn() trace("%s %d: (%s)\n", __FILE__, __LINE__, __FUNCTION__) 
#else // NOT DEBUG_TRACE
  #define trace(...)
  #define debugfn()
#endif //DEBUG_TRACE

#define todo() printf("%s %d (%s): Not implemented yet\n", __FILE__, __LINE__, __FUNCTION__)

#endif // PM_DEBUG_H
