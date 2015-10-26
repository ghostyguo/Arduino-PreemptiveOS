/*
 *   File: RTOS Demo
 * Author: Ghosty Guo
 */
#include "RTOS.h"

Task *pSendTask, *pWaitThread, *pRcvdThread;

void setup() {
    pinMode(13, OUTPUT);      // 把 ledPin 設置成 output pin
     
    Serial.begin(57600);
    Serial.println("Begin");
    
    RTOS.addTask(Task0, 5000);
    RTOS.addTask(Task1, 2000)->elapsedShift(-5000);
    pSendTask = RTOS.addTask(SendTask, 1000); //Main process
    pWaitThread = RTOS.addTask(WaitThread, 20, SUSPEND);  //wait thread of SendTask, 20ms is expected
    pRcvdThread = RTOS.addTask(RcvdThread, 50, SUSPEND);  //received thread of SendTask, 50ms is expected 
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
 *  IO Processing
 */
int rcvdByteCount;
void SendTask()
{
    Serial.print(millis()); 
    Serial.println("--------------------> SendTask()");    
    // Perform the send function
    // ......
    delay(10+random(10)); //Simulation of running time

    // Enable next task
    rcvdByteCount = 0;
    pWaitThread->run();

    RTOS.taskReport();   // task report
}

void WaitThread()
{
    Serial.print(millis()); 
    Serial.println("--------------------> WaitThread()");    
    // Check if data is received complete
    // ...
    delay(2+random(3)); //Simulation of running time
    if (rcvdByteCount>2) {
        pWaitThread->suspend(); // suspend myself thread
        pRcvdThread->run(); // enable next thread, or directly call the post data processing function
    } else {
        if (random(10)>3) rcvdByteCount++; //Simulation of blocked IO
    }
    
    RTOS.taskReport();    // task report
}

void RcvdThread()
{
    Serial.print(millis()); 
    Serial.println("--------------------> RcvdThread()");    

    // Post processing of data
    // ...
    delay(30+random(30)); //Simulation of running time, maybe learge then expected time (50ms)
    
    pRcvdThread->suspend(); // suspend myself thread, SendTask is finished
    
    RTOS.taskReport();    // task report
}


