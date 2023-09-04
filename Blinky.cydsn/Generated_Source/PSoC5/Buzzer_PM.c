/*******************************************************************************
* File Name: Buzzer_PM.c
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

#include "Buzzer.h"

static Buzzer_backupStruct Buzzer_backup;


/*******************************************************************************
* Function Name: Buzzer_SaveConfig
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
*  Buzzer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Buzzer_SaveConfig(void) 
{

    #if(!Buzzer_UsingFixedFunction)
        #if(!Buzzer_PWMModeIsCenterAligned)
            Buzzer_backup.PWMPeriod = Buzzer_ReadPeriod();
        #endif /* (!Buzzer_PWMModeIsCenterAligned) */
        Buzzer_backup.PWMUdb = Buzzer_ReadCounter();
        #if (Buzzer_UseStatus)
            Buzzer_backup.InterruptMaskValue = Buzzer_STATUS_MASK;
        #endif /* (Buzzer_UseStatus) */

        #if(Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_256_CLOCKS || \
            Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_2_4_CLOCKS)
            Buzzer_backup.PWMdeadBandValue = Buzzer_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Buzzer_KillModeMinTime)
             Buzzer_backup.PWMKillCounterPeriod = Buzzer_ReadKillTime();
        #endif /* (Buzzer_KillModeMinTime) */

        #if(Buzzer_UseControl)
            Buzzer_backup.PWMControlRegister = Buzzer_ReadControlRegister();
        #endif /* (Buzzer_UseControl) */
    #endif  /* (!Buzzer_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Buzzer_RestoreConfig
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
*  Buzzer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Buzzer_RestoreConfig(void) 
{
        #if(!Buzzer_UsingFixedFunction)
            #if(!Buzzer_PWMModeIsCenterAligned)
                Buzzer_WritePeriod(Buzzer_backup.PWMPeriod);
            #endif /* (!Buzzer_PWMModeIsCenterAligned) */

            Buzzer_WriteCounter(Buzzer_backup.PWMUdb);

            #if (Buzzer_UseStatus)
                Buzzer_STATUS_MASK = Buzzer_backup.InterruptMaskValue;
            #endif /* (Buzzer_UseStatus) */

            #if(Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_256_CLOCKS || \
                Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_2_4_CLOCKS)
                Buzzer_WriteDeadTime(Buzzer_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Buzzer_KillModeMinTime)
                Buzzer_WriteKillTime(Buzzer_backup.PWMKillCounterPeriod);
            #endif /* (Buzzer_KillModeMinTime) */

            #if(Buzzer_UseControl)
                Buzzer_WriteControlRegister(Buzzer_backup.PWMControlRegister);
            #endif /* (Buzzer_UseControl) */
        #endif  /* (!Buzzer_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Buzzer_Sleep
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
*  Buzzer_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Buzzer_Sleep(void) 
{
    #if(Buzzer_UseControl)
        if(Buzzer_CTRL_ENABLE == (Buzzer_CONTROL & Buzzer_CTRL_ENABLE))
        {
            /*Component is enabled */
            Buzzer_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Buzzer_backup.PWMEnableState = 0u;
        }
    #endif /* (Buzzer_UseControl) */

    /* Stop component */
    Buzzer_Stop();

    /* Save registers configuration */
    Buzzer_SaveConfig();
}


/*******************************************************************************
* Function Name: Buzzer_Wakeup
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
*  Buzzer_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Buzzer_Wakeup(void) 
{
     /* Restore registers values */
    Buzzer_RestoreConfig();

    if(Buzzer_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Buzzer_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
