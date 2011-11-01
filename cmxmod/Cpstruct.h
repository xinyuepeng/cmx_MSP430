#ifndef CMX_CXSTRUCT_H
#define CMX_CXSTRUCT_H

/* structure of task's ROM parameters */
typedef struct 
{	/* task function, stack start address, priority, reg bank ??? */
	void (*task_addr)(void);		/* the task's CODE address */
	word16 *system_stack;	/* task system stack start address */
	byte ram_tcb_index;		/* the RAM TCB array index number */
	byte init_priority;		/* the priority of this task */

} ROM_TCB;	/* NOTE:  6 byte structure */

/* structure of task's RAM parameters */
typedef struct _tcb {
	word16 tcbtimer;		/* the timer counter */
	word16 *stack_save;	/* stack saved address */
	struct _tcb *nxttcb;	/* pointer to next tcb */
	const ROM_TCB *rom_tcb_index;		/* address into the ROM_TCB array for this task */
	byte tcbstate;				/* state of task */
	byte wait;					/* wait on mailbox / resource */
	byte priority;				/* the priority of this task */
	byte e_flags;				/* flags for this task */
	byte e_match;				/* the flags states for match */
	} RAM_TCB;	/* NOTE: 12 byte structure */

typedef struct cmxmesg {
	struct cmxmesg *env_link;	/* link to next message block. */
	struct cmxmesg *link;		/* link to mailbox. */
	byte *message;					/* address of message. */
	} MSG;	/* NOTE: 6 byte structure */


typedef struct cmxmbox {
	MSG *first_lnk;		/* link to message block. */
	MSG *next_lnk;			/* link to message block. */
	RAM_TCB *waiter;		/* what task is waiting for message. */
	byte task_num;			/* task number for setting event bit */
	byte event_num;		/* event number */
	} MAILBOX;	/* NOTE: 8 byte structure */

/* resource structure */
typedef struct cmxresource {
	RAM_TCB *owner;	/* for now */
	} RESHDR;	/* NOTE: 2 byte structure */

/* the timed procedure structure */
typedef struct _tcproc {
	word16 tproc_timer;	/* the timer counter */
	word16 reload_time;	/* the time value that will be reloaded */
	byte tproc_start;		/* byte indicating timed procedure stopped / started */
	byte mode;				/* the cyclic timer event mode setting */
	byte tskid_pri;		/* the tskid or priority event setting */
	byte event_num;		/* the event bit(s) to set */
	} CYCLIC_TIMERS;	/* NOTE: 8 byte structure */

typedef struct semaphore {
		RAM_TCB *owner;		/* what task is waiting for message. */
		word16 sem_count;
		} SEM;

#endif /* CMX_CXSTRUCT_H */

