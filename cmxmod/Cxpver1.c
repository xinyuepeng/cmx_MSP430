/*********************************************************

Copyright (c) CMX Systems, Inc. 2004. All rights reserved

*********************************************************/
/* TINY PLUS version 2.00 */

#define CMXMODULE 1

#include <cpfuncs.h>	/* get cmx include header file */
#include <cpextern.h>	/* get cmx include header file */

void K_I_Func_Return()
{
	K_I_Enable_Sched();
}

byte K_I_Get_Ptr(byte ram_id)
{
	if (ram_id > MAX_TASKS)	/* see if task exists */
		return(K_ERROR);	/* no, return error */
	glob_tcbptr = &cmx_tcb[ram_id];
	if (glob_tcbptr->tcbstate)	/* see if NOT NULL */
		return(K_OK);	/* return good status */
	else
		return(K_ERROR);	/* it was so return error */
}

/*********************************************************************
 Create a task function
********************************************************************/
byte K_Task_Create(byte rom_id)
{
	byte new_tskid;

	if (rom_id > ROM_TASKS)	/* see if task exists */
		return(K_ERROR);	/* no, return error */
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	new_tskid = task_rom_list[rom_id].ram_tcb_index;
	K_I_Get_Ptr(new_tskid);
	if (new_tskid <= MAX_TASKS)	/* see if task can exist? */
		{
		if (glob_tcbptr->tcbstate == K_KILL)	/* see if slot empty */
			{
			glob_tcbptr->rom_tcb_index = &task_rom_list[rom_id];	/* install ROM task index */
			glob_tcbptr->tcbstate = K_IDLE;	/* place into IDLE state */
			K_I_Priority_In(task_rom_list[rom_id].init_priority);
			K_I_Func_Return();	/* re-enable task switching and test */
			return(K_OK);		/* return successful status to caller */
			}
		}
	K_I_Func_Return();	/* re-enable task switching and test */			
	return(K_ERROR);	/* return error status to caller */
}
byte K_Task_Priority(byte ram_id,byte new_priority)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	if(K_I_Get_Ptr(ram_id))
		{
		K_I_Func_Return();
		return(K_ERROR);
		}
	K_I_Unlink();	/* take task out of priority chain */
	K_I_Priority_In(new_priority);	/* now insert the task back into the
													priority chain link list */
	if (glob_tcbptr != activetcb)
		{
		if (new_priority < activetcb->priority)
			PREEMPTED;	/* yes, set the preempted scheduling flag */
		}
	K_I_Func_Return();	/* release task block */
	return(K_OK);	/* return good status */
}

/******************************************************
 This function will remove a task from the linked list of TCB's.
******************************************************/
void K_I_Unlink()
{
	RAM_TCB *prevtcb;

	/* remove the task from linked list */
	prevtcb = cmx_tcb;

	/* scan TCB's looking for TCB that points to this TCB */
	while (prevtcb->nxttcb != glob_tcbptr)
		{
		prevtcb = prevtcb->nxttcb;
		}
	prevtcb->nxttcb = glob_tcbptr->nxttcb;
	/* the previous tcb nxttcb points to the delink tcb nxttcb */
}

void K_I_Priority_In(byte priority)
{
	RAM_TCB *prevtcb;
	RAM_TCB *tp;

	/* INITIALIZE cmx_tcb[0].nxttcb = cmx_tcb; */
	tp = cmx_tcb;	/* address of TCB link list. */
	do /* insert new tcb at appropriate priority slot in chain...*/
		{
		prevtcb = tp;
		tp = tp->nxttcb;
		} while ((tp != cmx_tcb) && (tp->priority <= priority));
	/* We should insert it just after prevtcb, just before tp. */

	glob_tcbptr->nxttcb = tp;			/* adjust priority slot chain */
	prevtcb->nxttcb = glob_tcbptr;
	glob_tcbptr->priority = priority;	/* load in task's priority */
}

void K_Task_Coop_Sched(void)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	DO_COOP_SCHED;	/* set the cooperative schedule flag */
	K_I_Func_Return();	/* re-enable task switching and test */	
}

/*****************************************************************
 ALL tasks MUST call this function if the task is not compute bound
 and going to execute their end brace. This function MUST be called
 just before task going to hit their end brace.
*******************************************************************/
void K_Task_End(void)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	activetcb->tcbstate = K_IDLE;	/* put task into proper state */
	PREEMPTED;	/* set the preempted flag */
	K_I_Func_Return();	/* re-enable task switching and test */	
}

