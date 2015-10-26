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
        unsigned long startMillis;
        TaskState State;
        String Name; 
        
        void (*Entry)();
        void elapsedShift(long delay);
        void setState(TaskState state);
        void suspend();
        void run();
};

class PreemptiveOS {
    public:
        PreemptiveOS(); //constructor
        void start();
        void run();
        void stop();
        Task* addTask(void (*taskEntry)(), String taskName, unsigned int tickInterval = 1000, TaskState state = RUNNING);
        void debug();
        void taskReport();
        
    private:
        unsigned long lastMillis;
};

extern PreemptiveOS RTOS;
#endif
//
// END OF FILE
//


