/*******************************************************************************
* File Name: COUNTER_CLOCKWISE_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "COUNTER_CLOCKWISE.h"

static COUNTER_CLOCKWISE_backupStruct COUNTER_CLOCKWISE_backup;


/*******************************************************************************
* Function Name: COUNTER_CLOCKWISE_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  COUNTER_CLOCKWISE_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void COUNTER_CLOCKWISE_SaveConfig(void) 
{

    #if(!COUNTER_CLOCKWISE_UsingFixedFunction)
        #if(!COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
            COUNTER_CLOCKWISE_backup.PWMPeriod = COUNTER_CLOCKWISE_ReadPeriod();
        #endif /* (!COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */
        COUNTER_CLOCKWISE_backup.PWMUdb = COUNTER_CLOCKWISE_ReadCounter();
        #if (COUNTER_CLOCKWISE_UseStatus)
            COUNTER_CLOCKWISE_backup.InterruptMaskValue = COUNTER_CLOCKWISE_STATUS_MASK;
        #endif /* (COUNTER_CLOCKWISE_UseStatus) */

        #if(COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS || \
            COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
            COUNTER_CLOCKWISE_backup.PWMdeadBandValue = COUNTER_CLOCKWISE_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(COUNTER_CLOCKWISE_KillModeMinTime)
             COUNTER_CLOCKWISE_backup.PWMKillCounterPeriod = COUNTER_CLOCKWISE_ReadKillTime();
        #endif /* (COUNTER_CLOCKWISE_KillModeMinTime) */

        #if(COUNTER_CLOCKWISE_UseControl)
            COUNTER_CLOCKWISE_backup.PWMControlRegister = COUNTER_CLOCKWISE_ReadControlRegister();
        #endif /* (COUNTER_CLOCKWISE_UseControl) */
    #endif  /* (!COUNTER_CLOCKWISE_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_CLOCKWISE_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  COUNTER_CLOCKWISE_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_CLOCKWISE_RestoreConfig(void) 
{
        #if(!COUNTER_CLOCKWISE_UsingFixedFunction)
            #if(!COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
                COUNTER_CLOCKWISE_WritePeriod(COUNTER_CLOCKWISE_backup.PWMPeriod);
            #endif /* (!COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */

            COUNTER_CLOCKWISE_WriteCounter(COUNTER_CLOCKWISE_backup.PWMUdb);

            #if (COUNTER_CLOCKWISE_UseStatus)
                COUNTER_CLOCKWISE_STATUS_MASK = COUNTER_CLOCKWISE_backup.InterruptMaskValue;
            #endif /* (COUNTER_CLOCKWISE_UseStatus) */

            #if(COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS || \
                COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
                COUNTER_CLOCKWISE_WriteDeadTime(COUNTER_CLOCKWISE_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(COUNTER_CLOCKWISE_KillModeMinTime)
                COUNTER_CLOCKWISE_WriteKillTime(COUNTER_CLOCKWISE_backup.PWMKillCounterPeriod);
            #endif /* (COUNTER_CLOCKWISE_KillModeMinTime) */

            #if(COUNTER_CLOCKWISE_UseControl)
                COUNTER_CLOCKWISE_WriteControlRegister(COUNTER_CLOCKWISE_backup.PWMControlRegister);
            #endif /* (COUNTER_CLOCKWISE_UseControl) */
        #endif  /* (!COUNTER_CLOCKWISE_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: COUNTER_CLOCKWISE_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  COUNTER_CLOCKWISE_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void COUNTER_CLOCKWISE_Sleep(void) 
{
    #if(COUNTER_CLOCKWISE_UseControl)
        if(COUNTER_CLOCKWISE_CTRL_ENABLE == (COUNTER_CLOCKWISE_CONTROL & COUNTER_CLOCKWISE_CTRL_ENABLE))
        {
            /*Component is enabled */
            COUNTER_CLOCKWISE_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            COUNTER_CLOCKWISE_backup.PWMEnableState = 0u;
        }
    #endif /* (COUNTER_CLOCKWISE_UseControl) */

    /* Stop component */
    COUNTER_CLOCKWISE_Stop();

    /* Save registers configuration */
    COUNTER_CLOCKWISE_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER_CLOCKWISE_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  COUNTER_CLOCKWISE_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_CLOCKWISE_Wakeup(void) 
{
     /* Restore registers values */
    COUNTER_CLOCKWISE_RestoreConfig();

    if(COUNTER_CLOCKWISE_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        COUNTER_CLOCKWISE_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
