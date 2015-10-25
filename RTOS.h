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


typedef enum  {RUNNING, WAIT} TaskState;

class Task {
    public:  
        unsigned int TickInterval;
        unsigned int ElapsedTick;
        unsigned int ExecutionTick;
        TaskState State;
        
        void (*Entry)();
        void setState(TaskState state);
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


