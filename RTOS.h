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
#define  MaxTaskNumber 10

typedef enum  {RUNNING, SUSPEND} TaskState;

class Task {
    public:  
        long tickInterval;
        int elapsedTick;
        long executionTick;
        unsigned long startTick;
        TaskState runningState;
        String name; 
        
        void (*entry)();
        void elapsedShift(long tickShift);
        void setState(TaskState taskState);
        void suspend();
        void run();
        void report();
};

class TaskManager {
    public:
        TaskManager(); //constructor
        void start();
        void run();
        Task* addTask(void (*taskEntry)(), String taskName, unsigned int tickInterval = 1000, TaskState state = RUNNING);
        void debug();
        void activeTaskReport();
        void taskListReport();
        
    private:
        Task taskQueue[MaxTaskNumber];
        unsigned long lastMillis;        
        unsigned long switchCount=0;
        int activeTaskIndex=-1;
        int numberOfTask=0;

        void TaskSwitching();
};

class PreemptiveOS {
    // Other features, blocked I/O, semaphore, 
    // Not implemented yet   
    public:
        TaskManager taskManager;
        
        PreemptiveOS();
        void start();
        void run();
        
    private:        
};

extern PreemptiveOS RTOS;
#endif
//
// END OF FILE
//