/* mode
= 0 = K_Task_Kill
= 1 = K_Task_Kill_Create
= 2 = K_Task_Kill_Create_Start
*/
byte K_I_Task_Common(byte ram_id,byte rom_id,byte mode)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	if(K_I_Get_Ptr(ram_id) || rom_id > ROM_TASKS)
		{
		K_I_Func_Return();
		return(K_ERROR);
		}
	K_I_Unlink();	/* take task out of priority chain */
	if (mode)
		{
		/* see if new task has same RAM index as old, MUST */
		if (task_rom_list[rom_id].ram_tcb_index != ram_id)	
			{
			K_I_Func_Return();
			return(K_ERROR);
			}
		}
	PROC_DISABLE_INT;	/* disable interrupts */
	glob_tcbptr->tcbstate = K_KILL;	/* put task into proper state */
	PROC_ENABLE_INT;	/* enable interrupts */
	if (mode)			/* see if we need to create a new task */
		{
		K_Task_Create(rom_id);	/* yes, go create task */
		if (mode & 0x02)	/* see if we should also start task */
			K_Task_Start(ram_id);	/* yes, go start task */
		}
	PREEMPTED;	/* set the preempted flag */
	K_I_Func_Return();	/* re-enable task switching and test */
	return(K_OK);		/* return good to caller. Only if task did not kill itself */
}

/***********************************************************************

		TASK GENERAL PURPOSE EVENT SYNCHRONIZATION

************************************************************************/
/***************************************************************
 This allows a task to reset a task's event flags that they
 would like to be cleared.
***************************************************************/
byte K_Event_Reset(byte ram_id,byte event)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	if(K_I_Get_Ptr(ram_id))
		{
		K_I_Func_Return();
		return(K_ERROR);
		}
	glob_tcbptr->e_flags &= ~event;	/* place the event bit(s) into
											clear state, indicated by mask. */
	K_I_Func_Return();	/* release task block */
	return(K_OK);	/* return good status, if task not committing suicide */
}
/***************************************************************
 The following allows a task to wait for 1 or more events to be
 set, if they are not. Also a time period may be specified on how
 long the task will wait for at least 1 event to be set, in which
 the task wants.
MODE:
= 0 nothing
= 1 clear at beginning
= 2 auto clear on match
= 3 both 1 and 2
***************************************************************/
byte K_Event_Wait(byte match,word16 timecnt,byte mode)
{
	byte good_bits;

	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	PROC_DISABLE_INT;
	if (mode & 0x01) 	/* see if we should clear specified events at beginning. */
		{
		activetcb->e_flags &= ~match;	 /* yes, clear them. */
		activetcb->e_match = match;		/* This is my fix */
		goto K_Event_Wait_1;
		}
		/* see if ANY task flags match */
	if (!(activetcb->e_flags & (activetcb->e_match = match)))
		{
K_Event_Wait_1:
		PROC_ENABLE_INT;
		/* NO, so call CMX function that will suspend task. */
		if (K_I_Time_Common(timecnt,K_EVENT))	
			{
			return(0);	/* return the warning: that the time period expired */
			}
		K_I_Disable_Sched();	/* set task block. */
		}
	PROC_DISABLE_INT;
	good_bits = activetcb->e_flags & match;	/* what event bits matched. */
	if (mode & 0x02)	/* should we clear the events now? */
		{
		activetcb->e_flags &= ~match;	/* yes, clear them. */
		}
	PROC_ENABLE_INT;
	K_I_Func_Return();	/* release task lock. */
	return(good_bits);	/* return events that were set according to mask. */
}

