#include "cpdefine.h"
#include "cpvendor.h"
#include "cpstruct.h"

#ifndef CMX_CXFUNCS_H
#define CMX_CXFUNCS_H

byte K_Task_Create(byte);	/* create task. */
byte K_Task_Start(byte);		/* start task */
byte K_Task_Wait(word16);	/*	have task wait, with or without time period */
byte K_Task_Wake(byte);		/* wake a task that was waiting. */
void K_Task_End(void);		/* must be called by all task prior to end brace. */
void K_Task_Lock(void);		/* disable task switching */
void K_Task_Unlock(void);		/* re-enable task switching. */
byte K_I_Task_Common(byte,byte,byte);	/* common kill, kill-create, kill-start function */
#define K_Task_Kill(A) K_I_Task_Common(A,0,0)	/* remove (kill) a task. */
#define K_Task_Kill_Create(A,B) K_I_Task_Common(A,B,1)	/* remove (kill) a task, create new task. */
#define K_Task_Kill_Create_Start(A,B) K_I_Task_Common(A,B,2)	/* remove (kill) a task, create and start new task. */
byte K_Task_Priority(byte,byte);	/* change a task priority. */

byte K_I_Resource_Common(byte,word16,byte);	/* common resource reserve function */
#define K_Resource_Wait(A,B)	K_I_Resource_Common(A,B,1) /* reserve a resource, with or without time period */
#define K_Resource_Get(A) K_I_Resource_Common(A,0,0)	 /* get a resource if free */
byte K_Resource_Release(byte);		/* release a resource */

byte K_Event_Wait(byte,word16,byte);	/* wait on events, with/without time period */
byte K_Event_Signal(byte,byte,byte);		/* signal an event */
byte K_Event_Reset(byte,byte);				/* reset events bits */

void * K_I_Mesg_Wait_Common(byte,word16,byte);		/* common message get function */
#define K_Mesg_Wait(A,B) K_I_Mesg_Wait_Common(A,B,0)	/* wait for message, with or without time period */
#define K_Mesg_Get(A) K_I_Mesg_Wait_Common(A,0,1)		/* get a message if one is available */
byte K_I_Mesg_Send_Common(byte,byte,word16,void *);	/* common message send function */
#define K_Mesg_Send(A,B) K_I_Mesg_Send_Common(A,0,0,B)	/* send message to a mailbox */
#define K_Mesg_Send_Wait(A,B,C) K_I_Mesg_Send_Common(A,1,B,C)	/* send message, wait for reciever to ACK */

byte K_Mbox_Event_Set(byte,byte,byte );		/* mailbox event setting parameters */

byte K_Timer_Create(byte,byte,byte,byte);	/* create cyclic timer */

byte K_I_Cyclic_Common(byte,word16,word16,byte);		/* common cyclic function */
#define K_Timer_Start(A,B,C) K_I_Cyclic_Common(A,B,C,3)	/* start cyclic timer */
#define K_Timer_Restart(A) K_I_Cyclic_Common(A,0,0,0)		/* re-start cyclic timer */
#define K_Timer_Initial(A,B) K_I_Cyclic_Common(A,B,0,1)		/* re-start timer, with new initial time */
#define K_Timer_Cyclic(A,B) K_I_Cyclic_Common(A,0,B,2)		/* re-start timer, with new cyclic time */
byte K_Timer_Stop(byte);								/* stop cyclic timer */

byte K_I_Semaphore_Get_Common(byte,word16,byte);
#define K_Semaphore_Get(A) K_I_Semaphore_Get_Common(A,0,1)
#define K_Semaphore_Wait(A,B) K_I_Semaphore_Get_Common(A,B,0)
byte K_Semaphore_Post(byte);
byte K_Semaphore_Reset(byte);	

void K_Task_Coop_Sched(void);		/* do a cooperative task switch. */

void K_OS_Init(void);		/* initialize CMX */
void K_OS_Low_Power_Func(void);	/* reduced CPU power down function */
void K_OS_Start(void);		/* enter CMX OS */
void K_I_Timer_Task(void);	/* CMX's timer task */
void K_OS_Tick_Update(void);		/* called by interrupt, system tick */

/***************************************************************
	external assembly routines
***************************************************************/
void K_OS_Disable_Interrupts(void);	/* disable interrupts */
void K_OS_Enable_Interrupts(void);		/* enable interrupts */
void K_OS_Save_Interrupts(void);	/* save then disable interrupts */
void K_OS_Restore_Interrupts(void);		/* restore interrupts */
void K_I_Scheduler(void);				/* enter K_I_Scheduler, called by K_OS_Start */
void K_I_Sched(void);						/* enter K_I_Scheduler, by CMX function */
void K_OS_Intrp_Entry(void);					/* called by interrupts, entry */
void K_OS_Intrp_Exit(void);					/* called by interrupts, exit */
/*******************************************************
* the following functions are not to be called by user *
* used ONLY by other cmx functions  						 *
*******************************************************/

byte K_I_Time_Common(word16,byte);	/* common set task timer and state function */
void K_I_Unlink(void);
void K_I_Priority_In(byte);
byte K_I_Get_Ptr(byte);
void K_I_Func_Return(void);

#endif /* CMX_CXFUNCS_H */


