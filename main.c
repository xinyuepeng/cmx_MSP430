/* the following program does really nothing, except to show the
user different function calls and how to set them up. */

#include "cpfuncs.h"
#include "SysConfig.h"
#include "PowerClockControl.h"
#include "Alarm.h"
#include "HMI.h"
#include "KeyInput.h"
#include "SystemState.h"
#include "SystemData.h"

#define TASK_FRESH_LCD  0
#define CMX_TEST        1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

#define RESOURCE0	0	/* resource 0 number */

#define MBOX0 0		/* mailbox 0 number */
#define MBOX1 1		/* mailbox 1 number */

#define CYCLIC0 0		/* cyclic timer 0 number */
#define CYCLIC1 1		/* cyclic timer 1 number */

#define SEM0 0		/* SEM 0 number */

#define TSK1_EVENT1 0x01	/* task 1 event bit 0 */
#define TSK1_EVENT2 0x02	/* task 1 event bit 1 */
#define TSK1_EVENT3 0x04	/* task 1 event bit 2 */

#define TSK2_EVENT1 0x01	/* task 2 event bit 0 */
#define TSK2_EVENT2 0x02	/* task 2 event bit 1 */
#define TSK2_EVENT3 0x04	/* task 2 event bit 2 */

#define TSK3_EVENT1 0x01	/* task 3 event bit 0 */
#define TSK3_EVENT2 0x02	/* task 3 event bit 1 */
#define TSK3_EVENT3 0x04	/* task 3 event bit 2 */

#define TSK6_EVENT1 0x01	/* task 6 event bit 0 */
#define TSK6_EVENT2 0x02	/* task 6 event bit 1 */
#define TSK6_EVENT3 0x04	/* task 6 event bit 2 */

#define TASK1_ROM 1	/* task 1 index position within ROM TCB */
#define TASK2_ROM 2	/* task 2 index position within ROM TCB */
#define TASK3_ROM 3	/* task 3 index position within ROM TCB */
#define TASK4_ROM 4	/* task 4 index position within ROM TCB */
#define TASK5_ROM 5	/* task 5 index position within ROM TCB */
#define TASK6_ROM 6	/* task 6 index position within ROM TCB */
#define TASK7_ROM 7	/* task 7 index position within ROM TCB */

#define TASK1_RAM_POS 1	/* task 1 index position within RAM TCB */
#define TASK2_RAM_POS 2	/* task 2 index position within RAM TCB */
#define TASK3_RAM_POS 3	/* task 3 index position within RAM TCB */
#define TASK4_RAM_POS 4	/* task 4 index position within RAM TCB */
#define TASK5_RAM_POS 5	/* task 5 index position within RAM TCB */
#define TASK6_RAM_POS 6	/* task 6 index position within RAM TCB */
#define TASK7_RAM_POS 4	/* task 7 index position within RAM TCB */


/* NOTE: task 4 and 7 have common index number within RAM TCB,
	therefore only 1 of these tasks, may be active at any time */

void ADC_Reading_task(void);
void HMI_Reflash_task(void);
void Sys_state_task(void);
void KeyEvent_task(void);
void task5(void);
void task6(void);
void task7(void);

word16 stacks[282];	

const ROM_TCB task_rom_list[] =
	/* tsk addr, system stk, ram tcb index, priority */
	/* NOTE: notice how user stk and reg_bank are NOT USED, for HC08 family
				does NOT use them. */
{
    K_I_Timer_Task,0,0,0,
    ADC_Reading_task,&stacks[280],TASK1_RAM_POS,2,
#if TASK_FRESH_LCD    
    HMI_Reflash_task,&stacks[200],TASK2_RAM_POS,3,
    Sys_state_task, &stacks[120], TASK3_RAM_POS,4,
    KeyEvent_task, &stacks[80], TASK4_RAM_POS,5,
#else
    Sys_state_task, &stacks[200], TASK2_RAM_POS,3,
    KeyEvent_task, &stacks[120], TASK3_RAM_POS,4,
#endif
};

/* NOTE: task 4 and 7 have common index number within RAM TCB,
	therefore only 1 of these tasks, may be active at any time.
	Also note that they have the same system stack address, but
	their priorities are different. */

byte enable_semaphores;

void common_error(void);
void int_handler(void);

void ADC_Reading_task(void)
{
	byte status = 0;
        static unsigned counter = 0;
        while(1)
        {
          K_Task_Wait((unsigned short)500);
          Task_GetADC();
        }
	
	K_Task_End();
}

void HMI_Reflash_task(void)
{
	byte status = 0;
	
        while(1)
        {
#if CMX_TEST == 0          
            K_Task_Wait(1);
#else
            K_Task_Wait(1000);
#endif            
            HMI_FreshDisplay();
        }

	K_Task_End();	/* now end task, put into IDLE state */
}