/**********************************************************************
 The following function will set possibly set an event.

byte mode: 	0 = specific task
				1 = highest priority (single task even if more then one of same)
				2 = highest priority waiting task on THIS EVENT
				3 = all tasks
				4 = all waiting tasks THIS EVENT
				5 = all with same priority (specify priority)
				6 = all waiting with same priority (specify priority) THIS EVENT
************************************************************************/
byte K_Event_Signal(byte mode, byte tskid_pri,byte flag)
{
	RAM_TCB *tcbptr;	/* This is so this function is reentrant */
	/* Prior release used glob_tcbptr which could be corrupted by
		a interrupt using this function when a task was using it or
		if a interrupt interrupted another interrupt using this function */

	if (TEST_CMX_ACTIVE)	/* see if CMX RTOS active */
		{
		if (mode > 0x06)	/* see if mode greater then maximum allowed. */
			return(K_ERROR);

		K_I_Disable_Sched();	/* set task lock, task switching disabled */

		if (!mode)	/* is mode 0. */
			{
			if (tskid_pri > MAX_TASKS)	/* see if task exists */
				{
				K_I_Func_Return();
				return(K_ERROR);	/* it was so return error */
				}
			tcbptr = &cmx_tcb[tskid_pri];
			if (!(tcbptr->tcbstate))	/* see if NOT NULL */
				{
				K_I_Func_Return();
				return(K_ERROR);	/* it was so return error */
				}
			goto test_event;
			}
		else
			{
			tcbptr = cmx_tcb[0].nxttcb;	/* highest tcb ???? */
			}
		for (; tcbptr != cmx_tcb; tcbptr = tcbptr->nxttcb) /* see if we tested all. */
			{
			if (mode > 0x04)	/* is mode greater then 4? */
				{
				if (tcbptr->priority != tskid_pri)	/* see if priority match. */
					continue;
				}
			if (!(mode & 0x01))	/* see if task must be waiting on this EVENT */
				{
				if (!((tcbptr->e_match & flag) &&
					(tcbptr->tcbstate & K_EVENT)))
					{
					continue;	/* continue if task was not. */
					}
				}
test_event:
			PROC_SAVE_INTERRUPTS;
			tcbptr->e_flags |= flag;
			PROC_RESTORE_INTERRUPTS;
			/* see if task event(s) match. */
			if (tcbptr->e_match & (tcbptr->e_flags))
				{
				if (tcbptr->tcbstate & K_EVENT)	/* see if task was waiting on EVENT. */
					{
					tcbptr->tcbstate = K_RESUME; /* yes, the task may now resume. */
					tcbptr->tcbtimer = 1;
					
					if (TEST_NOT_PREEMPTED)	/* see if preempted flag not set yet. */
						{
						if ((activetcb->tcbstate != K_RUNNING) ||
							(tcbptr->priority < activetcb->priority)
							)
							PREEMPTED;	/* yes, set the preempted scheduling flag */
						}
					if (mode < 0x03)	/* see if we should exit function. */
						break;
					}
				}
			if (mode < 0x02)	/* see if we should exit function. */
				break;
			}
		K_I_Func_Return();	/* call CMX function, that will release task block */	
		return(K_OK);	/* return good status */
		}
	else
		return(K_ERROR);
}

/****************************************************************
 Start a task, if not already started.
*****************************************************************/
byte K_Task_Start(byte ram_id)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	if(K_I_Get_Ptr(ram_id))
		{
		K_I_Func_Return();
		return(K_ERROR);
		}
	if (glob_tcbptr->tcbstate & K_IDLE) /* see if task not started */
		{
		glob_tcbptr->tcbstate = K_READY;	/* place into the ready state */
		if (glob_tcbptr->priority < activetcb->priority)
			PREEMPTED;	/* yes, set the preempted scheduling flag */
		K_I_Func_Return();	/* re-enable task switching and test */
		return(K_OK);	/* return good status */
		}
	else
		{
		K_I_Func_Return();	/* re-enable task switching and test */
		return(K_ERROR);	/* return good status */
		}
}

/********************************************************************
 Common routine, called by other CMX functions to place task
 into suspended state, with or without time period enabled.
********************************************************************/
byte K_I_Time_Common(word16 timecnt,byte STATE)
{
	/* mask out all, but resource / mailbox number */
	activetcb->tcbtimer = timecnt;	/* load timer with value */
	if (STATE == K_EVENT)
		{
		PROC_DISABLE_INT;
		if (activetcb->e_flags & activetcb->e_match)
			{
			PROC_ENABLE_INT;
			K_I_Func_Return();	/* re-enable task switching and test */
			return(K_OK);	/* return successful status to caller */
			}
		}
	if (STATE == K_SEMAPHORE)
		{
		PROC_DISABLE_INT;
		if (sem_array[activetcb->wait].sem_count)	/* semaphore was posted as we came to this point!!! */
			{
			PROC_ENABLE_INT;
			K_I_Func_Return();	/* re-enable task switching and test */
			return(K_OK);	/* return successful status to caller */
			}
		}
	if (timecnt)	/* if timecnt > 0 */
		{
		SET_TIMER_ACTIVE;	/* set task timer active flag */
		STATE |= K_TIME;	/* change state to reflect time flag */
		}
	activetcb->tcbstate = STATE;	/* set task's state */
	PROC_ENABLE_INT;
	PREEMPTED;	/* set the preempted flag */
	K_I_Func_Return();	/* re-enable task switching and test */

	/* see if task was woken, because the thing it was waiting for happened,
		or that the time period specified has elapsed */
	if (activetcb->tcbtimer)
		return(K_OK);	/* return good status */
	else
		return(K_TIMEOUT);	/* return the warning: that the time period expired */
}

