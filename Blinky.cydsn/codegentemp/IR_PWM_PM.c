/*******************************************************************************
* File Name: IR_PWM_PM.c
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

#include "IR_PWM.h"

static IR_PWM_backupStruct IR_PWM_backup;


/*******************************************************************************
* Function Name: IR_PWM_SaveConfig
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
*  IR_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void IR_PWM_SaveConfig(void) 
{

    #if(!IR_PWM_UsingFixedFunction)
        #if(!IR_PWM_PWMModeIsCenterAligned)
            IR_PWM_backup.PWMPeriod = IR_PWM_ReadPeriod();
        #endif /* (!IR_PWM_PWMModeIsCenterAligned) */
        IR_PWM_backup.PWMUdb = IR_PWM_ReadCounter();
        #if (IR_PWM_UseStatus)
            IR_PWM_backup.InterruptMaskValue = IR_PWM_STATUS_MASK;
        #endif /* (IR_PWM_UseStatus) */

        #if(IR_PWM_DeadBandMode == IR_PWM__B_PWM__DBM_256_CLOCKS || \
            IR_PWM_DeadBandMode == IR_PWM__B_PWM__DBM_2_4_CLOCKS)
            IR_PWM_backup.PWMdeadBandValue = IR_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(IR_PWM_KillModeMinTime)
             IR_PWM_backup.PWMKillCounterPeriod = IR_PWM_ReadKillTime();
        #endif /* (IR_PWM_KillModeMinTime) */

        #if(IR_PWM_UseControl)
            IR_PWM_backup.PWMControlRegister = IR_PWM_ReadControlRegister();
        #endif /* (IR_PWM_UseControl) */
    #endif  /* (!IR_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: IR_PWM_RestoreConfig
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
*  IR_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void IR_PWM_RestoreConfig(void) 
{
        #if(!IR_PWM_UsingFixedFunction)
            #if(!IR_PWM_PWMModeIsCenterAligned)
                IR_PWM_WritePeriod(IR_PWM_backup.PWMPeriod);
            #endif /* (!IR_PWM_PWMModeIsCenterAligned) */

            IR_PWM_WriteCounter(IR_PWM_backup.PWMUdb);

            #if (IR_PWM_UseStatus)
                IR_PWM_STATUS_MASK = IR_PWM_backup.InterruptMaskValue;
            #endif /* (IR_PWM_UseStatus) */

            #if(IR_PWM_DeadBandMode == IR_PWM__B_PWM__DBM_256_CLOCKS || \
                IR_PWM_DeadBandMode == IR_PWM__B_PWM__DBM_2_4_CLOCKS)
                IR_PWM_WriteDeadTime(IR_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(IR_PWM_KillModeMinTime)
                IR_PWM_WriteKillTime(IR_PWM_backup.PWMKillCounterPeriod);
            #endif /* (IR_PWM_KillModeMinTime) */

            #if(IR_PWM_UseControl)
                IR_PWM_WriteControlRegister(IR_PWM_backup.PWMControlRegister);
            #endif /* (IR_PWM_UseControl) */
        #endif  /* (!IR_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: IR_PWM_Sleep
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
*  IR_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void IR_PWM_Sleep(void) 
{
    #if(IR_PWM_UseControl)
        if(IR_PWM_CTRL_ENABLE == (IR_PWM_CONTROL & IR_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            IR_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            IR_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (IR_PWM_UseControl) */

    /* Stop component */
    IR_PWM_Stop();

    /* Save registers configuration */
    IR_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: IR_PWM_Wakeup
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
*  IR_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void IR_PWM_Wakeup(void) 
{
     /* Restore registers values */
    IR_PWM_RestoreConfig();

    if(IR_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        IR_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
