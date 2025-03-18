//******************************************************************************************************
//******************************************************************************************************
//TITLE: My Real Time Operating System
//Version: 1.0
//Author: Zachary Plato
//Description:
/* myRTOS.c contains all of the Real Time Operating System Utility Code in one file. This file
 * contains all Real-time task creation, semaphore functions, mailbox functions, task scheduler,
 * and Task Delay Functions
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

//Library Includes
#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "myRTOS.h"


//------------------------------------------------------------------------------------------------//
//*FUNCTION: CreateTask
//*DESCRIPTION: Creates Real Time Task, initializing stack memory and task specific parameters
//*INPUTS: task (identifier), task handler (called when task is scheduled),
//*stack (pre-allocated memory associated with task), stack_words (predefined size of stack memory),
//*priority of the task
//*OUTPUTS: Defined and Schedulable Task placed within the Task Control Block
//------------------------------------------------------------------------------------------------//
void CreateTask(int task, void (*funct)(), void *stack, uint32_t stack_words, int32_t priority)
{
	uint32_t *ptr = (uint32_t *)stack + (stack_words-1); // a pointer to the last byte of stack
	*ptr-- = 0x01000000; 								 // xPSR, Thumb state only (program status register), decrement the stack pointer
	*ptr-- = (uint32_t)funct; 							 //decrement it again and have it point to our function
	for (int i=0; i<6; ++i)	*ptr-- = 0; 				 // lr, r12, r3, r2, r1, r0 , place 0's in the stack for the next 6 positions
	*ptr = -7; 											 // exception link register
	for (int i=0; i<8; ++i)	*--ptr = 0; 				 // r11, r10, r9, r8, r7, r6, r5, r4 // place some more zeros

	//Define Task Specific Paramters
	TCB[task].stack_pointer = ptr;	//
	TCB[task].suspend = 0;			//Time scheduler is to suspend before scheduling the task, 0 = task is initially available
	TCB[task].priority = priority;  //Defined Task Priority, lower = higher priority
	TCB[task].blocked = 0; 			//Blocking Identifier, 0 = not initially blocked
	TCB[task].blockedby = 0; 		//Index of blocking task, 0 = nobody is blocking
	TCB[task].task = task;			//Task Identifier
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: initSemaphoreBinary
//*DESCRIPTION: initializes a array of binary semaphore to untaken and assigns an index to each
//*INPUTS: Beginning Address of a Semaphore Array (SemaphoreList) and the initial size of the array
//*OUTPUTS: N/A
//------------------------------------------------------------------------------------------------//
void initSemaphoreBinary(xSemaphore* SemaphoreList, int initialSize)
{
	//Initialize all values of given semaphore
	for(int i = 0; i < initialSize; i++)
	{
		SemaphoreList[i].Semaphore = false; //untaken initially
		SemaphoreList[i].index = i;
	}
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: xSemaphoreTake
//*DESCRIPTION: Take semaphore if available, otherwise block and identify the semaphore that
//*blocks the current task by the semaphores index.
//*INPUTS: Address of Semaphore to Take
//*OUTPUTS: Task blocked or Semaphore Taken
//------------------------------------------------------------------------------------------------//
void xSemaphoreTake(xSemaphore* Semaphore)
{
	__disable_irq(); //disable interrupts
	if(Semaphore->Semaphore == false)
	{
		Semaphore->Semaphore = true;//Semaphore is free to take, so take it
		//Set the field to say who took the semaphore (so that we can tell who is blocking)
		__enable_irq();

	}
	else //if we cannot take the semaphore, then block the task trying to take it
	{	 //and identify the current task its blocked by
		CurrentTask->blocked = 1;
		CurrentTask->blockedby = Semaphore->index; //Set the semaphore identifier
		__enable_irq();
		Yield();
	}
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: xSemaphoreGive
//*DESCRIPTION: Releases the semaphore, an algorithm designed within decides if the semaphore is
//*free or taken by another task that was previously blocked by it.
//*INPUTS: Address of Semaphore to Give
//*OUTPUTS: Semaphore Released or Taken by highest priority task blocked by the Semaphore.
//------------------------------------------------------------------------------------------------//
void xSemaphoreGive(xSemaphore* Semaphore)
{
	__disable_irq(); //disable interrupts


//ALGORITHM PSUEDO CODE
//Loop through all tasks and check the following:
	//Is there somebody else waiting for the semaphore(i.e., blocked)? Thats not CurrentTask
		//No - set semaphore to false (releasing the semaphore)
		//Yes - unblock highest priority task waiting for the semaphore, leave semaphore to true
	//Does the task unblockeds priority greater than the current tasks priority?
		//Yes - Yield (i.e., invoke the scheduler to schedule that task)
		//No - Return (your higher priority so continue running)

	int j = NUM_TASKS+1; //index of highest priority task blocked by semaphore (if it exists)
	for (int i = NUM_TASKS-1; i>0; i--)
	{
		//Check if there someone else blocked by this semaphore
		if(TCB[i].blocked == 1 && TCB[i].blockedby == Semaphore->index)
		{
			//check in priority order
			j = TCB[i].priority;
		}
	}

	if(j == NUM_TASKS+1) //there were no blocked tasks
	{
		Semaphore->Semaphore=false;// give the semaphore
	}
	else
	{
		TCB[j].blocked = 0; //unblock the highest priority task waiting for the semaphore

		//Check if the current task running or the newly unblocked task has higher priority
		//If it does, invoke the scheduler and run the newly unblocked task
		if(TCB[j].priority < CurrentTask->priority)
		{
			Semaphore->Semaphore=true; 	//keep the semaphore blocked, since the newly unblocked task will take it
			__enable_irq(); 			//enable interrupts
			Yield(); 					//invoke scheduler
		}
		//Continue running the current task since our priority is higher than the unblocked task priority
	}
	__enable_irq();
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: vTaskDelayUntil
//*DESCRIPTION: delays scheduler from scheduling the current task until a user defined release
//*time relative to the system tick. updates the next release time based off of the period
//*INPUTS: Pointer to the tasks release_time and period as defined by user.
//*OUTPUTS:
//------------------------------------------------------------------------------------------------//
void vTaskDelayUntil(int* release_time, int period)
{
	CurrentTask->suspend = *release_time; //Current Task won't be released until suspend > sysTick
	*release_time += period; //Update the tasks next release time based off of tasks period
	Yield(); //invoke the scheduler
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: initMailbox
//*DESCRIPTION: initializes mailbox semaphores and their associated indicies
//*INPUTS: Address of the Mailbox and starting semaphore index
//*OUTPUTS: N/A
//------------------------------------------------------------------------------------------------//
void initMailbox(xMailbox *box, int index)
{
	//Initialize all values
	box->sem_Data.Semaphore = true; //No data initially in the mailbox
	box->sem_Data.index = index; //used for a list of semaphores, unused here
	box->sem_Space.Semaphore = false; //initialized to false, there is space in the mailbox
	box->sem_Space.index = index+1; //used for a list of semaphores, unused here
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: writeToBox
//*DESCRIPTION: checks to see if there is space available in the mailbox,
//*writes data to the mailbox if there is, else blocks until mailbox is released.
//*INPUTS: Address of Mailbox, Address of data to store within mailbox
//*OUTPUTS: N/A
//------------------------------------------------------------------------------------------------//
void writeToBox(xMailbox *box, int* stored_data)
{
	xSemaphoreTake(&box->sem_Space); //Check Mailbox Status
	box->the_data = *stored_data;	 //Write Data to Mailbox
	xSemaphoreGive(&box->sem_Data);  //State Mailbox is Full
}

//------------------------------------------------------------------------------------------------//
//*FUNCTION: readFromBox
//*DESCRIPTION: Checks to see if there is data to be read from the box,
//*returns data if there data in the mailbox, blocks if otherwise.
//*INPUTS: Address of Mailbox, Address of place to return data through
//*OUTPUTS: returns (through a pointer) the data stored in the mailbox
//------------------------------------------------------------------------------------------------//
void readFromBox(xMailbox *box, int* returned_data)
{
	xSemaphoreTake(&box->sem_Data); //Check if Mailbox has Data
	*returned_data = box->the_data; //Read Data from Mailbox
	xSemaphoreGive(&box->sem_Space);//State Mailbox is now Empty
}


//------------------------------------------------------------------------------------------------//
//*FUNCTION: Scheduler
//*DESCRIPTION: Called from the svc handler or systick handler inside context.asm,
//*returns the address of the next task to be scheduled
//*INPUTS: N/A
//*OUTPUTS: Address of task to be scheduled
//------------------------------------------------------------------------------------------------//
// Called in an interrupt context to select next task to run
TaskControlBlock* scheduler(void) //return type pointer
{
	//Execute Tasks in order of priority (based on position within the task control block (TCB))
	//1. if the task is available
	//2. if the task has not been blocked by a semaphore
	for (int i = 1; i < NUM_TASKS; i++)
	{
		if (SystemTick >= TCB[i].suspend && TCB[i].blocked == 0)
		{
			return TCB+i; //return address of task to be scheduled
		}
	}
	//return TCB (address of first element in task control block)
	return TCB; //If no task is released, conduct Aperiodic jobs
}

