/*******************************************************************************
* File Name: Buzzer.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Buzzer.h"

/* Error message for removed <resource> through optimization */
#ifdef Buzzer_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Buzzer_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Buzzer_initVar = 0u;


/*******************************************************************************
* Function Name: Buzzer_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Buzzer_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Buzzer_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Buzzer_initVar == 0u)
    {
        Buzzer_Init();
        Buzzer_initVar = 1u;
    }
    Buzzer_Enable();

}


/*******************************************************************************
* Function Name: Buzzer_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Buzzer_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Buzzer_Init(void) 
{
    #if (Buzzer_UsingFixedFunction || Buzzer_UseControl)
        uint8 ctrl;
    #endif /* (Buzzer_UsingFixedFunction || Buzzer_UseControl) */

    #if(!Buzzer_UsingFixedFunction)
        #if(Buzzer_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Buzzer_interruptState;
        #endif /* (Buzzer_UseStatus) */
    #endif /* (!Buzzer_UsingFixedFunction) */

    #if (Buzzer_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Buzzer_CONTROL |= Buzzer_CFG0_MODE;
        #if (Buzzer_DeadBand2_4)
            Buzzer_CONTROL |= Buzzer_CFG0_DB;
        #endif /* (Buzzer_DeadBand2_4) */

        ctrl = Buzzer_CONTROL3 & ((uint8 )(~Buzzer_CTRL_CMPMODE1_MASK));
        Buzzer_CONTROL3 = ctrl | Buzzer_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Buzzer_RT1 &= ((uint8)(~Buzzer_RT1_MASK));
        Buzzer_RT1 |= Buzzer_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Buzzer_RT1 &= ((uint8)(~Buzzer_SYNCDSI_MASK));
        Buzzer_RT1 |= Buzzer_SYNCDSI_EN;

    #elif (Buzzer_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Buzzer_CONTROL & ((uint8)(~Buzzer_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Buzzer_CTRL_CMPMODE1_MASK));
        Buzzer_CONTROL = ctrl | Buzzer_DEFAULT_COMPARE2_MODE |
                                   Buzzer_DEFAULT_COMPARE1_MODE;
    #endif /* (Buzzer_UsingFixedFunction) */

    #if (!Buzzer_UsingFixedFunction)
        #if (Buzzer_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Buzzer_AUX_CONTROLDP0 |= (Buzzer_AUX_CTRL_FIFO0_CLR);
        #else /* (Buzzer_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Buzzer_AUX_CONTROLDP0 |= (Buzzer_AUX_CTRL_FIFO0_CLR);
            Buzzer_AUX_CONTROLDP1 |= (Buzzer_AUX_CTRL_FIFO0_CLR);
        #endif /* (Buzzer_Resolution == 8) */

        Buzzer_WriteCounter(Buzzer_INIT_PERIOD_VALUE);
    #endif /* (!Buzzer_UsingFixedFunction) */

    Buzzer_WritePeriod(Buzzer_INIT_PERIOD_VALUE);

        #if (Buzzer_UseOneCompareMode)
            Buzzer_WriteCompare(Buzzer_INIT_COMPARE_VALUE1);
        #else
            Buzzer_WriteCompare1(Buzzer_INIT_COMPARE_VALUE1);
            Buzzer_WriteCompare2(Buzzer_INIT_COMPARE_VALUE2);
        #endif /* (Buzzer_UseOneCompareMode) */

        #if (Buzzer_KillModeMinTime)
            Buzzer_WriteKillTime(Buzzer_MinimumKillTime);
        #endif /* (Buzzer_KillModeMinTime) */

        #if (Buzzer_DeadBandUsed)
            Buzzer_WriteDeadTime(Buzzer_INIT_DEAD_TIME);
        #endif /* (Buzzer_DeadBandUsed) */

    #if (Buzzer_UseStatus || Buzzer_UsingFixedFunction)
        Buzzer_SetInterruptMode(Buzzer_INIT_INTERRUPTS_MODE);
    #endif /* (Buzzer_UseStatus || Buzzer_UsingFixedFunction) */

    #if (Buzzer_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Buzzer_GLOBAL_ENABLE |= Buzzer_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Buzzer_CONTROL2 |= Buzzer_CTRL2_IRQ_SEL;
    #else
        #if(Buzzer_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Buzzer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Buzzer_STATUS_AUX_CTRL |= Buzzer_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Buzzer_interruptState);

            /* Clear the FIFO to enable the Buzzer_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Buzzer_ClearFIFO();
        #endif /* (Buzzer_UseStatus) */
    #endif /* (Buzzer_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Buzzer_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Buzzer_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Buzzer_UsingFixedFunction)
        Buzzer_GLOBAL_ENABLE |= Buzzer_BLOCK_EN_MASK;
        Buzzer_GLOBAL_STBY_ENABLE |= Buzzer_BLOCK_STBY_EN_MASK;
    #endif /* (Buzzer_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Buzzer_UseControl || Buzzer_UsingFixedFunction)
        Buzzer_CONTROL |= Buzzer_CTRL_ENABLE;
    #endif /* (Buzzer_UseControl || Buzzer_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Buzzer_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Buzzer_Stop(void) 
{
    #if (Buzzer_UseControl || Buzzer_UsingFixedFunction)
        Buzzer_CONTROL &= ((uint8)(~Buzzer_CTRL_ENABLE));
    #endif /* (Buzzer_UseControl || Buzzer_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Buzzer_UsingFixedFunction)
        Buzzer_GLOBAL_ENABLE &= ((uint8)(~Buzzer_BLOCK_EN_MASK));
        Buzzer_GLOBAL_STBY_ENABLE &= ((uint8)(~Buzzer_BLOCK_STBY_EN_MASK));
    #endif /* (Buzzer_UsingFixedFunction) */
}

