/*
 *    FILE: RTOS.cpp
 *  AUTHOR: Ghosty Guo
 */
#include "RTOS.h"

#define  MaxTaskNumber 10

PreemptiveOS RTOS;

unsigned long switchCount=0;
//
Task taskQueue[MaxTaskNumber];
int taskRunningIndex=-1;
int numberOfTask=0;

void TaskSwitching()
{  
    for (int i=0; i<numberOfTask; i++) {
        if (taskQueue[i].State==RUNNING) {
            if (taskQueue[i].ElapsedTick >= taskQueue[i].TickInterval) {            
                taskRunningIndex = i;
                taskQueue[taskRunningIndex].startMillis=millis();            
                taskQueue[taskRunningIndex].Entry();  
                taskQueue[taskRunningIndex].ExecutionTick = millis()-taskQueue[taskRunningIndex].startMillis;
                //taskQueue[taskRunningIndex].ElapsedTick -= taskQueue[taskRunningIndex].TickInterval;
                taskQueue[taskRunningIndex].ElapsedTick =0;
            
                // validate the task
                /*
                if (taskQueue[taskRunningIndex].ExecutionTick>taskQueue[taskRunningIndex].TickInterval) {
                    // run time too long
                    Serial.print("\n****** RTOS : Task # ");  
                    Serial.print(taskRunningIndex);
                    Serial.print(" Exhausted Tick = ");
                    Serial.println(taskQueue[taskRunningIndex].ExecutionTick);
                    Serial.println();
                }
                */
            } else {
                taskRunningIndex = -1;
            }
        } else if (taskQueue[i].State==SUSPEND) {
            // Suspend Process
        }
    }
}

void  Task::setState(TaskState state)
{
    State = state;
    ExecutionTick = 0;
    ElapsedTick = 0; 
}

void Task::suspend() 
{
    setState(SUSPEND);      
}

void Task::run()
{
    setState(RUNNING);            
}

void Task::elapsedShift(long delay)
{
    ElapsedTick += delay;        
}

void PreemptiveOS::start()
{    
    lastMillis = millis();
}

PreemptiveOS::PreemptiveOS()
{
}

void PreemptiveOS::run()
{  
    unsigned long diffMillis = millis()-lastMillis;
    lastMillis = millis();
    switchCount++;
    
    for (int i=0; i<numberOfTask; i++) {
        taskQueue[i].ElapsedTick += diffMillis;
    }
    TaskSwitching();
}

void PreemptiveOS::stop()
{  
    //Timer1.detachInterrupt(); // attach the service routine here
}


Task* PreemptiveOS::addTask(void (*taskEntry)(), String taskName, unsigned int tickInterval, TaskState state)
{
    if (numberOfTask<MaxTaskNumber) {
      taskQueue[numberOfTask].Entry = taskEntry;
      taskQueue[numberOfTask].Name = taskName;
      taskQueue[numberOfTask].TickInterval = tickInterval;
      taskQueue[numberOfTask].ElapsedTick = 0;
      taskQueue[numberOfTask].ExecutionTick = 0;
      taskQueue[numberOfTask].setState(state);
      numberOfTask++;
    } else {
        // Too Many Tasks
        Serial.print("Too Many Tasks");
        while(1);
    }
    return (&taskQueue[numberOfTask-1]); //TaskID
}

void PreemptiveOS::debug()
{
    //if (isTaskRunning) return;
    Serial.print("RunCount = ");
    Serial.print(switchCount);
    Serial.print(", NumberOfTask=");
    Serial.print(numberOfTask);
    if (taskRunningIndex>=0) {      
        Serial.print(", Run Task #");
        Serial.print(taskRunningIndex);    
        Serial.print(" Entry=");
        Serial.print((int)taskQueue[taskRunningIndex].Entry);     
        Serial.print(" Interval=");
        Serial.print(taskQueue[taskRunningIndex].TickInterval);   
        Serial.print(" Elapsed=");
        Serial.println(taskQueue[taskRunningIndex].ElapsedTick);   
        Serial.print(" Execution=");
        Serial.println(taskQueue[taskRunningIndex].ExecutionTick); 
    } else {
        Serial.println();
    }
}


void PreemptiveOS::taskReport()
{
    if (taskRunningIndex>=0) {      
        taskQueue[taskRunningIndex].ExecutionTick = millis()-taskQueue[taskRunningIndex].startMillis;
        Serial.print("*** TASK : Run Task #");
        Serial.print(taskRunningIndex);    
        Serial.print(", Name=");
        Serial.print(taskQueue[taskRunningIndex].Name);     
        Serial.print(", Execution=");
        Serial.print(taskQueue[taskRunningIndex].ExecutionTick); 
        if (taskQueue[taskRunningIndex].ExecutionTick>taskQueue[taskRunningIndex].TickInterval) {           
            Serial.print(" *** Exhausted ***"); // run time too long
        }
        Serial.println("\n");
    } else {
        Serial.println("No Task");
    }
}

