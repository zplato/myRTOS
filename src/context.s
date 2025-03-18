/****************************************************************************
 * SysTick_Handler
 * Interrupt Service Routine for system tick counter
 * The name of this function cannot be changed - it establishes the linkage
 *****************************************************************************/
    .syntax unified
    .text
    .thumb
    .thumb_func
    .align      4
    .globl      SysTick_Handler
    .type       SysTick_Handler, %function
    .globl      SVC_Handler
    .type       SVC_Handler, %function
    .globl      Yield
    .type       Yield, %function

SysTick_Handler:
    push   {r4-r11, lr}

    //increment system tick
    ldr    r6,=SystemTick  // r6 is the address of the current task
    ldr	   r7,[r6]  	   // r7 is the current tick
    add	   r7,r7,#1		   // increment tick count
    str    r7,[r6]   	   // store tick count

    //set the new current task
    ldr    r4,=CurrentTask // r4 is address of current task
    ldr    r5,[r4]         // r5 is current task
    str    sp,[r5,#0]      // stack pointer is first thing in TCB
    bl     scheduler
    str    r0,[r4]         // save new CurrentTask
    ldr    sp,[r0,#0]      // get sp from new current task

    pop    {r4-r11, pc}

//This handler mimics the SysTick_Handler however when called it doesn't increment the SystemTick
SVC_Handler:
    push   {r4-r11, lr}    //store all current task info
    ldr    r4,=CurrentTask // r4 is address of current task
    ldr    r5,[r4]         // r5 is current task
    str    sp,[r5,#0]      // stack pointer is first thing in TCB
  	bl     scheduler	   // call the scheduler
    str    r0,[r4]         // save new CurrentTask
    ldr    sp,[r0,#0]      // get sp from new current task
    pop    {r4-r11, pc}

Yield:
	svc #0 					//Raise SVC interrupt
	bx lr					//return from subroutine


	.end