/*********************************************************************
 Allows task to wait on another task to wake it or time period to expire
*********************************************************************/
byte K_Task_Wait(word16 timecnt)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	
	/* call the function that will suspend this task, indicate the
		reason to suspend this task */
	return(K_I_Time_Common(timecnt,K_WAIT));	
}

/*********************************************************************
 Go wake a task, that is waiting on what ever
*********************************************************************/
byte K_Task_Wake(byte ram_id)
{
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	if(K_I_Get_Ptr(ram_id))
		{
		K_I_Func_Return();
		return(K_ERROR);
		}
	/* see if task waiting on this call, or task forcefully be woken up */
	PROC_DISABLE_INT;	/* disable interrupts */
	if	(glob_tcbptr->tcbstate & K_ANY_WAIT)	/* see if task waiting on anything */
		{
		glob_tcbptr->tcbstate = K_RESUME;	/* put task into the RESUME state */
		PROC_ENABLE_INT;	/* re-enable interrupts */
		glob_tcbptr->tcbtimer = 0;	/* let K_I_Time_Common function know task woken
											look like time ran out */

		/* see if this task has a higher priority then the current RUNNING task */
		if (glob_tcbptr->priority < activetcb->priority)
			PREEMPTED;	/* yes, set the preempted scheduling flag */
		K_I_Func_Return();	/* re-enable task switching and test */
		return(K_OK);	/* return good status */
		}
	PROC_ENABLE_INT;	/* re-enable interrupts */
	K_I_Func_Return();	/* re-enable task switching and test */
	return(K_NOT_WAITING);	/* return error that task was not waiting */
}

/*****************************************************************

!!! WARNING: the timer task will not run, so task timers will NOT
	 be serviced. Task MUST NOT call any function that may suspend this task
*****************************************************************/
void K_Task_Lock(void)
{
	/* set the task lock flag. This will stop the K_I_Scheduler
		from re-scheduling, until the K_Task_Unlock function is called */

	K_I_Disable_Sched();	/* set task lock, task switching disabled */
}
void K_Task_Unlock(void)
{
	/* lower the task privilege flag. See if the K_I_Scheduler
		should be invoked */

	K_I_Func_Return();	/* re-enable task switching and test */
}

/**************************************************************
		MESSAGES
**************************************************************/

/**********************************************************
 The following is the generalize message wait function
**********************************************************/
void * K_I_Mesg_Wait_Common(byte mailbox,word16 timecnt,byte is_get)
{
	MAILBOX *mail_ptr;
	byte *copy_ptr;
	MSG *link;

	if (mailbox >= MAX_MAILBOXES)	/* see if mailbox number OK. */
		return(NULL);	/* no, return null idicating error. */
	K_I_Disable_Sched();	/* set task block. */
	mail_ptr = &mail_box[mailbox];	/* address of the CMX mailbox handler. */
	if (mail_ptr->waiter)	/* see if another task already "owns" this mailbox. */
		{
		K_I_Func_Return();	/* release task block. */
		return(NULL);	/* return error, for only one task may wait on mailbox. */
		}
	if (!mail_ptr->first_lnk) 	/* any messages present in mailbox, null = no */
		{
		if (is_get)	/* K_Mesg_Get function used, do not wait */
			{
			K_I_Func_Return();	/* release task block. */
			return(NULL);	/* return error. */
			}
		mail_ptr->waiter = activetcb;	/* identify who "owns" mailbox. */
		K_I_Time_Common(timecnt,K_MESSAGE);	/* go suspend task. */

		/* task will be return to here by the K_I_Time_Common function. */
		K_I_Disable_Sched();	/* set task lock */
		mail_ptr->waiter = NULL;	/* now task will NOT own mailbox. */
		/* test to see if time expired or task forcefully woken, prior
			to a message being set to this mailbox. */
		if (!activetcb->tcbtimer)
			{
			K_I_Func_Return();
			return(NULL);	/* yes, pass back null, indicating no message. */
			}
		}

	link = mail_ptr->first_lnk;	/* get first link. */
	link->env_link = message_ptr->env_link; /* re-arrange linked list. */
	message_ptr->env_link = link;
	message_free++;	/* increment message free counter. */

	copy_ptr = link->message;		/* address of message */
	/* update link to point to next message link, also test if more messages
		present. */
	if ((mail_ptr->first_lnk = link->link))
		{
		if (mail_ptr->task_num)	/* see if mailbox is suppose to signal event,
											that it has messages. */
			/* yes, then go set proper event bit. */
			K_Event_Signal(0,mail_ptr->task_num,mail_ptr->event_num);
		}
	link->link = NULL;	/* set message link to NULL. */
	K_I_Func_Return();	/* release task block */
	return((void *)copy_ptr);	/* return good status to caller */
}

