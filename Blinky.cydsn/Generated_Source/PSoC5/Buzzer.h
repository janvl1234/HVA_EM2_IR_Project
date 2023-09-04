/*******************************************************************************
* File Name: Buzzer.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_Buzzer_H)
#define CY_PWM_Buzzer_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Buzzer_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define Buzzer_Resolution                     (16u)
#define Buzzer_UsingFixedFunction             (0u)
#define Buzzer_DeadBandMode                   (0u)
#define Buzzer_KillModeMinTime                (0u)
#define Buzzer_KillMode                       (0u)
#define Buzzer_PWMMode                        (0u)
#define Buzzer_PWMModeIsCenterAligned         (0u)
#define Buzzer_DeadBandUsed                   (0u)
#define Buzzer_DeadBand2_4                    (0u)

#if !defined(Buzzer_PWMUDB_genblk8_stsreg__REMOVED)
    #define Buzzer_UseStatus                  (1u)
#else
    #define Buzzer_UseStatus                  (0u)
#endif /* !defined(Buzzer_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(Buzzer_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define Buzzer_UseControl                 (1u)
#else
    #define Buzzer_UseControl                 (0u)
#endif /* !defined(Buzzer_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define Buzzer_UseOneCompareMode              (1u)
#define Buzzer_MinimumKillTime                (1u)
#define Buzzer_EnableMode                     (0u)

#define Buzzer_CompareMode1SW                 (0u)
#define Buzzer_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define Buzzer__B_PWM__DISABLED 0
#define Buzzer__B_PWM__ASYNCHRONOUS 1
#define Buzzer__B_PWM__SINGLECYCLE 2
#define Buzzer__B_PWM__LATCHED 3
#define Buzzer__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define Buzzer__B_PWM__DBMDISABLED 0
#define Buzzer__B_PWM__DBM_2_4_CLOCKS 1
#define Buzzer__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define Buzzer__B_PWM__ONE_OUTPUT 0
#define Buzzer__B_PWM__TWO_OUTPUTS 1
#define Buzzer__B_PWM__DUAL_EDGE 2
#define Buzzer__B_PWM__CENTER_ALIGN 3
#define Buzzer__B_PWM__DITHER 5
#define Buzzer__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define Buzzer__B_PWM__LESS_THAN 1
#define Buzzer__B_PWM__LESS_THAN_OR_EQUAL 2
#define Buzzer__B_PWM__GREATER_THAN 3
#define Buzzer__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define Buzzer__B_PWM__EQUAL 0
#define Buzzer__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!Buzzer_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!Buzzer_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!Buzzer_PWMModeIsCenterAligned) */
        #if (Buzzer_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (Buzzer_UseStatus) */

        /* Backup for Deadband parameters */
        #if(Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_256_CLOCKS || \
            Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(Buzzer_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (Buzzer_KillModeMinTime) */

        /* Backup control register */
        #if(Buzzer_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (Buzzer_UseControl) */

    #endif /* (!Buzzer_UsingFixedFunction) */

}Buzzer_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    Buzzer_Start(void) ;
void    Buzzer_Stop(void) ;

#if (Buzzer_UseStatus || Buzzer_UsingFixedFunction)
    void  Buzzer_SetInterruptMode(uint8 interruptMode) ;
    uint8 Buzzer_ReadStatusRegister(void) ;
#endif /* (Buzzer_UseStatus || Buzzer_UsingFixedFunction) */

#define Buzzer_GetInterruptSource() Buzzer_ReadStatusRegister()

#if (Buzzer_UseControl)
    uint8 Buzzer_ReadControlRegister(void) ;
    void  Buzzer_WriteControlRegister(uint8 control)
          ;
#endif /* (Buzzer_UseControl) */

#if (Buzzer_UseOneCompareMode)
   #if (Buzzer_CompareMode1SW)
       void    Buzzer_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (Buzzer_CompareMode1SW) */
