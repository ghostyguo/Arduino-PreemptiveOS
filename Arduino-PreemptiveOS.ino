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
    
    RTOS.taskManager.addTask(SlaveTask, "SlaveTask", 20);  //first task has the highest priority
    RTOS.taskManager.addTask(GenericTask0, "Task0", 5000);    
    RTOS.taskManager.addTask(GenericTask1, "Task1", 2000)->elapsedShift(-5000); //start this task at (count from -5000ms, 1st run at 7000 ms)
    
    pMasterTask = RTOS.taskManager.addTask(MasterTask, "MasterTask", 1000); //Main process
    pMasterWaitThread = RTOS.taskManager.addTask(MasterWaitThread, "MasterWaitThread", 20, SUSPEND);  //wait thread of SendTask, 20ms is expected
    pMasterRcvdThread = RTOS.taskManager.addTask(MasterRcvdThread, "MasterRcvdThread", 50, SUSPEND);  //received thread of SendTask, 50ms is expected
    
    RTOS.start();
}

void loop() {
    RTOS.run(); //Always run th OS
    if (keyPressed()=='?') {
        RTOS.taskManager.taskListReport();
        while (keyPressed()<0); //wait key pressed
    }
}

int keyPressed()
{
    return Serial.available() ? Serial.read() : -1;
}

//
// Task Section
//
void GenericTask0()
{     
    Serial.println("--------------------> Task0()");    

    RTOS.taskManager.activeTaskReport();    // task report
}

void GenericTask1()
{
    Serial.println("--------------------> Task1()");    

    RTOS.taskManager.activeTaskReport();    // single task report
}

/*
 *  IO Processing Tasks
 */
int rcvdByteCount;
void MasterTask()
{
    Serial.println("--------------------> MasterTask()");    
    // Perform the send function
    // ......
    delay(10+random(10)); //Simulation of running time

    // Enable next task
    rcvdByteCount = 0;
    pMasterWaitThread->run();

    RTOS.taskManager.activeTaskReport();    // single task report
}

void MasterWaitThread()
{
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
    
    RTOS.taskManager.activeTaskReport();    // single task report
}

void MasterRcvdThread()
{
    Serial.println("--------------------> MasterRcvdThread()");    

    // Post processing of data
    // ...
    delay(30+random(30)); //Simulation of running time, maybe learge then expected time (50ms)
    
    pMasterRcvdThread->suspend(); // suspend myself thread, SendTask is finished
    
    RTOS.taskManager.activeTaskReport();    // single task report
}

void SlaveTask()
{
    //Serial.println("--------------------> SlaveTask()"); // too many printlns here
    if (random(30)>0) { //Simulation of data receiveing
        return; // not received
    }
    //received
    Serial.println("--------------------------------------------------------------------> SlaveTask()");
    //
    // Response of data
    delay(20+random(10)); //Simulation of running time, maybe learge then expected time (50ms)
    
    RTOS.taskManager.activeTaskReport();    // single task report
}



