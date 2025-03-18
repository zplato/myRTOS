#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "myRTOS.h"

//Task Stacks
//Arrays of 32 bit unsigned integers
uint32_t stack1[100]; //task A
uint32_t stack2[100]; //task B
uint32_t stack3[100]; //task C
uint32_t stack4[100]; //task D

//Task Periods (in milliseconds)
static unsigned char A_Delay = 50;
static unsigned char B_Delay = 125;
static unsigned char C_Delay = 29;  //unused task period
static unsigned char D_Delay = 49;  //unused task period

//Semaphore Declarations
xSemaphore  SemaphoreList[6]; //Declare a List of Semaphores
xSemaphore* ASemaphore; //Semaphore for task A and so on.....
xSemaphore* BSemaphore;
xSemaphore* CSemaphore;
xSemaphore* DSemaphore;
xSemaphore* LCDSemaphore; //LCD Critical Section Semaphore

//Mailbox Declarations
xMailbox boxC, boxD; //Mailbox for task C and D respectively

//LCD Global Variables
int prog = 0; //placeholder to update the progress bar
int pos = 0;  //placeholder to update touchpad position

//Function Prototypes
extern void Yield(); //function prototype


//My task function declarations
void Task_A_Loop(void)
{
	//release tasks synchronously after 10 systicks
	int prev_release_time = 10;
	while(1)
	{
		//Set task period and release it once its ready
		vTaskDelayUntil(&prev_release_time,A_Delay);
		xSemaphoreGive(ASemaphore);
		xSemaphoreTake(ASemaphore);

		int position = CAPLESENSE_getSliderPosition(); //returns 0-48 based on slider position
													   //-1 if unused

	    //Count every time Job A executes, release task C every 10th execution
		//Every 10th execution gives the LCD a 2Hz refresh rate
		static int job_count = 0;
		job_count++;

		//every job send the data to task D via a mailbox
		writeToBox(&boxD, &position);

		//Every 10th job send data to task C via a mailbox
		if (job_count == 10)
		{
			writeToBox(&boxC, &position); //Release task C via mailbox
			job_count = 0; //reset
		}
	}
}


void Task_B_Loop(void)
{
	//release tasks synchronously after 10 systicks
	int prev_release_time = 10;
	while(1)
	{
		//Set task period and release it once its ready
		vTaskDelayUntil(&prev_release_time,B_Delay);
		xSemaphoreGive(BSemaphore);
     	xSemaphoreTake(BSemaphore);

		//CRITICAL SECTION (LCD RESOURCE) - USE SEMAPHORE TO BLOCK PREEMPTION
		xSemaphoreTake(LCDSemaphore); //if the LCD is available then
		SegmentLCD_ARing(prog,0); //turn off the previous segment
		prog = (prog+1)&7; //bitwise and with 7
		SegmentLCD_ARing(prog,1); //turn on the next segment
		xSemaphoreGive(LCDSemaphore);
	}
}

void Task_C_Loop(void)
{
	while(1)
	{
		readFromBox(&boxC, &pos);// get the position variable if available

		//CRITICAL SECTION (LCD RESOURCE) - USE SEMAPHORE TO BLOCK PREEMPTION
		xSemaphoreTake(LCDSemaphore);

		//Update LCD According to Slider Position
		if (pos == -1)
		{
			SegmentLCD_Write("NOTOUCH");
		}
		else if (pos >= 0 && pos <= 16) //Left Oriented
		{
			SegmentLCD_Write("LEFT");
		}
		else if (pos > 16 && pos <33) //Center Oriented
		{
			SegmentLCD_Write("CENTER");
		}
		else if (pos >= 33)
		{
			SegmentLCD_Write("RIGHT"); //Right Oriented
		}
		xSemaphoreGive(LCDSemaphore); //Finished with LCD Critical Section
	}
}

void Task_D_Loop(void)
{
	while(1)
	{
		//Task Released Via Mailbox
		readFromBox(&boxD, &pos);// get the position variable if available

		//CRITICAL SECTION (LCD RESOURCE) - USE SEMAPHORE TO BLOCK PREEMPTION
		xSemaphoreTake(LCDSemaphore);
		SegmentLCD_Number(pos);			//Update the position on the slider
										//-1 if not touching
		xSemaphoreGive(LCDSemaphore);	//Release Semaphore
	}
}

int main(void)
{
  // Vendor function to work around bugs in some versions of the hardware
  CHIP_Init();

  //CAPACITIVE TOUCH SENSOR INIT
  CAPLESENSE_Init(false);

  //LCD INIT
  //LCD initializes slowly, values written are to initialize it beyond startup state
  //for faster response time
  SegmentLCD_Init(false);
  SegmentLCD_Number(0);
  SegmentLCD_Write("Hello");

  //RTOS VARS INIT
  CurrentTask = TCB; //Task Control Block Pointer, pointing to the current task
  SystemTick = 0;    //System Tick Value initialized to Zero
  idle_count = 0;    //count for aperiodic/sporadic tasks

  //SYSTEM CLOCK CONFIGURATION
  SystemCoreClock = 14000000; 				  // 14 MHz for this device
  if (SysTick_Config(SystemCoreClock/1000))  //configured to interrupt every 1ms
	  while (1);

  //SEMAPHORES INIT
  /* numbers are semaphore indices */
  initSemaphoreBinary(SemaphoreList,6); //initialize the Semaphore list declared above
  ASemaphore = &SemaphoreList[0];		//define Semaphore A and so on...
  BSemaphore = &SemaphoreList[1];
  CSemaphore = &SemaphoreList[2];
  DSemaphore = &SemaphoreList[3];
  LCDSemaphore = &SemaphoreList[4];
  LCDSemaphore = &SemaphoreList[5];

  //MAILBOX INIT
  /* numbers are semaphore indices */
  initMailbox(&boxC,6); //Initialize Mailboxes Declared above
  initMailbox(&boxD,8);

  //CREATE REAL TIME TASKS
  //CreateTask(task identifier, task_handler, task_stack, task_stack_size, priority);
  CreateTask(1,Task_A_Loop,stack1,100,1); //Initialize Task A index as 1, callback as Task_A_Loop etc.
  CreateTask(2,Task_B_Loop,stack2,100,2); //So on.....
  CreateTask(3,Task_C_Loop,stack3,100,3);
  CreateTask(4,Task_D_Loop,stack4,100,4);

  /* Infinite loop for aperiodic and sporadic tasks */
  while (1) {idle_count++;}
}
