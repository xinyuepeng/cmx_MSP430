/*********************************************************************
 The following will be included when the cmx_inip.c program gets
 compiled. These are set the maximums for a variety of things. This way
 CMX can pre-allocate the needed memory to accomplish the user
 needs. This is so no memory allocation functions are needed by
 CMX, and no delays are introduced to obtain the needed memory during
 run time.

 The user is free to make a copy of this file for each application
 program, so each application program will have their own "cpconfig.h"
 file. Also the user should probally copy the cmx_inip.c file to a "user
 name file", for each application program. This way the user does not
 have to change "this" file each time a different application program
 is worked with. Just remember to make sure you change the 
 #include <cpconfig.h> line in the user named ("cmx_inip.c") file
 to include the proper user named "cpconfig.h" file.

 Also if any of these defines ARE changed, then the user must be
 sure to compile the cmx_inip.c file (or equalivent user file)
 so these NEW values take effect.
 *********************************************************************/
#define C_MAX_TASKS 5	/* Maximum number of tasks for this program. */
#define C_ROM_TASKS 5	/* Maximum number of tasks for this program. */
#define C_MAX_RESOURCES 3	/* Maximum number of resources. */
#define C_MAX_CYCLIC_TIMERS 3	/* Maximum number of cyclic timers. */
#define C_MAX_MESSAGES 3	/* Maximum number of messages. */
#define C_MAX_MAILBOXES 2	/* Maximum number of mailboxes. */
#define C_MAX_SEMAPHORES 3	/* Maximum number of semaphores. */
#define C_INTERRUPT_SIZE 64	/* Size of interrupt stack. The CMX timer task
 scheduler, and int_action function use this. */

#define C_RTC_SCALE 1		/* number of timer interrupt ticks, before CMX 
 tick. All time counters based on this.  */

#define CMX_RAM_INIT 1		/* ENABLE or DISABLE the initialization of CMX RAM
 variables. Usually done by Compiler start up code
 in most cases. If not, MUST be ENABLED. Normally
 compiler code will set GLOBAL variables that are
 initialized and set non-initialize variables to
 0 (zero). Set to 1 to ENABLE CMX code to initialize
 it's variables, set to 0 (zero) to DISABLE */

