#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#if defined(__APPLE__) || defined(__MACH__)
#define AL_MAC
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define AL_WINDOWS
#include<windows.h>
#define SLEEP(x) Sleep(x)
#else
#pragma message("[al] operating system not recognized")
#endif

#if defined(AL_DEBUG)
#if defined(AL_MAC)
#include <signal.h>
#define DBREAK() raise(SIGTRAP)
#elif defined(AL_WINDOWS)
#define DBREAK() __debugbreak()
#endif
#else
#define DBREAK()
#endif

#define _AL_CSTR(foo) #foo
#define _AL_XCSTR(foo) _AL_CSTR(foo)


#define ASSERT(CONDITION) do { \
 if (!(CONDITION)) { \
  printf( \
   "\nASSERT(%s); // <- Line %d in %s\n\n", \
   _AL_CSTR(CONDITION), \
   __LINE__, \
   __FILE__ \
  ); \
 *(volatile int *) NULL = 0; } \
} while (0)

typedef int bool;
#define true 1
#define false 0

void al_init() {
 setvbuf(stdout, NULL, _IONBF, 0); // don't buffer printf
 DBREAK(); // NOTE pauses RAD-debugger at start
}