void Sys_state_task(void)
{   
    unsigned int counter = 0;
    unsigned char event_bits;
    while(1)
    {
#if TASK_FRESH_LCD       
        event_bits = K_Event_Wait(TSK3_EVENT1 | TSK3_EVENT2, 0, 2);
#else
        event_bits = K_Event_Wait(TSK2_EVENT1 | TSK2_EVENT2, 0, 2);
#endif        
        if(event_bits == 0)
        {
        }
        else
        {
#if TASK_FRESH_LCD           
            if(event_bits & TSK3_EVENT1)
#else
            if(event_bits & TSK2_EVENT1)
#endif              
            {
                if(BACKLIGHT_TIMER==1)
                {
                    DisableBackLight();
                }
                else if(BACKLIGHT_TIMER > 1)
                    BACKLIGHT_TIMER--;
                else
                    DisableBackLight();
                SysStateTimer();
            }
#if TASK_FRESH_LCD           
            if(TSK3_EVENT2 & event_bits)
#else
            if(TSK2_EVENT2 & event_bits)
#endif              
            {
            
                if(Key_Pressed())
                    SysStateKeyHandler(Key_ReadKey());
                else
                    SysStateKeyHandler(Key_PopKey());
            
            }
        }
        /*
            handler timer and keyinput event
        */
        UpdateSystemScreen();
       
    }
    
    K_Task_End();
}

void KeyEvent_task(void)
{
    unsigned char status;
    while(1)
    {
        K_Task_Wait(10);
 
        Task_KeyScan();
        if(Key_Hit()) {
#if TASK_FRESH_LCD           
            status = K_Event_Signal(0, TASK3_RAM_POS, TSK3_EVENT2);
#else
            status = K_Event_Signal(0, TASK2_RAM_POS, TSK2_EVENT2);
#endif            
            if(status != K_OK)
            {
                Key_ClearBuf();
            }
        }
        /*
            handler timer and keyinput event
        */
    }
    
    K_Task_End();
}

void common_error(void)
{
	while(1)
		{
		;
		}
}

int main(void)
{
	byte status;

	/* WARNING: we are disabling watchdog timer!!! */
        WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	K_OS_Init();	 /* initialize ram and things */
	status = K_Task_Create(TASK1_ROM);	/* create task 1 */
#if CMX_TEST == 0        
	status = K_Task_Create(TASK2_ROM);	/* create task 2 */
        status = K_Task_Create(TASK3_ROM);
    #if TASK_FRESH_LCD        
        status = K_Task_Create(TASK4_ROM);       
	status = K_Timer_Create(CYCLIC0,0,TASK3_RAM_POS,TSK3_EVENT1);	/* set up cyclic timer 0 */
    #else
        status = K_Timer_Create(CYCLIC0,0,TASK2_RAM_POS,TSK2_EVENT1);
    #endif        
	status = K_Timer_Start(CYCLIC0,10,10);		/* now start cyclic timer 0 */
#endif        
        status = K_Task_Start(TASK1_ROM);
#if CMX_TEST == 0        
        status = K_Task_Start(TASK2_ROM);  
        status = K_Task_Start(TASK3_ROM); 
    #if TASK_FRESH_LCD          
        status = K_Task_Start(TASK4_ROM); 
    #endif        
#endif        
	/*
	the following will set up the CMX timer tick. When the interrupt
	happens, the interrupt code will properly call the K_OS_Intrp_Entry
	assembly lanquage routine, call the K_OS_Tick_Update function, then call
	the K_OS_Intrp_Exit assembly routine.
	*/

	// Initialize Timer A

#if 0
        CCTL0 = CCIE;                             // CCR0 interrupt enabled
        CCR0  = 328;
        TACTL = TASSEL_1 + MC_1;                  // SMCLK, upmode
#endif
        InitPowerClockController();
        HMI_Init();
        Alarm_Init();
        Alarm_Off();
        Key_Init();
        EnablePowerOn();
        
        HMI_DisplayAll();
        SystemData_Init();
        SystemData_Read();
        EnableSysLowPowerMode();
	K_OS_Start();			/* enter CMX RTOS */
}

unsigned int flag = 0;
void int_handler(void)
{
	K_OS_Tick_Update();	/* call the CMX tick function */
	K_Event_Signal(0,TASK1_RAM_POS,TSK1_EVENT3);		/* signal an event */
	if (enable_semaphores)
        {
	    K_Semaphore_Post(SEM0);
	}   
#if TASK_FRESH_LCD == 0
        HMI_FreshDisplay();
#endif        
}