#else
    #if (Buzzer_CompareMode1SW)
        void    Buzzer_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (Buzzer_CompareMode1SW) */
    #if (Buzzer_CompareMode2SW)
        void    Buzzer_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (Buzzer_CompareMode2SW) */
#endif /* (Buzzer_UseOneCompareMode) */

#if (!Buzzer_UsingFixedFunction)
    uint16   Buzzer_ReadCounter(void) ;
    uint16 Buzzer_ReadCapture(void) ;

    #if (Buzzer_UseStatus)
            void Buzzer_ClearFIFO(void) ;
    #endif /* (Buzzer_UseStatus) */

    void    Buzzer_WriteCounter(uint16 counter)
            ;
#endif /* (!Buzzer_UsingFixedFunction) */

void    Buzzer_WritePeriod(uint16 period)
        ;
uint16 Buzzer_ReadPeriod(void) ;

#if (Buzzer_UseOneCompareMode)
    void    Buzzer_WriteCompare(uint16 compare)
            ;
    uint16 Buzzer_ReadCompare(void) ;
#else
    void    Buzzer_WriteCompare1(uint16 compare)
            ;
    uint16 Buzzer_ReadCompare1(void) ;
    void    Buzzer_WriteCompare2(uint16 compare)
            ;
    uint16 Buzzer_ReadCompare2(void) ;
#endif /* (Buzzer_UseOneCompareMode) */


#if (Buzzer_DeadBandUsed)
    void    Buzzer_WriteDeadTime(uint8 deadtime) ;
    uint8   Buzzer_ReadDeadTime(void) ;
#endif /* (Buzzer_DeadBandUsed) */

#if ( Buzzer_KillModeMinTime)
    void Buzzer_WriteKillTime(uint8 killtime) ;
    uint8 Buzzer_ReadKillTime(void) ;
#endif /* ( Buzzer_KillModeMinTime) */

void Buzzer_Init(void) ;
void Buzzer_Enable(void) ;
void Buzzer_Sleep(void) ;
void Buzzer_Wakeup(void) ;
void Buzzer_SaveConfig(void) ;
void Buzzer_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define Buzzer_INIT_PERIOD_VALUE          (100u)
#define Buzzer_INIT_COMPARE_VALUE1        (10u)
#define Buzzer_INIT_COMPARE_VALUE2        (255u)
#define Buzzer_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    Buzzer_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Buzzer_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Buzzer_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Buzzer_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define Buzzer_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  Buzzer_CTRL_CMPMODE2_SHIFT)
#define Buzzer_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  Buzzer_CTRL_CMPMODE1_SHIFT)
#define Buzzer_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (Buzzer_UsingFixedFunction)
   #define Buzzer_PERIOD_LSB              (*(reg16 *) Buzzer_PWMHW__PER0)
   #define Buzzer_PERIOD_LSB_PTR          ( (reg16 *) Buzzer_PWMHW__PER0)
   #define Buzzer_COMPARE1_LSB            (*(reg16 *) Buzzer_PWMHW__CNT_CMP0)
   #define Buzzer_COMPARE1_LSB_PTR        ( (reg16 *) Buzzer_PWMHW__CNT_CMP0)
   #define Buzzer_COMPARE2_LSB            (0x00u)
   #define Buzzer_COMPARE2_LSB_PTR        (0x00u)
   #define Buzzer_COUNTER_LSB             (*(reg16 *) Buzzer_PWMHW__CNT_CMP0)
   #define Buzzer_COUNTER_LSB_PTR         ( (reg16 *) Buzzer_PWMHW__CNT_CMP0)
   #define Buzzer_CAPTURE_LSB             (*(reg16 *) Buzzer_PWMHW__CAP0)
   #define Buzzer_CAPTURE_LSB_PTR         ( (reg16 *) Buzzer_PWMHW__CAP0)
   #define Buzzer_RT1                     (*(reg8 *)  Buzzer_PWMHW__RT1)
   #define Buzzer_RT1_PTR                 ( (reg8 *)  Buzzer_PWMHW__RT1)

