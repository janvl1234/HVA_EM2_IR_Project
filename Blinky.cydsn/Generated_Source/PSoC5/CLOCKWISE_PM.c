/*******************************************************************************
* File Name: CLOCKWISE_PM.c
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

#include "CLOCKWISE.h"

static CLOCKWISE_backupStruct CLOCKWISE_backup;


/*******************************************************************************
* Function Name: CLOCKWISE_SaveConfig
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
*  CLOCKWISE_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void CLOCKWISE_SaveConfig(void) 
{

    #if(!CLOCKWISE_UsingFixedFunction)
        #if(!CLOCKWISE_PWMModeIsCenterAligned)
            CLOCKWISE_backup.PWMPeriod = CLOCKWISE_ReadPeriod();
        #endif /* (!CLOCKWISE_PWMModeIsCenterAligned) */
        CLOCKWISE_backup.PWMUdb = CLOCKWISE_ReadCounter();
        #if (CLOCKWISE_UseStatus)
            CLOCKWISE_backup.InterruptMaskValue = CLOCKWISE_STATUS_MASK;
        #endif /* (CLOCKWISE_UseStatus) */

        #if(CLOCKWISE_DeadBandMode == CLOCKWISE__B_PWM__DBM_256_CLOCKS || \
            CLOCKWISE_DeadBandMode == CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
            CLOCKWISE_backup.PWMdeadBandValue = CLOCKWISE_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(CLOCKWISE_KillModeMinTime)
             CLOCKWISE_backup.PWMKillCounterPeriod = CLOCKWISE_ReadKillTime();
        #endif /* (CLOCKWISE_KillModeMinTime) */

        #if(CLOCKWISE_UseControl)
            CLOCKWISE_backup.PWMControlRegister = CLOCKWISE_ReadControlRegister();
        #endif /* (CLOCKWISE_UseControl) */
    #endif  /* (!CLOCKWISE_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CLOCKWISE_RestoreConfig
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
*  CLOCKWISE_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CLOCKWISE_RestoreConfig(void) 
{
        #if(!CLOCKWISE_UsingFixedFunction)
            #if(!CLOCKWISE_PWMModeIsCenterAligned)
                CLOCKWISE_WritePeriod(CLOCKWISE_backup.PWMPeriod);
            #endif /* (!CLOCKWISE_PWMModeIsCenterAligned) */

            CLOCKWISE_WriteCounter(CLOCKWISE_backup.PWMUdb);

            #if (CLOCKWISE_UseStatus)
                CLOCKWISE_STATUS_MASK = CLOCKWISE_backup.InterruptMaskValue;
            #endif /* (CLOCKWISE_UseStatus) */

            #if(CLOCKWISE_DeadBandMode == CLOCKWISE__B_PWM__DBM_256_CLOCKS || \
                CLOCKWISE_DeadBandMode == CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
                CLOCKWISE_WriteDeadTime(CLOCKWISE_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(CLOCKWISE_KillModeMinTime)
                CLOCKWISE_WriteKillTime(CLOCKWISE_backup.PWMKillCounterPeriod);
            #endif /* (CLOCKWISE_KillModeMinTime) */

            #if(CLOCKWISE_UseControl)
                CLOCKWISE_WriteControlRegister(CLOCKWISE_backup.PWMControlRegister);
            #endif /* (CLOCKWISE_UseControl) */
        #endif  /* (!CLOCKWISE_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: CLOCKWISE_Sleep
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
*  CLOCKWISE_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void CLOCKWISE_Sleep(void) 
{
    #if(CLOCKWISE_UseControl)
        if(CLOCKWISE_CTRL_ENABLE == (CLOCKWISE_CONTROL & CLOCKWISE_CTRL_ENABLE))
        {
            /*Component is enabled */
            CLOCKWISE_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            CLOCKWISE_backup.PWMEnableState = 0u;
        }
    #endif /* (CLOCKWISE_UseControl) */

    /* Stop component */
    CLOCKWISE_Stop();

    /* Save registers configuration */
    CLOCKWISE_SaveConfig();
}


/*******************************************************************************
* Function Name: CLOCKWISE_Wakeup
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
*  CLOCKWISE_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CLOCKWISE_Wakeup(void) 
{
     /* Restore registers values */
    CLOCKWISE_RestoreConfig();

    if(CLOCKWISE_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        CLOCKWISE_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
