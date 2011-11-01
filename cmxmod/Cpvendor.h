#ifndef CMX_CXVENDOR_H
#define CMX_CXVENDOR_H

/* for Microchip dsPIC processor family */
#define PROCESSOR dsPIC

#define PROC_DISABLE_INT K_OS_Disable_Interrupts()	/* could be assembly, 
															depends on compiler */
#define PROC_ENABLE_INT K_OS_Enable_Interrupts()	/* could be assembly, 
															depends on compiler */

#define PROC_SAVE_INTERRUPTS K_OS_Save_Interrupts()
#define PROC_RESTORE_INTERRUPTS K_OS_Restore_Interrupts()

#ifdef CMX_INIT_MODULE 

byte int_count;		/* counts depth of interrupts */
byte locked_out;	/* task lock out flag, disables scheduling */
byte cmx_flag1;		/* CMX flag register, internal use */
word16 sr_holder;		/* holder for interrupt state, internal use */
struct _tcb *activetcb;		/* activetcb pointer */
word16 *stack_holder;	/* stack holder */ 
word16 *interrupt_stack; /* interrupt stack */

#else

extern byte int_count;
extern byte locked_out;	
extern byte cmx_flag1;
extern word16 sr_holder;		/* holder for interrupt state, internal use */
extern struct _tcb *activetcb;		/* activetcb pointer */
extern word16 *stack_holder;	/* stack holder */ 
extern word16 *interrupt_stack; /* interrupt stack */

#endif

/* CMX cmx_flag1 bit representation */
#define preempted 0x01	/* higher priority task ready, or task running going
									to sleep */
#define do_timer_tsk 0x02	/* do CMX timer task */
#define do_coop_sched 0x10	/* perform a cooperative reschedule, preempted 
										flag over rules this */
#define timer_active 0x20	/* shows (probally) at least 1 task timers 
										need servicing */
#define idle_flag	0x40		/* informs K_I_Scheduler, to test for power down state */
#define cmx_active 0x80		/* informs CMX, that user has entered RTOS */


/* Do NOT change these */
#define PREEMPTED cmx_flag1 |= preempted
#define TEST_NOT_PREEMPTED !(cmx_flag1 & preempted)
#define DO_TIMER_TSK cmx_flag1 |= do_timer_tsk
#define DO_COOP_SCHED cmx_flag1 |= do_coop_sched
#define SET_TIMER_ACTIVE cmx_flag1 |= timer_active
#define CLR_TIMER_ACTIVE cmx_flag1 &= ~timer_active
#define TEST_TIMER_ACTIVE cmx_flag1 & timer_active
#define TEST_CMX_ACTIVE cmx_flag1 & cmx_active
#define CMX_ACTIVE cmx_flag1 |= cmx_active

#endif /* CMX_CXVENDOR_H */