#else
   #if (Buzzer_Resolution == 8u) /* 8bit - PWM */

       #if(Buzzer_PWMModeIsCenterAligned)
           #define Buzzer_PERIOD_LSB      (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define Buzzer_PERIOD_LSB_PTR  ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define Buzzer_PERIOD_LSB      (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define Buzzer_PERIOD_LSB_PTR  ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (Buzzer_PWMModeIsCenterAligned) */

       #define Buzzer_COMPARE1_LSB        (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Buzzer_COMPARE1_LSB_PTR    ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Buzzer_COMPARE2_LSB        (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Buzzer_COMPARE2_LSB_PTR    ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Buzzer_COUNTERCAP_LSB      (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Buzzer_COUNTERCAP_LSB_PTR  ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Buzzer_COUNTER_LSB         (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Buzzer_COUNTER_LSB_PTR     ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Buzzer_CAPTURE_LSB         (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define Buzzer_CAPTURE_LSB_PTR     ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(Buzzer_PWMModeIsCenterAligned)
               #define Buzzer_PERIOD_LSB      (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define Buzzer_PERIOD_LSB_PTR  ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define Buzzer_PERIOD_LSB      (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define Buzzer_PERIOD_LSB_PTR  ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (Buzzer_PWMModeIsCenterAligned) */

            #define Buzzer_COMPARE1_LSB       (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Buzzer_COMPARE1_LSB_PTR   ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Buzzer_COMPARE2_LSB       (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Buzzer_COMPARE2_LSB_PTR   ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Buzzer_COUNTERCAP_LSB     (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Buzzer_COUNTERCAP_LSB_PTR ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Buzzer_COUNTER_LSB        (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Buzzer_COUNTER_LSB_PTR    ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Buzzer_CAPTURE_LSB        (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define Buzzer_CAPTURE_LSB_PTR    ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(Buzzer_PWMModeIsCenterAligned)
               #define Buzzer_PERIOD_LSB      (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define Buzzer_PERIOD_LSB_PTR  ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define Buzzer_PERIOD_LSB      (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define Buzzer_PERIOD_LSB_PTR  ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (Buzzer_PWMModeIsCenterAligned) */

            #define Buzzer_COMPARE1_LSB       (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Buzzer_COMPARE1_LSB_PTR   ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Buzzer_COMPARE2_LSB       (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Buzzer_COMPARE2_LSB_PTR   ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Buzzer_COUNTERCAP_LSB     (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Buzzer_COUNTERCAP_LSB_PTR ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Buzzer_COUNTER_LSB        (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Buzzer_COUNTER_LSB_PTR    ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Buzzer_CAPTURE_LSB        (*(reg16 *) Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define Buzzer_CAPTURE_LSB_PTR    ((reg16 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define Buzzer_AUX_CONTROLDP1          (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define Buzzer_AUX_CONTROLDP1_PTR      ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (Buzzer_Resolution == 8) */

   #define Buzzer_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define Buzzer_AUX_CONTROLDP0          (*(reg8 *)  Buzzer_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define Buzzer_AUX_CONTROLDP0_PTR      ((reg8 *)   Buzzer_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (Buzzer_UsingFixedFunction) */

#if(Buzzer_KillModeMinTime )
    #define Buzzer_KILLMODEMINTIME        (*(reg8 *)  Buzzer_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define Buzzer_KILLMODEMINTIME_PTR    ((reg8 *)   Buzzer_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (Buzzer_KillModeMinTime ) */

#if(Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_256_CLOCKS)
    #define Buzzer_DEADBAND_COUNT         (*(reg8 *)  Buzzer_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Buzzer_DEADBAND_COUNT_PTR     ((reg8 *)   Buzzer_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Buzzer_DEADBAND_LSB_PTR       ((reg8 *)   Buzzer_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define Buzzer_DEADBAND_LSB           (*(reg8 *)  Buzzer_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (Buzzer_UsingFixedFunction)
        #define Buzzer_DEADBAND_COUNT         (*(reg8 *)  Buzzer_PWMHW__CFG0)
        #define Buzzer_DEADBAND_COUNT_PTR     ((reg8 *)   Buzzer_PWMHW__CFG0)
        #define Buzzer_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Buzzer_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define Buzzer_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define Buzzer_DEADBAND_COUNT         (*(reg8 *)  Buzzer_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Buzzer_DEADBAND_COUNT_PTR     ((reg8 *)   Buzzer_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Buzzer_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Buzzer_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define Buzzer_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (Buzzer_UsingFixedFunction) */