/*******************************************************************
 General function for sending a message to a mailbox.
*******************************************************************/
byte K_I_Mesg_Send_Common(byte mailbox,byte wait,word16 timecnt,void *mesg)
{
	MAILBOX *mail_ptr;
	MSG *link;	/* scratch pointer */

	K_I_Disable_Sched();
	/* see if mailbox within range and that there are
		message links available. */
	if ((mailbox >= MAX_MAILBOXES) || (!message_free))	
		{
		/* error */
		K_I_Func_Return();	/* release task block */
		return(K_ERROR);	/* return good status to caller */
		}
	mail_ptr = &mail_box[mailbox];	/* CMX mailbox handler. */
	if (mail_ptr->waiter)	/* task waiting on this mailbox */
		{
		mail_ptr->waiter->tcbstate = K_RESUME;	/* allow task to run again */
		mail_ptr->waiter->tcbtimer = 1;	/* task woke before time out */
		if (mail_ptr->waiter->priority < activetcb->priority)
			PREEMPTED;	/* yes, so set the preempted scheduling flag */
		}

	link = message_ptr;	/* link now points to message header */
	message_ptr = link->env_link; /* update linked list */

	if (mail_ptr->first_lnk)	/* see if first link set */
		{
		mail_ptr->next_lnk->link = link;	/* yes, set next link. */
		}
	else
		{
		mail_ptr->first_lnk = link;	/* set first link. */
		/* because this is the first message into the mailbox
			see if mailbox is suppose to notify task that there
			are messages now residing in mailbox. */
		if (mail_ptr->task_num)	/* does task want to be notified? */
			/* yes, go set event. */
			K_Event_Signal(0,mail_ptr->task_num,mail_ptr->event_num);
		}
	mail_ptr->next_lnk = link;	/* set mailbox next link. */
	link->message = (byte *)mesg;	/* load address of message. */
	--message_free;	/* decrement message free */

	if (wait)	/* is this the send and wait call function */
		{
		/* this task will now be suspended. */
		return(K_I_Time_Common(timecnt,K_WAIT));
		}
	else
		{
		K_I_Func_Return();	/* release task block */
		return(K_OK);	/* return good status to caller */
		}
}

/************************************************************
 the following function sets up a mailbox to automatically
 set an event of a particular task, when and if this mailbox
 has messages in it.
*************************************************************/
byte K_Mbox_Event_Set(byte mailbox,byte taskid,byte event_num)
{
	MAILBOX *mail_ptr;

	if (mailbox >= MAX_MAILBOXES) /* see if mailbox number OK. */
		return(K_ERROR);	/* NO, send error status. */
	K_I_Disable_Sched();	/* set task block. */

	mail_ptr = &mail_box[mailbox];	/* get CMX mailbox handler */
	mail_ptr->task_num = taskid;	/* load task_num with task slot number. */
	mail_ptr->event_num = event_num;	/* what event bit will be set. */
	K_I_Func_Return();	/* release task block. */
	return(K_OK);	/* return good status. */
}
/*******************************************************************
		RESOURCE MANAGER
********************************************************************/
/*******************************************************************
 this allows a task to either get a resource or wait for resource if busy
******************************************************************/
/*
mode = 0 = K_Resource_Get
mode = 1 = K_Resource_Wait
*/
byte K_I_Resource_Common(byte res_grp, word16 timecnt, byte mode)
{
	if (res_grp >= MAX_RESOURCES)	/* see if resource group exists */
		return(K_ERROR);	/* no, return error status */

	K_I_Disable_Sched();	/* set task lock, task switching disabled */

	if (res_que[res_grp].owner)	/* see if another task already owns this */
		{
		if (mode)		/* did the K_Resource_Wait function call this function */
			{
			activetcb->wait = res_grp;	/* save what resource group */
			return(K_I_Time_Common(timecnt,K_RESOURCE));	/* common CMX suspend function */
			}
		else
			{
			 K_I_Func_Return();	/* re-enable task switching and test */
			 return(K_RESOURCE_OWNED);	/* return error that resource is already owned */
			}
		}
	res_que[res_grp].owner = activetcb;	/* identify who owns this resource */
	K_I_Func_Return();	/* re-enable task switching and test */
	return(K_OK);	/* return successful status to caller */
}

