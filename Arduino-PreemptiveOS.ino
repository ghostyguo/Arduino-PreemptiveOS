/*
 *   File: RTOS Demo
 * Author: Ghosty Guo
 */
#include "RTOS.h"

Task *pMasterTask, *pMasterWaitThread, *pMasterRcvdThread;

void setup() {
    randomSeed(analogRead(0));  // simulation initialization
     
    Serial.begin(57600);
    Serial.println("Begin");    
    
    RTOS.addTask(SlaveTask, "SlaveTask", 20);  //first task has the highest priority
    RTOS.addTask(Task0, "Task0", 5000);    
    RTOS.addTask(Task1, "Task1", 2000)->elapsedShift(-5000);
    
    pMasterTask = RTOS.addTask(MasterTask, "MasterTask", 1000); //Main process
    pMasterWaitThread = RTOS.addTask(MasterWaitThread, "MasterWaitThread", 20, SUSPEND);  //wait thread of SendTask, 20ms is expected
    pMasterRcvdThread = RTOS.addTask(MasterRcvdThread, "MasterRcvdThread", 50, SUSPEND);  //received thread of SendTask, 50ms is expected
    
    RTOS.start();
}

void loop() {
    RTOS.run(); //Always run
}

//
// Task Section
//
void Task0()
{     
    Serial.print(millis()); 
    Serial.println("--------------------> Task0()");    

    RTOS.taskReport();    // task report
}

void Task1()
{
    Serial.print(millis()); 
    Serial.println("--------------------> Task1()");    

    RTOS.taskReport();    // task report
}

/*
 *  IO Processing Tasks
 */
int rcvdByteCount;
void MasterTask()
{
    Serial.print(millis()); 
    Serial.println("--------------------> MasterTask()");    
    // Perform the send function
    // ......
    delay(10+random(10)); //Simulation of running time

    // Enable next task
    rcvdByteCount = 0;
    pMasterWaitThread->run();

    RTOS.taskReport();   // task report
}

void MasterWaitThread()
{
    Serial.print(millis()); 
    Serial.println("--------------------> MasterWaitThread()");    
    // Check if data is received complete
    // ...
    delay(2+random(3)); //Simulation of running time
    if (rcvdByteCount>2) {
        pMasterWaitThread->suspend(); // suspend myself thread
        pMasterRcvdThread->run(); // enable next thread, or directly call the post data processing function
    } else {
        if (random(10)>3) rcvdByteCount++; //Simulation of blocked IO
    }
    
    RTOS.taskReport();    // task report
}

void MasterRcvdThread()
{
    Serial.print(millis()); 
    Serial.println("--------------------> MasterRcvdThread()");    

    // Post processing of data
    // ...
    delay(30+random(30)); //Simulation of running time, maybe learge then expected time (50ms)
    
    pMasterRcvdThread->suspend(); // suspend myself thread, SendTask is finished
    
    RTOS.taskReport();    // task report
}

void SlaveTask()
{
    //Serial.print(millis()); 
    //Serial.println("--------------------> SlaveTask()");
    if (random(30)>0) { //Simulation of data receiveing
        return; // not received
    }
    //received
    Serial.print(millis()); 
    Serial.println("--------------------------------------------------------------------> SlaveTask()");
    //
    // Response of data
    delay(20+random(10)); //Simulation of running time, maybe learge then expected time (50ms)
    
    RTOS.taskReport();    // task report
}



