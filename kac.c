#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#if defined(__APPLE__) || defined(__MACH__)
#define KAC_MAC
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define KAC_WINDOWS
#include<windows.h>
#define SLEEP(x) Sleep(x)
#else
#pragma message("[kac] operating system not recognized")
#endif

#if defined(KAC_DEBUG)
#if defined(KAC_MAC)
#include <signal.h>
#define DBREAK() raise(SIGTRAP)
#elif defined(KAC_WINDOWS)
#define DBREAK() __debugbreak()
#endif
#else
#define DBREAK()
#endif

#define _KAC_CSTR(foo) #foo
#define _KAC_XCSTR(foo) _KAC_CSTR(foo)


#define ASSERT(CONDITION) do { \
 if (!(CONDITION)) { \
  printf( \
   "\nASSERT(%s); // <- Line %d in %s\n\n", \
   _KAC_CSTR(CONDITION), \
   __LINE__, \
   __FILE__ \
  ); \
 *(volatile int *) NULL = 0; } \
} while (0)

typedef int bool;
#define true 1
#define false 0

void kac_init() {
 setvbuf(stdout, NULL, _IONBF, 0); // don't buffer printf
 DBREAK(); // NOTE pauses RAD-debugger at start
}