/**************************************************************************
 This allows a task to release the resource that it owns and possibly
 wake up another task waiting for this resource.
*************************************************************************/
byte K_Resource_Release(byte res_grp)
{
	if (res_grp >= MAX_RESOURCES)	/* see if resource group exists */
		return(K_ERROR);	/* no, return error status */

	if (res_que[res_grp].owner != activetcb) /* test to see if task owns it */
		{
		return(K_RESOURCE_NOT_OWNED);	/* resource not owned by this task */
		}
	K_I_Disable_Sched();	/* set task lock, task switching disabled */
	glob_tcbptr = cmx_tcb[0].nxttcb;
	do {
		/* see if another task wants ownership of this resource */
		if (glob_tcbptr->tcbstate & K_RESOURCE)
			{
			if (glob_tcbptr->wait == res_grp) /* right group */
				{
				res_que[res_grp].owner = glob_tcbptr;	/* tell resource who owns it */
				glob_tcbptr->tcbstate = K_RESUME;	/* wake task up */
				/* tell K_I_Time_Common function, time period did not expire */
				glob_tcbptr->tcbtimer = 1;
				if (glob_tcbptr->priority < activetcb->priority)
					PREEMPTED;  /* yes, set the preempted scheduling flag */
				goto cxrel_exit;
				}
			}
		glob_tcbptr = glob_tcbptr->nxttcb;
		} while (glob_tcbptr != cmx_tcb);	/* test next task */
	res_que[res_grp].owner = NULL;	/* no task waiting on this resource */
cxrel_exit:
	K_I_Func_Return();	/* re-enable task switching and test */
	return(K_OK);	/* return good status to caller */
}
/********************************************************************
 Enter into the CMX RTOS. Once entered, the CMX OS takes over
********************************************************************/
void K_OS_Start(void)
{
	PROC_DISABLE_INT;	/* disable interrupts */
	PREEMPTED;			/* set preempted flag */
	CMX_ACTIVE;			/* show that CMX RTOS active */
	K_I_Scheduler();		/* invoke CMX K_I_Scheduler */
}

/********************************************************************
		CYCLIC TIMER FUNCTIONS
*********************************************************************/
/* tproc_start states:
	0 = stopped
	1 = started
*/

/********************************************************************
 This function sets up the cyclic timer's event parameters.
********************************************************************/
byte K_Timer_Create(byte timed_num,byte mode,byte tskid_pri,byte event_num)
{
	struct _tcproc *tpptr;

	if (timed_num >= MAX_CYCLIC_TIMERS)	/* have we exceeded the maximum number */
		return(K_ERROR);	/* yes, return error status */
	K_I_Disable_Sched();	/* set task block */
	tpptr = &tcproc[timed_num];	/* get address of cyclic timer handler */
	tpptr->mode = mode;	/* load K_Event_Signal MODE */
	tpptr->tskid_pri = tskid_pri;	/* load in the tasks slot number
												or priority. May not be used. */
	tpptr->event_num = event_num;	/* load in the event to be set. */
	K_I_Func_Return();	/* release task block */
	return(K_OK);	/* return good status */
}

