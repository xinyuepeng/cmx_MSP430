/*********************************************************

Copyright (c) CMX Systems, Inc. All rights reserved

*********************************************************/
/* TINY PLUS version 2.00 */

#define CMX_INIT_MODULE
#define CMXMODULE

#include "cpfuncs.h"	/* get cmx include header file */
#include "cpconfig.h"	/* get user configuration file */

#if C_MAX_MESSAGES > 0
#define CMX_MAX_MESSAGES C_MAX_MESSAGES + 2
#else
#define CMX_MAX_MESSAGES 0
#endif

#if CMX_MAX_MESSAGES > 65535
#undef CMX_MAX_MESSAGES
#define CMX_MAX_MESSAGES 65535
#endif

/***********************************************************
 WARNING: these are assumed to be initailized by the "C" compiler
 start up code. If NOT, then the user must initialize them
 in the K_OS_Init function.
***********************************************************/

byte MAX_TASKS = C_MAX_TASKS;	
byte ROM_TASKS = C_ROM_TASKS;	
byte MAX_RESOURCES = C_MAX_RESOURCES;
byte MAX_CYCLIC_TIMERS = C_MAX_CYCLIC_TIMERS;
byte MAX_MAILBOXES = C_MAX_MAILBOXES;
byte MAX_SEMAPHORES = C_MAX_SEMAPHORES;
byte RTC_SCALE = C_RTC_SCALE;

/*******************************************************
 The following sets up the necessary memory needed by CMX
*******************************************************/

/******************************************************
 The cpvendor.h header file will actually add additional
 CMX variables to ONLY this file. See the header file.
********************************************************/

struct _tcb cmx_tcb[C_MAX_TASKS+1];

#if C_MAX_CYCLIC_TIMERS > 0

CYCLIC_TIMERS tcproc[C_MAX_CYCLIC_TIMERS];

#else

byte tcproc[1];

#endif

#if C_MAX_RESOURCES > 0

RESHDR res_que[C_MAX_RESOURCES];

#else

byte res_que[1];

#endif

#if C_MAX_MAILBOXES > 0

MAILBOX mail_box[C_MAX_MAILBOXES];

#else

byte mail_box[1];

#endif

#if C_MAX_MESSAGES > 0

MSG message_box[CMX_MAX_MESSAGES];

#else

byte message_box[1];

#endif

#if C_MAX_SEMAPHORES > 0

SEM sem_array[C_MAX_SEMAPHORES];

#else

byte sem_array[1];

#endif

struct {
	word16 interrupt_bytes[C_INTERRUPT_SIZE / 2];
	word16 dummy;
	} int_storage;

MSG *message_ptr;
word16 message_free;

RAM_TCB *glob_tcbptr;

byte rtc_count;		/* interrupt K_OS_Tick_Update frequency scale counter */

#if CMX_RAM_INIT > 0
static void clear_ram(void *,word16);
static void init_cmx_variables(void);
#endif

void K_OS_Init()
{
	MSG* link;		/* scratch pointer */

	locked_out = 1;	/* set lock so it will never goto K_I_Scheduler */
	int_count = cmx_flag1 = 0;		

#if CMX_RAM_INIT > 0		/* Should we initialize CMX variables */
	init_cmx_variables();
#endif

#if C_MAX_MESSAGES > 0
	link = message_box; /* address of 1st block */
	message_free = CMX_MAX_MESSAGES;	/* use pipe_max to hold count */
	while (message_free--)
		{
		message_ptr = link;	/* point head at 1st block */
		link++;		/* compute addr of next block */
		message_ptr->env_link = link;	/* create link to it */
		}
	message_ptr->env_link = message_box;	/* last link in chain is null */
	message_ptr = message_box;	/* point head at 1st block */
	message_free = CMX_MAX_MESSAGES-2;
#endif

	interrupt_stack = &int_storage.dummy;
	activetcb = cmx_tcb;  /* dummy tcb */
	activetcb->nxttcb = cmx_tcb;		/* set up link. */
	rtc_count = C_RTC_SCALE;			/* number of timer ticks, before running
												CMX task timer if needed. */
}	/* K_OS_Init */

#if CMX_RAM_INIT > 0
static void init_cmx_variables()
{
	MAX_TASKS = C_MAX_TASKS;	
	ROM_TASKS = C_ROM_TASKS;	
	MAX_RESOURCES = C_MAX_RESOURCES;
	MAX_CYCLIC_TIMERS = C_MAX_CYCLIC_TIMERS;
	MAX_MAILBOXES = C_MAX_MAILBOXES;
	MAX_SEMAPHORES = C_MAX_SEMAPHORES;
	RTC_SCALE = C_RTC_SCALE;
	clear_ram(cmx_tcb,sizeof cmx_tcb);

#if C_MAX_CYCLIC_TIMERS > 0
	clear_ram(tcproc,sizeof tcproc);
#endif

#if C_MAX_RESOURCES > 0
	clear_ram(res_que,sizeof res_que);
#endif

#if C_MAX_MAILBOXES > 0
	clear_ram(mail_box,sizeof mail_box);
#endif

#if C_MAX_MESSAGES > 0
	clear_ram(message_box,sizeof message_box);
#endif

#if C_MAX_SEMAPHORES > 0
	clear_ram(sem_array,sizeof sem_array);
#endif

}

static void clear_ram(void *addr_ptr,word16 size)
{
	byte *crap_ptr;
	crap_ptr = addr_ptr;
	while(size--)
		{
		*crap_ptr++ = 0;
		}
}
#endif

/******************************************************************
	K_OS_Tick_Update:  Called from timer interrupt routine to signal delay tic.
				THIS CODE EXECUTES ONLY AT INTERRUPT LEVEL.
**********************************************************************/
void K_OS_Tick_Update()
{
	if (TEST_CMX_ACTIVE)	/* see if CMX RTOS active yet */
		{
		if (!(--rtc_count))	/* converts hardware tics to CMX delay tics */
			{
			rtc_count = RTC_SCALE;	/* reset counter with rtc prescaler */
			if (TEST_TIMER_ACTIVE)	/* see if any task timers need servicing */
				{
				DO_TIMER_TSK;			/* yes, set proper flag */
				}
	  		}
		}
}	/* K_OS_Tick_Update */

/*****************************************************************
 The following is the CPU reduced power function. The user
 MUST write their own and they must ensure that the proceesor
 returns to the caller, without modifying the stack.

 NOTE: the CMX K_I_Scheduler calls this function which is in the assembly
 module supplied. The user may find it necessary to modify the
 assembly file, so every thing is correct when the CPU
 comes out of the reduced power state. ALSO the CPU must let
 the interrupts wake it up, out of the reduced power state
 so as the task's timers and cyclic timers may have their
 time counters decremented if need be.
*****************************************************************/
void SystemLowPowerMode();
#include "MSP430x14x.h"
void K_OS_Low_Power_Func()
{
    P4OUT |= 0x01;
    LPM3;
    P4OUT &= 0xFE;
}