#if (Buzzer_UseOneCompareMode)
    #if (Buzzer_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Buzzer_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Buzzer_SetCompareMode(uint8 comparemode) 
        {
            #if(Buzzer_UsingFixedFunction)

                #if(0 != Buzzer_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Buzzer_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Buzzer_CTRL_CMPMODE1_SHIFT) */

                Buzzer_CONTROL3 &= ((uint8)(~Buzzer_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Buzzer_CONTROL3 |= comparemodemasked;

            #elif (Buzzer_UseControl)

                #if(0 != Buzzer_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Buzzer_CTRL_CMPMODE1_SHIFT)) &
                                                Buzzer_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Buzzer_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Buzzer_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Buzzer_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Buzzer_CTRL_CMPMODE2_SHIFT)) &
                                               Buzzer_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Buzzer_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Buzzer_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Buzzer_CONTROL &= ((uint8)(~(Buzzer_CTRL_CMPMODE1_MASK |
                                            Buzzer_CTRL_CMPMODE2_MASK)));
                Buzzer_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Buzzer_UsingFixedFunction) */
        }
    #endif /* Buzzer_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Buzzer_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Buzzer_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Buzzer_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Buzzer_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Buzzer_CTRL_CMPMODE1_SHIFT)) &
                                           Buzzer_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Buzzer_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Buzzer_CTRL_CMPMODE1_SHIFT) */

            #if (Buzzer_UseControl)
                Buzzer_CONTROL &= ((uint8)(~Buzzer_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Buzzer_CONTROL |= comparemodemasked;
            #endif /* (Buzzer_UseControl) */
        }
    #endif /* Buzzer_CompareMode1SW */

#if (Buzzer_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Buzzer_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Buzzer_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Buzzer_CTRL_CMPMODE2_SHIFT)) &
                                                 Buzzer_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Buzzer_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Buzzer_CTRL_CMPMODE2_SHIFT) */

        #if (Buzzer_UseControl)
            Buzzer_CONTROL &= ((uint8)(~Buzzer_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Buzzer_CONTROL |= comparemodemasked;
        #endif /* (Buzzer_UseControl) */
    }
    #endif /*Buzzer_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Buzzer_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Buzzer_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void Buzzer_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(Buzzer_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Buzzer_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 Buzzer_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Buzzer_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(Buzzer_CAPTURE_LSB_PTR));
    }

    #if (Buzzer_UseStatus)


        /*******************************************************************************
        * Function Name: Buzzer_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Buzzer_ClearFIFO(void) 
        {
            while(0u != (Buzzer_ReadStatusRegister() & Buzzer_STATUS_FIFONEMPTY))
            {
                (void)Buzzer_ReadCapture();
            }
        }

    #endif /* Buzzer_UseStatus */

#endif /* !Buzzer_UsingFixedFunction */


