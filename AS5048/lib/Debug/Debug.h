#ifndef DEBUG_H
#define DEBUG_H

// #define DEBUG // If you comment this line, the DPRINT & DPRINTLN lines are
//               // defined as blank.

#ifdef DEBUG // Macros are usually in all capital letters.
#include <HardwareSerial.h>
#define DPRINT(...) Serial.print(__VA_ARGS__) // DPRINT is a macro, debug print
#define DPRINTLN(...)                                                          \
  Serial.println(__VA_ARGS__) // DPRINTLN is a macro, debug print with new line
#else
#define DPRINT(...)   // now defines a blank line
#define DPRINTLN(...) // now defines a blank line
#endif

#endif /* end of include guard: DEBUG_H */
