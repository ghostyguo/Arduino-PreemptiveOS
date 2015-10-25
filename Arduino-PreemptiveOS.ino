/*
 *   File: RTOS Demo
 * Author: Ghosty Guo
 */
#include "RTOS.h"

void setup() {
     pinMode(13, OUTPUT);      // 把 ledPin 設置成 output pin
     
    Serial.begin(57600);
    Serial.println("Begin");
    
    RTOS.addTask(Task1, 500);
    RTOS.addTask(Task2, 300);
    RTOS.addTask(Task3, 2000);
    RTOS.start();
}

void loop() {
    RTOS.run();
}

void Task1()
{      
    Serial.println("Task1()");    
    RTOS.taskReport();   
}

void Task2()
{
    Serial.println("Task2()");    
    RTOS.taskReport();   
}

void Task3()
{
    Serial.println("Task2()");    
    RTOS.taskReport();   
}


