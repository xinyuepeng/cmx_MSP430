#ifndef CMX_CXDEFINE_H
#define CMX_CXDEFINE_H

#ifdef byte
#undef byte
#endif
#ifdef word16
#undef word16
#endif
#ifdef word32
#undef word32
#endif
typedef unsigned short word16;
typedef unsigned char byte;
typedef unsigned int bit_word16;
typedef signed short sign_word16;
typedef unsigned long word32;
#ifndef NULL
#define NULL 0x00
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define K_KILL 0x00				/* the task is killed */
#define K_EVENT 0x01			/* waiting for event */
#define K_RESOURCE 0x02		/*	wait for resource */
#define K_MESSAGE 0x04			/* waiting on message */
#define K_TIME 0x08				/* wait on time */
#define K_WAIT 0x10				/* general wait */
#define K_SEMAPHORE (K_WAIT | K_MESSAGE)		/* waiting on semaphore */

#define K_ANY_WAIT 0x1F		/* any wait mask */

#define K_IDLE 0x20				/* the task is idle state */
#define K_READY 0x40 			/* the task is ready to run (from beginning) */
#define K_RESUME 0X80			/* the task is ready to run (not from beginning) */
#define K_RUNNING 0XC0			/* the task is ready to run (not from beginning) */


/* status return values */
#define K_OK 0x00				/* successful call */
#define K_TIMEOUT 0x01			/* either time period ran out, or task woken by K_Task_Wake */
#define K_NOT_WAITING 0x02		/* task not waiting */
#define K_RESOURCE_OWNED 0x05	/* resource already owned */
#define K_RESOURCE_NOT_OWNED 0x06 	/* resource not owned by caller */
#define K_SEMAPHORE_NONE 0x0c		/* no semaphore posted, returned (only cxsemget call */	
#define K_ERROR 0xff				/* general purpose error */

#define K_I_Disable_Sched()   (++locked_out) /* disable task switching */
#define K_I_Enable_Sched() {if ((cmx_flag1 & (preempted | do_timer_tsk | do_coop_sched)) && (locked_out == 1)) \
K_I_Sched(); else --locked_out; } 

#endif /* CMX_CXDEFINE_H */