/*******************************************************************************
* Function Name: Buzzer_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void Buzzer_WritePeriod(uint16 period) 
{
    #if(Buzzer_UsingFixedFunction)
        CY_SET_REG16(Buzzer_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(Buzzer_PERIOD_LSB_PTR, period);
    #endif /* (Buzzer_UsingFixedFunction) */
}

#if (Buzzer_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Buzzer_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Buzzer_WriteCompare(uint16 compare) \
                                       
    {
        #if(Buzzer_UsingFixedFunction)
            CY_SET_REG16(Buzzer_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Buzzer_COMPARE1_LSB_PTR, compare);
        #endif /* (Buzzer_UsingFixedFunction) */

        #if (Buzzer_PWMMode == Buzzer__B_PWM__DITHER)
            #if(Buzzer_UsingFixedFunction)
                CY_SET_REG16(Buzzer_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(Buzzer_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Buzzer_UsingFixedFunction) */
        #endif /* (Buzzer_PWMMode == Buzzer__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Buzzer_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_WriteCompare1(uint16 compare) \
                                        
    {
        #if(Buzzer_UsingFixedFunction)
            CY_SET_REG16(Buzzer_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Buzzer_COMPARE1_LSB_PTR, compare);
        #endif /* (Buzzer_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Buzzer_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_WriteCompare2(uint16 compare) \
                                        
    {
        #if(Buzzer_UsingFixedFunction)
            CY_SET_REG16(Buzzer_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(Buzzer_COMPARE2_LSB_PTR, compare);
        #endif /* (Buzzer_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Buzzer_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Buzzer_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Buzzer_DeadBand2_4)
            CY_SET_REG8(Buzzer_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Buzzer_DEADBAND_COUNT &= ((uint8)(~Buzzer_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Buzzer_DEADBAND_COUNT_SHIFT)
                Buzzer_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Buzzer_DEADBAND_COUNT_SHIFT)) &
                                                    Buzzer_DEADBAND_COUNT_MASK;
            #else
                Buzzer_DEADBAND_COUNT |= deadtime & Buzzer_DEADBAND_COUNT_MASK;
            #endif /* (Buzzer_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Buzzer_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Buzzer_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Buzzer_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Buzzer_DeadBand2_4)
            return (CY_GET_REG8(Buzzer_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Buzzer_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Buzzer_DEADBAND_COUNT & Buzzer_DEADBAND_COUNT_MASK)) >>
                                                                           Buzzer_DEADBAND_COUNT_SHIFT));
            #else
                return (Buzzer_DEADBAND_COUNT & Buzzer_DEADBAND_COUNT_MASK);
            #endif /* (Buzzer_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Buzzer_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Buzzer_UseStatus || Buzzer_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Buzzer_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Buzzer_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Buzzer_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Buzzer_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Buzzer_STATUS_PTR));
    }

#endif /* (Buzzer_UseStatus || Buzzer_UsingFixedFunction) */


#if (Buzzer_UseControl)


    /*******************************************************************************
    * Function Name: Buzzer_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 Buzzer_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Buzzer_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Buzzer_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Buzzer_CONTROL_PTR, control);
    }

#endif /* (Buzzer_UseControl) */


#if (!Buzzer_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Buzzer_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 Buzzer_ReadCapture(void) 
    {
        return (CY_GET_REG16(Buzzer_CAPTURE_LSB_PTR));
    }

#endif /* (!Buzzer_UsingFixedFunction) */


#if (Buzzer_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Buzzer_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 Buzzer_ReadCompare(void) 
    {
        #if(Buzzer_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(Buzzer_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(Buzzer_COMPARE1_LSB_PTR));
        #endif /* (Buzzer_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Buzzer_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Buzzer_ReadCompare1(void) 
    {
        return (CY_GET_REG16(Buzzer_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Buzzer_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Buzzer_ReadCompare2(void) 
    {
        return (CY_GET_REG16(Buzzer_COMPARE2_LSB_PTR));
    }

#endif /* (Buzzer_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Buzzer_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 Buzzer_ReadPeriod(void) 
{
    #if(Buzzer_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Buzzer_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Buzzer_PERIOD_LSB_PTR));
    #endif /* (Buzzer_UsingFixedFunction) */
}

#if ( Buzzer_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Buzzer_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Buzzer_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Buzzer_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Buzzer_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 Buzzer_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Buzzer_KILLMODEMINTIME_PTR));
    }

#endif /* ( Buzzer_KillModeMinTime) */

/* [] END OF FILE */