/******************************************************************
 This is the common routine that the most of the cyclic timer functions
 will call. This function starts, or restarts a cyclic timer and possibly
 also changes the cyclic timer's time parameters.

mode:
= 0: = just restart timer if not started
= 1: = change only initial time and start
= 2: = change only cyclic time and start
= 3: = change both initial time and cyclic time and start
**************************************************************************/
byte K_I_Cyclic_Common(byte timed_num,word16 timecnt, word16 cyclic,byte mode)
{
	struct _tcproc *tpptr;

	if (timed_num >= MAX_CYCLIC_TIMERS)	/* have we exceeded the maximum number */
		return(K_ERROR);
	K_I_Disable_Sched();	/* set task block */
	tpptr = &tcproc[timed_num];	/* get the CMX handler for this cyclic timer. */
	if (mode & 0x01)	/* see if mode bit 0 set. */
		{
		tpptr->tproc_timer = timecnt;	/* load time period into counter */
		}
	if (mode & 0x02)	/* see if mode bit 1 set. */
		{
		tpptr->reload_time = cyclic;	/* load cyclic time into counter */
		}
	if (!(tpptr->tproc_start))	/* see if 0 */
		{
		tpptr->tproc_start = 1;	/* start the timed procedure */
		SET_TIMER_ACTIVE;	/* set task timer active flag */
		}
	K_I_Func_Return();	/* release task block */
	return(K_OK);	/* return good status */
}

/******************************************************************
 stop a cyclic timer function
******************************************************************/
byte K_Timer_Stop(byte timed_num)
{
	struct _tcproc *tpptr;

	if (timed_num >= MAX_CYCLIC_TIMERS)	/* have we exceeded the maximum number */
		return(K_ERROR);
	K_I_Disable_Sched();	/* set task block */
	tpptr = &tcproc[timed_num];	/* address of cyclic timer structure. */
	if (tpptr->tproc_start)	/* see if non 0 */
		{
		tpptr->tproc_start = 0;	/* stop cyclic timer. */
		}
	K_I_Func_Return();	/* release task block */
	return(K_OK);	/* return good status */
}

/*******************************************************************
 This is the CMX timer task, that will service 1 or more tasks timers.
 When a task timer count decrements down to 0, then that task will
 be place into the resume state, thus allowing the task to run again
********************************************************************/
void K_I_Timer_Task(void)
{
	struct _tcproc *tpptr;

	CLR_TIMER_ACTIVE;	/* reset timer active flag */
	glob_tcbptr = cmx_tcb[0].nxttcb;
	do {
		PROC_DISABLE_INT;	/* disable interrupts */
		if (glob_tcbptr->tcbstate & K_TIME)	/* is task waiting on time */
			{
			if (!(--glob_tcbptr->tcbtimer))	/* decrement it's count */
				{
				glob_tcbptr->tcbstate = K_RESUME;	/* place task into resume satet */
				PROC_ENABLE_INT;	/* enable interrupts */
				if (TEST_NOT_PREEMPTED)	/* have we previously set preempted flag */
					{
					if (glob_tcbptr->priority < activetcb->priority)
						PREEMPTED;	/* yes, set the preempted scheduling flag */
					}
				}
			else
				{
				SET_TIMER_ACTIVE; /* set the timer test flag, for task timer not 0 */
				}
			}
		PROC_ENABLE_INT;	/* enable interrupts */
		glob_tcbptr = glob_tcbptr->nxttcb;
		} while (glob_tcbptr != cmx_tcb);	/* test next task */

	tpptr = tcproc;
	while (tpptr < &tcproc[MAX_CYCLIC_TIMERS]) {
		if (tpptr->tproc_start)	/* is cyclic timer waiting on time */
			{
			if (!(--tpptr->tproc_timer))	/* decrement it's count */
				{
				if ((tpptr->tproc_timer = tpptr->reload_time) == 0)
					{
					/* if cyclic time period 0, then stop cyclic timer */
					tpptr->tproc_start = 0;
					}
				else
					{
					SET_TIMER_ACTIVE; /* set the timer test flag, for cyclic timer not 0 */
					}
				/* execute the cyclic timer's K_Event_Signal function parameters. */
				K_Event_Signal(tpptr->mode,tpptr->tskid_pri,tpptr->event_num);
				}
			else
				{
				SET_TIMER_ACTIVE; /* set the timer test flag, for cyclic timer not 0 */
				}
			}
		tpptr++;
		} 	/* test next timer */

}	/* K_I_Timer_Task */

/*****************************************************************
 Semaphore stuff
******************************************************************/
/*
 Pend on semaphore, if count 0, otherwise return with semaphore

 mode 0 : wait if semaphore not present
 mode 1 : do NOT wait if semaphore not present, return immediatedly

*/

