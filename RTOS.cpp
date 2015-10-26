/*
 *    FILE: RTOS.cpp
 *  AUTHOR: Ghosty Guo
 */
#include "RTOS.h"

// extern
PreemptiveOS RTOS;

// local
const char *strRunningState[] = {"RUNNING","SUSPEND"};

/*
 * Task Class
 */
void  Task::setState(TaskState taskState)
{
    runningState = taskState;
    executionTick = 0;
    elapsedTick = 0; 
}

void Task::suspend() 
{
    setState(SUSPEND);      
}

void Task::run()
{
    setState(RUNNING);            
}

void Task::elapsedShift(long tickShift)
{
    elapsedTick += tickShift;        
}

void Task::report()
{  
    Serial.print(" Name='");
    Serial.print(name);     
    Serial.print("', Interval=");
    Serial.print(tickInterval); 
    Serial.print("', Start=");
    Serial.print(startTick); 
    Serial.print(", Execution=");
    Serial.print(executionTick);  
    Serial.print(", Elapsed=");
    Serial.print(elapsedTick); 
    Serial.print(", State=");
    Serial.println(strRunningState[runningState]); 
}

/*
 *  TaskManager Class
 */

void TaskManager::TaskSwitching()
{  
    for (int i=0; i<numberOfTask; i++) {
        if (taskQueue[i].runningState==RUNNING) {
            if (taskQueue[i].elapsedTick >= taskQueue[i].tickInterval) {            
                activeTaskIndex = i;
                taskQueue[activeTaskIndex].startTick=millis();            
                taskQueue[activeTaskIndex].entry();  
                taskQueue[activeTaskIndex].executionTick = millis()-taskQueue[activeTaskIndex].startTick;
                //taskQueue[activeTaskIndex].elapsedTick -= taskQueue[activeTaskIndex].tickInterval;
                taskQueue[activeTaskIndex].elapsedTick =0;
            
                // validate the task
                /*
                if (taskQueue[activeTaskIndex].ExecutionTick>taskQueue[activeTaskIndex].TickInterval) {
                    // run time too long
                    Serial.print("\n****** RTOS : Task # ");  
                    Serial.print(activeTaskIndex);
                    Serial.print(" Exhausted Tick = ");
                    Serial.println(taskQueue[activeTaskIndex].ExecutionTick);
                    Serial.println();
                }
                */
            } else {
                activeTaskIndex = -1;
            }
        } 
        else if (taskQueue[i].runningState==SUSPEND) {
            // Suspended Task
        }
    }
}

void TaskManager::init()
{    
    lastMillis = millis();
}

TaskManager::TaskManager()
{
}

void TaskManager::run()
{  
    unsigned long diffMillis = millis()-lastMillis;
    lastMillis = millis();
    switchCount++;
    
    for (int i=0; i<numberOfTask; i++) {
        if (taskQueue[i].runningState==RUNNING) {
            taskQueue[i].elapsedTick += diffMillis;
        } 
        else if (taskQueue[i].runningState==SUSPEND) {
            // Suspended Task
        }
    }
    TaskSwitching();
}


Task* TaskManager::addTask(void (*taskEntry)(), String taskName, unsigned int tickInterval, TaskState state)
{
    if (numberOfTask<MaxTaskNumber) {
      taskQueue[numberOfTask].entry = taskEntry;
      taskQueue[numberOfTask].name = taskName;
      taskQueue[numberOfTask].tickInterval = tickInterval;
      taskQueue[numberOfTask].elapsedTick = 0;
      taskQueue[numberOfTask].executionTick = 0;
      taskQueue[numberOfTask].setState(state);
      numberOfTask++;
    } else {
        // Too Many Tasks
        Serial.print("Too Many Tasks");
        while(1);
    }
    return (&taskQueue[numberOfTask-1]); //TaskID
}

void TaskManager::taskListReport()
{
    Serial.print("*** TaskListReport : SwitchCount = ");
    Serial.print(switchCount);
    Serial.print(", NumberOfTask=");
    Serial.print(numberOfTask);    
    Serial.print(", ActiveTask#");
    Serial.println(activeTaskIndex);
    for (int i=0; i<numberOfTask; i++) {      
        Serial.print("TASK # ");
        Serial.print(i);    
        taskQueue[i].report();
    }
}

void TaskManager::activeTaskReport()
{
    if (activeTaskIndex>=0) {      
        taskQueue[activeTaskIndex].executionTick = millis()-taskQueue[activeTaskIndex].startTick;
        Serial.print("*** TASK : Active Task #");
        Serial.print(activeTaskIndex);    
        Serial.print(", Name='");
        Serial.print(taskQueue[activeTaskIndex].name);     
        Serial.print("', Start=");
        Serial.print(taskQueue[activeTaskIndex].startTick); 
        Serial.print(", Execution=");
        Serial.print(taskQueue[activeTaskIndex].executionTick); 
        if (taskQueue[activeTaskIndex].executionTick > taskQueue[activeTaskIndex].tickInterval) {           
            Serial.print(" *** Exhausted ***"); // run time too long
        }
        Serial.println("\n");
    } else {
        Serial.println("No Task");
    }
}

/*
 * PreemptiveOS Class
 */
PreemptiveOS::PreemptiveOS()
{
}

void PreemptiveOS::init()
{
    taskManager.init();    
}

void PreemptiveOS::run()
{
    taskManager.run();    
}