#endif /* (Buzzer_DeadBandMode == Buzzer__B_PWM__DBM_256_CLOCKS) */



#if (Buzzer_UsingFixedFunction)
    #define Buzzer_STATUS                 (*(reg8 *) Buzzer_PWMHW__SR0)
    #define Buzzer_STATUS_PTR             ((reg8 *) Buzzer_PWMHW__SR0)
    #define Buzzer_STATUS_MASK            (*(reg8 *) Buzzer_PWMHW__SR0)
    #define Buzzer_STATUS_MASK_PTR        ((reg8 *) Buzzer_PWMHW__SR0)
    #define Buzzer_CONTROL                (*(reg8 *) Buzzer_PWMHW__CFG0)
    #define Buzzer_CONTROL_PTR            ((reg8 *) Buzzer_PWMHW__CFG0)
    #define Buzzer_CONTROL2               (*(reg8 *) Buzzer_PWMHW__CFG1)
    #define Buzzer_CONTROL3               (*(reg8 *) Buzzer_PWMHW__CFG2)
    #define Buzzer_GLOBAL_ENABLE          (*(reg8 *) Buzzer_PWMHW__PM_ACT_CFG)
    #define Buzzer_GLOBAL_ENABLE_PTR      ( (reg8 *) Buzzer_PWMHW__PM_ACT_CFG)
    #define Buzzer_GLOBAL_STBY_ENABLE     (*(reg8 *) Buzzer_PWMHW__PM_STBY_CFG)
    #define Buzzer_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) Buzzer_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define Buzzer_BLOCK_EN_MASK          (Buzzer_PWMHW__PM_ACT_MSK)
    #define Buzzer_BLOCK_STBY_EN_MASK     (Buzzer_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define Buzzer_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define Buzzer_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define Buzzer_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define Buzzer_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define Buzzer_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define Buzzer_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define Buzzer_CTRL_ENABLE            (uint8)((uint8)0x01u << Buzzer_CTRL_ENABLE_SHIFT)
    #define Buzzer_CTRL_RESET             (uint8)((uint8)0x01u << Buzzer_CTRL_RESET_SHIFT)
    #define Buzzer_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Buzzer_CTRL_CMPMODE2_SHIFT)
    #define Buzzer_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Buzzer_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Buzzer_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define Buzzer_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << Buzzer_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define Buzzer_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define Buzzer_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define Buzzer_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define Buzzer_STATUS_TC_INT_EN_MASK_SHIFT            (Buzzer_STATUS_TC_SHIFT - 4u)
    #define Buzzer_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define Buzzer_STATUS_CMP1_INT_EN_MASK_SHIFT          (Buzzer_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define Buzzer_STATUS_TC              (uint8)((uint8)0x01u << Buzzer_STATUS_TC_SHIFT)
    #define Buzzer_STATUS_CMP1            (uint8)((uint8)0x01u << Buzzer_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define Buzzer_STATUS_TC_INT_EN_MASK              (uint8)((uint8)Buzzer_STATUS_TC >> 4u)
    #define Buzzer_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)Buzzer_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define Buzzer_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define Buzzer_RT1_MASK              (uint8)((uint8)0x03u << Buzzer_RT1_SHIFT)
    #define Buzzer_SYNC                  (uint8)((uint8)0x03u << Buzzer_RT1_SHIFT)
    #define Buzzer_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define Buzzer_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << Buzzer_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define Buzzer_SYNCDSI_EN            (uint8)((uint8)0x0Fu << Buzzer_SYNCDSI_SHIFT)


