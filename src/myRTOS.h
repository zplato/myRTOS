//******************************************************************************************************
//******************************************************************************************************
//TITLE: My Real Time Operating System
//Version: 1.0
//Author: Zachary Plato
//Description:
/* myRTOS.h contains all of the Real Time Operating System Interface Code for my Real-Time Operating
*  System (myRTOS). The code provided below is shared between myRTOS.c (utility source code) and
*  the user defined myRTOS project. myRTOS.h includes structs, global variable declarations
*  and function prototypes.
 */
//Revision History:
//||=============================================================================================||
//||---VERSION---++-----------------------DESCRIPTION-----------------------++--DATE (MM/DD/YY)--||
//||Version 1.0  ++ The code provided was developed during ECE 5710 - Real  ++     11/26/17      ||
//||             ++ Time Systems Course in the Fall Semester 2017. Code is  ++                   ||
//||             ++ provided as is, without warranty or liability.          ++                   ||
//||-------------++---------------------------------------------------------++-------------------||
//||=============================================================================================||
//******************************************************************************************************
//******************************************************************************************************

#ifndef MYRTOS_H_
#define MYRTOS_H_

#define NUM_TASKS 5 //Hard Coded Number of Real-Time Tasks, (always NUM_TASKS-1)


//------------------------------------------------------------------------------------------------//
// -- 									STRUCTURES											   -- //
//------------------------------------------------------------------------------------------------//


//STRUCT: TaskControlBlock
//DESCRIPTION: struct containing all task associated parameters, such as priority, blocked, etc.
typedef struct
{
	uint32_t *stack_pointer; 	//points to allocated task stack memory
	int32_t suspend;			//suspend time till tasks next release
	int32_t priority;			//tasks priority level (lower num = higher priority)
	int32_t blocked; 			//task is blocked: 0 == false, 1 == true
	int blockedby;				//index of the semaphore blocking the task
	int task; 					//index of task - for location within a TCB Array
} TaskControlBlock;

//STRUCT: xSemaphore
//DESCRIPTION:
typedef struct {
  bool Semaphore;				//Binary semaphore: false == untaken, true == taken
  int index; 					//index of Semaphore - for location within Semaphore Array
} xSemaphore;

//STRUCT: xMailbox
//DESCRIPTION:
typedef struct {
	xSemaphore sem_Space;		//Indicates Mailbox Status: false == emtpy, true == full
	xSemaphore sem_Data;		//Indicates Mailbox holds Data: false == empty, true == full
	int the_data;				//Data Held by Mailbox
} xMailbox;

//------------------------------------------------------------------------------------------------//
// -- 								GLOBAL VARIABLES										   -- //
//------------------------------------------------------------------------------------------------//
TaskControlBlock TCB[NUM_TASKS]; 		//Array which holds (NUM_TASKS) of Task Control Blocks(TCB)
volatile TaskControlBlock* CurrentTask; //Points to the current task executing
uint32_t SystemTick; 					//System Tick incremented as defined by end user
int idle_count; 						//Count for aperiodic/sporadic tasks

//------------------------------------------------------------------------------------------------//
// -- 								FUNCTION PROTOTYPES 									   -- //
//------------------------------------------------------------------------------------------------//
//*NOTE: See Function Definitions in myRTOS.c for Complete Description

//Initialize Binary Semaphore Array to untaken and assign an index to each semaphore
void initSemaphoreBinary(xSemaphore* SemaphoreList, int initialSize);
void xSemaphoreTake(xSemaphore* Semaphore);			 //Take Semaphore if Available
void xSemaphoreGive(xSemaphore* Semaphore);			 //Check Algorithm and Give Semaphore
void readFromBox(xMailbox *box, int* x);			 //Read Data From Mailbox
void writeToBox(xMailbox *box, int* x);				 //Write data into mailbox
void initMailbox(xMailbox *box, int index);			 //Initalize Mailbox Semaphores and Parameters
TaskControlBlock* scheduler(void); 					 //Real-Time Task Scheduler
void vTaskDelayUntil(int* release_time, int period); //Set release time of task
//Create Real-Time Task: allocate memory, define parameters.
void CreateTask(int task, void (*funct)(), void *stack, uint32_t stack_words, int32_t priority);

#endif /* MYRTOS_H_ */
