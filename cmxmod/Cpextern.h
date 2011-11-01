#ifndef CMX_CXEXTERN_H
#define CMX_CXEXTERN_H

extern byte MAX_TASKS;
extern byte ROM_TASKS;
extern byte MAX_RESOURCES;
extern byte MAX_CYCLIC_TIMERS;
extern byte MAX_MAILBOXES;
extern byte MAX_QUEUES;
extern byte RTC_SCALE;
extern byte MAX_SEMAPHORES;

extern RAM_TCB cmx_tcb[];

extern MAILBOX mail_box[];

extern MSG message_box[];

extern MSG *message_ptr;
extern word16 message_free;

extern RESHDR res_que[];

extern SEM sem_array[];

extern CYCLIC_TIMERS tcproc[];

extern const ROM_TCB task_rom_list[];	/* CMX ROM TCB array, do NOT change name */

extern RAM_TCB *glob_tcbptr;

#endif /* CMX_CXEXTERN_H */

