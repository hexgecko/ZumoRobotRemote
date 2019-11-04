#pragma once

#include <Arduino.h>

class Protothread {
public:
    Protothread() : mLine(0) { }

    // Restart protothread
    void restart() { mLine = 0; }

    // Step protothread
    void stop() { mLine = 0xFFFF; }

    // Return true if the protothread is still running.
    bool isRunning() { return mLine != 0xFFFF; }

    // Resume the protothread, return true if it still running else false.
    virtual bool resume() = 0;

protected:
    uint16_t mLine;
    unsigned long mSleepExpire;
};

// Declare start of protothread, used in the resume() implementation.
#define PT_BEGIN() \
    bool __yielded = true; (void) __yielded; switch (mLine) { case 0:

// Stop protothread and end, used in the resume() implementation.
#define PT_END() \
    default: ; } stop(); return false;

// Sleep the protothread in milliseconds.
#define PT_SLEEP(milliseconds) \
    do { mSleepExpire = millis() + milliseconds; mLine = __LINE__; case __LINE__: \
    if(millis() <= mSleepExpire) return true; } while(0)

// Cause protothread to wait until given condition is true.
#define PT_WAIT_UNTIL(condition) \
    do { mLine = __LINE__; case __LINE__: \
    if(!(condition)) return true; } while (0)

// Cause protothread to wait while given condition is true.
#define PT_WAIT_WHILE(condition) PT_WAIT_UNTIL(!(condition))

// Cause protothread to wait until given child protothread completes.
#define PT_WAIT_THREAD(child) PT_WAIT_WHILE((child).resume())

// Restart and spawn given child protothread and wait until it completes.
#define PT_SPAWN(child) \
    do { (child).restart(); PT_WAIT_THREAD(child); } while (0)

// Restart protothread's execution at its PT_BEGIN.
#define PT_RESTART() do { restart(); return true; } while (0)

// Stop and exit from protothread.
#define PT_EXIT() do { stop(); return false; } while (0)

// Yield protothread till next call to resume().
#define PT_YIELD() \
    do { __yielded = false; mLine = __LINE__; case __LINE__: \
    if (!__yielded) return true; } while (0)

// Yield protothread until given condition is true.
#define PT_YIELD_UNTIL(condition) \
    do { __yielded = false; mLine = __LINE__; case __LINE__: \
    if (!__yielded || !(condition)) return true; } while (0)
