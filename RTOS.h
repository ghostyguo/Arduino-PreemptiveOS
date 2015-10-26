/*
 *    FILE: RTOS.h
 *  AUTHOR: Ghosty Guo
 */

#ifndef RTOS_h
#define RTOS_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define RTOS_VERSION "0.1"


typedef enum  {RUNNING, SUSPEND} TaskState;

class Task {
    public:  
        long TickInterval;
        int ElapsedTick;
        long ExecutionTick;
        TaskState State;
        
        void (*Entry)();
        void elapsedShift(long delay);
        void setState(TaskState state);
        void suspend();
        void run();
};

class PreemptiveOS {
    public:
        void start();
        void run();
        void stop();
        Task* addTask(void (*taskEntry)(), unsigned int tickInterval = 1000, TaskState state = RUNNING);
        void debug();
        void taskReport();
};

extern PreemptiveOS RTOS;
#endif
//
// END OF FILE
//