#else
    #define Buzzer_STATUS                (*(reg8 *)   Buzzer_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Buzzer_STATUS_PTR            ((reg8 *)    Buzzer_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Buzzer_STATUS_MASK           (*(reg8 *)   Buzzer_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Buzzer_STATUS_MASK_PTR       ((reg8 *)    Buzzer_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Buzzer_STATUS_AUX_CTRL       (*(reg8 *)   Buzzer_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define Buzzer_CONTROL               (*(reg8 *)   Buzzer_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define Buzzer_CONTROL_PTR           ((reg8 *)    Buzzer_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define Buzzer_CTRL_ENABLE_SHIFT      (0x07u)
    #define Buzzer_CTRL_RESET_SHIFT       (0x06u)
    #define Buzzer_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define Buzzer_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define Buzzer_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define Buzzer_CTRL_ENABLE            (uint8)((uint8)0x01u << Buzzer_CTRL_ENABLE_SHIFT)
    #define Buzzer_CTRL_RESET             (uint8)((uint8)0x01u << Buzzer_CTRL_RESET_SHIFT)
    #define Buzzer_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Buzzer_CTRL_CMPMODE2_SHIFT)
    #define Buzzer_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Buzzer_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define Buzzer_STATUS_KILL_SHIFT          (0x05u)
    #define Buzzer_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define Buzzer_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define Buzzer_STATUS_TC_SHIFT            (0x02u)
    #define Buzzer_STATUS_CMP2_SHIFT          (0x01u)
    #define Buzzer_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Buzzer_STATUS_KILL_INT_EN_MASK_SHIFT          (Buzzer_STATUS_KILL_SHIFT)
    #define Buzzer_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (Buzzer_STATUS_FIFONEMPTY_SHIFT)
    #define Buzzer_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (Buzzer_STATUS_FIFOFULL_SHIFT)
    #define Buzzer_STATUS_TC_INT_EN_MASK_SHIFT            (Buzzer_STATUS_TC_SHIFT)
    #define Buzzer_STATUS_CMP2_INT_EN_MASK_SHIFT          (Buzzer_STATUS_CMP2_SHIFT)
    #define Buzzer_STATUS_CMP1_INT_EN_MASK_SHIFT          (Buzzer_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define Buzzer_STATUS_KILL            (uint8)((uint8)0x00u << Buzzer_STATUS_KILL_SHIFT )
    #define Buzzer_STATUS_FIFOFULL        (uint8)((uint8)0x01u << Buzzer_STATUS_FIFOFULL_SHIFT)
    #define Buzzer_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << Buzzer_STATUS_FIFONEMPTY_SHIFT)
    #define Buzzer_STATUS_TC              (uint8)((uint8)0x01u << Buzzer_STATUS_TC_SHIFT)
    #define Buzzer_STATUS_CMP2            (uint8)((uint8)0x01u << Buzzer_STATUS_CMP2_SHIFT)
    #define Buzzer_STATUS_CMP1            (uint8)((uint8)0x01u << Buzzer_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Buzzer_STATUS_KILL_INT_EN_MASK            (Buzzer_STATUS_KILL)
    #define Buzzer_STATUS_FIFOFULL_INT_EN_MASK        (Buzzer_STATUS_FIFOFULL)
    #define Buzzer_STATUS_FIFONEMPTY_INT_EN_MASK      (Buzzer_STATUS_FIFONEMPTY)
    #define Buzzer_STATUS_TC_INT_EN_MASK              (Buzzer_STATUS_TC)
    #define Buzzer_STATUS_CMP2_INT_EN_MASK            (Buzzer_STATUS_CMP2)
    #define Buzzer_STATUS_CMP1_INT_EN_MASK            (Buzzer_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define Buzzer_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define Buzzer_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define Buzzer_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define Buzzer_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define Buzzer_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* Buzzer_UsingFixedFunction */

#endif  /* CY_PWM_Buzzer_H */


/* [] END OF FILE */
