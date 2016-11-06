#ifndef RFM69_DEBUG_H
#define RFM69_DEBUG_H

#ifndef NDEBUG
 #include <stdio.h>
 #define DBG_PRINTF printf
 #define DBG_PUTS   puts
#else
 #define DBG_PRINTF(...)
 #define DBG_PUTS(str)
#endif

#endif