byte K_I_Semaphore_Get_Common(byte sem_num, word16 timecnt, byte mode)	
{
	SEM *sem_ptr;

	if (sem_num >= MAX_SEMAPHORES)
		return(K_ERROR);	/* return good status to caller */

	K_I_Disable_Sched();	/* set task lock, task switching disabled */

	sem_ptr = &sem_array[sem_num];	/* CMX semaphore handler. */
	if (sem_ptr->owner)	/* see if another task already owns this */
		{
		K_I_Func_Return();	/* release task block */
		return(K_ERROR);	/* return error status to caller */
		}
	PROC_DISABLE_INT;
	if (!sem_ptr->sem_count)
		{
		if (mode)
			{
			PROC_ENABLE_INT;
			K_I_Func_Return();	/* release task block */
			return(K_SEMAPHORE_NONE);	/* return error that semaphore
											had not been posted to */
			}
		sem_ptr->owner = activetcb;
		PROC_ENABLE_INT;
		activetcb->wait = sem_num;
		if (K_I_Time_Common(timecnt,K_SEMAPHORE))	/* common CMX suspend function */
			{
			sem_ptr->owner = NULL;
			return(K_SEMAPHORE_NONE);	/* return error that semaphore
											had not been posted to */
			}
		sem_ptr->owner = NULL;
		K_I_Disable_Sched();	/* set task block. */
		}
	PROC_DISABLE_INT;
	if ((--sem_ptr->sem_count) == 0xffff)
		sem_ptr->sem_count = 0;
	PROC_ENABLE_INT;
	K_I_Func_Return();
	return(K_OK);
}	/* K_I_Semaphore_Get_Common */

byte K_Semaphore_Post(byte sem_num)	
{
	SEM *sem_ptr;
	RAM_TCB *tcbptr;	/* This is so this function is reentrant
								because interrupts can call this directly */

	if (sem_num >= MAX_SEMAPHORES)
		{
		return(K_ERROR);	/* return good status to caller */
		}
	if (TEST_CMX_ACTIVE)	/* see if CMX RTOS active */
		{
		K_I_Disable_Sched();
		sem_ptr = &sem_array[sem_num];	/* CMX semaphore handler. */
		PROC_SAVE_INTERRUPTS;
		sem_ptr->sem_count++;
		tcbptr = sem_ptr->owner;
		PROC_RESTORE_INTERRUPTS;
		/* Fixed 02/09/07 */
		if (tcbptr != NULL)
			{
		if (tcbptr->tcbstate & K_SEMAPHORE)	/* see if task was waiting on SEAMPHORE. */
			{
			tcbptr->tcbstate = K_RESUME; /* yes, the task may now resume. */
			tcbptr->tcbtimer = 1;
			
			if (TEST_NOT_PREEMPTED)	/* see if preempted flag not set yet. */
				{
				if (tcbptr->priority < activetcb->priority)
					PREEMPTED;	/* yes, set the preempted scheduling flag */
				}
			}
			}
		K_I_Func_Return();	/* call CMX function, that will release task block */	
		return(K_OK);	/* return good status */
		}
	else
		return(K_ERROR);
}	/* K_Semaphore_Post */

byte K_Semaphore_Reset(byte sem_num)	
{
	SEM *sem_ptr;

	if (sem_num >= MAX_SEMAPHORES)
		{
		return(K_ERROR);	/* return error status to caller */
		}
	K_I_Disable_Sched();
	sem_ptr = &sem_array[sem_num];	/* CMX semaphore handler. */
	PROC_DISABLE_INT;
	sem_ptr->sem_count = 0;
	glob_tcbptr = sem_ptr->owner;
	/* Fixed 02/09/07 */
	if (glob_tcbptr != NULL)
		{
		sem_ptr->owner = NULL;
		}
	PROC_ENABLE_INT;
	if (glob_tcbptr != NULL)
		{
	if (glob_tcbptr->tcbstate & K_SEMAPHORE)	/* see if task was waiting on SEAMPHORE. */
		{
		glob_tcbptr->tcbstate = K_RESUME; /* yes, the task may now resume. */
		glob_tcbptr->tcbtimer = 0;
		
		if (TEST_NOT_PREEMPTED)	/* see if preempted flag not set yet. */
			{
			if (glob_tcbptr->priority < activetcb->priority)
				PREEMPTED;	/* yes, set the preempted scheduling flag */
			}
		}
		}
	K_I_Func_Return();	/* call CMX function, that will release task block */	
	return(K_OK);	/* return good status */
}



