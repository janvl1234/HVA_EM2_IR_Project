/*******************************************************************************
* File Name: COUNTER_CLOCKWISE.h
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

#if !defined(CY_PWM_COUNTER_CLOCKWISE_H)
#define CY_PWM_COUNTER_CLOCKWISE_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 COUNTER_CLOCKWISE_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define COUNTER_CLOCKWISE_Resolution                     (16u)
#define COUNTER_CLOCKWISE_UsingFixedFunction             (0u)
#define COUNTER_CLOCKWISE_DeadBandMode                   (0u)
#define COUNTER_CLOCKWISE_KillModeMinTime                (0u)
#define COUNTER_CLOCKWISE_KillMode                       (0u)
#define COUNTER_CLOCKWISE_PWMMode                        (0u)
#define COUNTER_CLOCKWISE_PWMModeIsCenterAligned         (0u)
#define COUNTER_CLOCKWISE_DeadBandUsed                   (0u)
#define COUNTER_CLOCKWISE_DeadBand2_4                    (0u)

#if !defined(COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__REMOVED)
    #define COUNTER_CLOCKWISE_UseStatus                  (1u)
#else
    #define COUNTER_CLOCKWISE_UseStatus                  (0u)
#endif /* !defined(COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(COUNTER_CLOCKWISE_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define COUNTER_CLOCKWISE_UseControl                 (1u)
#else
    #define COUNTER_CLOCKWISE_UseControl                 (0u)
#endif /* !defined(COUNTER_CLOCKWISE_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define COUNTER_CLOCKWISE_UseOneCompareMode              (1u)
#define COUNTER_CLOCKWISE_MinimumKillTime                (1u)
#define COUNTER_CLOCKWISE_EnableMode                     (0u)

#define COUNTER_CLOCKWISE_CompareMode1SW                 (0u)
#define COUNTER_CLOCKWISE_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define COUNTER_CLOCKWISE__B_PWM__DISABLED 0
#define COUNTER_CLOCKWISE__B_PWM__ASYNCHRONOUS 1
#define COUNTER_CLOCKWISE__B_PWM__SINGLECYCLE 2
#define COUNTER_CLOCKWISE__B_PWM__LATCHED 3
#define COUNTER_CLOCKWISE__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define COUNTER_CLOCKWISE__B_PWM__DBMDISABLED 0
#define COUNTER_CLOCKWISE__B_PWM__DBM_2_4_CLOCKS 1
#define COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define COUNTER_CLOCKWISE__B_PWM__ONE_OUTPUT 0
#define COUNTER_CLOCKWISE__B_PWM__TWO_OUTPUTS 1
#define COUNTER_CLOCKWISE__B_PWM__DUAL_EDGE 2
#define COUNTER_CLOCKWISE__B_PWM__CENTER_ALIGN 3
#define COUNTER_CLOCKWISE__B_PWM__DITHER 5
#define COUNTER_CLOCKWISE__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define COUNTER_CLOCKWISE__B_PWM__LESS_THAN 1
#define COUNTER_CLOCKWISE__B_PWM__LESS_THAN_OR_EQUAL 2
#define COUNTER_CLOCKWISE__B_PWM__GREATER_THAN 3
#define COUNTER_CLOCKWISE__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define COUNTER_CLOCKWISE__B_PWM__EQUAL 0
#define COUNTER_CLOCKWISE__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!COUNTER_CLOCKWISE_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */
        #if (COUNTER_CLOCKWISE_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (COUNTER_CLOCKWISE_UseStatus) */

        /* Backup for Deadband parameters */
        #if(COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS || \
            COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(COUNTER_CLOCKWISE_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (COUNTER_CLOCKWISE_KillModeMinTime) */

        /* Backup control register */
        #if(COUNTER_CLOCKWISE_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (COUNTER_CLOCKWISE_UseControl) */

    #endif /* (!COUNTER_CLOCKWISE_UsingFixedFunction) */

}COUNTER_CLOCKWISE_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    COUNTER_CLOCKWISE_Start(void) ;
void    COUNTER_CLOCKWISE_Stop(void) ;

#if (COUNTER_CLOCKWISE_UseStatus || COUNTER_CLOCKWISE_UsingFixedFunction)
    void  COUNTER_CLOCKWISE_SetInterruptMode(uint8 interruptMode) ;
    uint8 COUNTER_CLOCKWISE_ReadStatusRegister(void) ;
#endif /* (COUNTER_CLOCKWISE_UseStatus || COUNTER_CLOCKWISE_UsingFixedFunction) */

#define COUNTER_CLOCKWISE_GetInterruptSource() COUNTER_CLOCKWISE_ReadStatusRegister()

#if (COUNTER_CLOCKWISE_UseControl)
    uint8 COUNTER_CLOCKWISE_ReadControlRegister(void) ;
    void  COUNTER_CLOCKWISE_WriteControlRegister(uint8 control)
          ;
#endif /* (COUNTER_CLOCKWISE_UseControl) */

#if (COUNTER_CLOCKWISE_UseOneCompareMode)
   #if (COUNTER_CLOCKWISE_CompareMode1SW)
       void    COUNTER_CLOCKWISE_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (COUNTER_CLOCKWISE_CompareMode1SW) */
#else
    #if (COUNTER_CLOCKWISE_CompareMode1SW)
        void    COUNTER_CLOCKWISE_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (COUNTER_CLOCKWISE_CompareMode1SW) */
    #if (COUNTER_CLOCKWISE_CompareMode2SW)
        void    COUNTER_CLOCKWISE_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (COUNTER_CLOCKWISE_CompareMode2SW) */
#endif /* (COUNTER_CLOCKWISE_UseOneCompareMode) */

#if (!COUNTER_CLOCKWISE_UsingFixedFunction)
    uint16   COUNTER_CLOCKWISE_ReadCounter(void) ;
    uint16 COUNTER_CLOCKWISE_ReadCapture(void) ;

    #if (COUNTER_CLOCKWISE_UseStatus)
            void COUNTER_CLOCKWISE_ClearFIFO(void) ;
    #endif /* (COUNTER_CLOCKWISE_UseStatus) */

    void    COUNTER_CLOCKWISE_WriteCounter(uint16 counter)
            ;
#endif /* (!COUNTER_CLOCKWISE_UsingFixedFunction) */

void    COUNTER_CLOCKWISE_WritePeriod(uint16 period)
        ;
uint16 COUNTER_CLOCKWISE_ReadPeriod(void) ;

#if (COUNTER_CLOCKWISE_UseOneCompareMode)
    void    COUNTER_CLOCKWISE_WriteCompare(uint16 compare)
            ;
    uint16 COUNTER_CLOCKWISE_ReadCompare(void) ;
#else
    void    COUNTER_CLOCKWISE_WriteCompare1(uint16 compare)
            ;
    uint16 COUNTER_CLOCKWISE_ReadCompare1(void) ;
    void    COUNTER_CLOCKWISE_WriteCompare2(uint16 compare)
            ;
    uint16 COUNTER_CLOCKWISE_ReadCompare2(void) ;
#endif /* (COUNTER_CLOCKWISE_UseOneCompareMode) */


#if (COUNTER_CLOCKWISE_DeadBandUsed)
    void    COUNTER_CLOCKWISE_WriteDeadTime(uint8 deadtime) ;
    uint8   COUNTER_CLOCKWISE_ReadDeadTime(void) ;
#endif /* (COUNTER_CLOCKWISE_DeadBandUsed) */

#if ( COUNTER_CLOCKWISE_KillModeMinTime)
    void COUNTER_CLOCKWISE_WriteKillTime(uint8 killtime) ;
    uint8 COUNTER_CLOCKWISE_ReadKillTime(void) ;
#endif /* ( COUNTER_CLOCKWISE_KillModeMinTime) */

void COUNTER_CLOCKWISE_Init(void) ;
void COUNTER_CLOCKWISE_Enable(void) ;
void COUNTER_CLOCKWISE_Sleep(void) ;
void COUNTER_CLOCKWISE_Wakeup(void) ;
void COUNTER_CLOCKWISE_SaveConfig(void) ;
void COUNTER_CLOCKWISE_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define COUNTER_CLOCKWISE_INIT_PERIOD_VALUE          (10000u)
#define COUNTER_CLOCKWISE_INIT_COMPARE_VALUE1        (100u)
#define COUNTER_CLOCKWISE_INIT_COMPARE_VALUE2        (255u)
#define COUNTER_CLOCKWISE_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    COUNTER_CLOCKWISE_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    COUNTER_CLOCKWISE_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    COUNTER_CLOCKWISE_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    COUNTER_CLOCKWISE_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define COUNTER_CLOCKWISE_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  COUNTER_CLOCKWISE_CTRL_CMPMODE2_SHIFT)
#define COUNTER_CLOCKWISE_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  COUNTER_CLOCKWISE_CTRL_CMPMODE1_SHIFT)
#define COUNTER_CLOCKWISE_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (COUNTER_CLOCKWISE_UsingFixedFunction)
   #define COUNTER_CLOCKWISE_PERIOD_LSB              (*(reg16 *) COUNTER_CLOCKWISE_PWMHW__PER0)
   #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR          ( (reg16 *) COUNTER_CLOCKWISE_PWMHW__PER0)
   #define COUNTER_CLOCKWISE_COMPARE1_LSB            (*(reg16 *) COUNTER_CLOCKWISE_PWMHW__CNT_CMP0)
   #define COUNTER_CLOCKWISE_COMPARE1_LSB_PTR        ( (reg16 *) COUNTER_CLOCKWISE_PWMHW__CNT_CMP0)
   #define COUNTER_CLOCKWISE_COMPARE2_LSB            (0x00u)
   #define COUNTER_CLOCKWISE_COMPARE2_LSB_PTR        (0x00u)
   #define COUNTER_CLOCKWISE_COUNTER_LSB             (*(reg16 *) COUNTER_CLOCKWISE_PWMHW__CNT_CMP0)
   #define COUNTER_CLOCKWISE_COUNTER_LSB_PTR         ( (reg16 *) COUNTER_CLOCKWISE_PWMHW__CNT_CMP0)
   #define COUNTER_CLOCKWISE_CAPTURE_LSB             (*(reg16 *) COUNTER_CLOCKWISE_PWMHW__CAP0)
   #define COUNTER_CLOCKWISE_CAPTURE_LSB_PTR         ( (reg16 *) COUNTER_CLOCKWISE_PWMHW__CAP0)
   #define COUNTER_CLOCKWISE_RT1                     (*(reg8 *)  COUNTER_CLOCKWISE_PWMHW__RT1)
   #define COUNTER_CLOCKWISE_RT1_PTR                 ( (reg8 *)  COUNTER_CLOCKWISE_PWMHW__RT1)

#else
   #if (COUNTER_CLOCKWISE_Resolution == 8u) /* 8bit - PWM */

       #if(COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
           #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */

       #define COUNTER_CLOCKWISE_COMPARE1_LSB        (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define COUNTER_CLOCKWISE_COMPARE1_LSB_PTR    ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define COUNTER_CLOCKWISE_COMPARE2_LSB        (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define COUNTER_CLOCKWISE_COMPARE2_LSB_PTR    ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define COUNTER_CLOCKWISE_COUNTERCAP_LSB      (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define COUNTER_CLOCKWISE_COUNTERCAP_LSB_PTR  ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define COUNTER_CLOCKWISE_COUNTER_LSB         (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define COUNTER_CLOCKWISE_COUNTER_LSB_PTR     ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define COUNTER_CLOCKWISE_CAPTURE_LSB         (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define COUNTER_CLOCKWISE_CAPTURE_LSB_PTR     ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
               #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */

            #define COUNTER_CLOCKWISE_COMPARE1_LSB       (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define COUNTER_CLOCKWISE_COMPARE1_LSB_PTR   ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define COUNTER_CLOCKWISE_COMPARE2_LSB       (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define COUNTER_CLOCKWISE_COMPARE2_LSB_PTR   ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define COUNTER_CLOCKWISE_COUNTERCAP_LSB     (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define COUNTER_CLOCKWISE_COUNTERCAP_LSB_PTR ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define COUNTER_CLOCKWISE_COUNTER_LSB        (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define COUNTER_CLOCKWISE_COUNTER_LSB_PTR    ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define COUNTER_CLOCKWISE_CAPTURE_LSB        (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define COUNTER_CLOCKWISE_CAPTURE_LSB_PTR    ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(COUNTER_CLOCKWISE_PWMModeIsCenterAligned)
               #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define COUNTER_CLOCKWISE_PERIOD_LSB      (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define COUNTER_CLOCKWISE_PERIOD_LSB_PTR  ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (COUNTER_CLOCKWISE_PWMModeIsCenterAligned) */

            #define COUNTER_CLOCKWISE_COMPARE1_LSB       (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define COUNTER_CLOCKWISE_COMPARE1_LSB_PTR   ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define COUNTER_CLOCKWISE_COMPARE2_LSB       (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define COUNTER_CLOCKWISE_COMPARE2_LSB_PTR   ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define COUNTER_CLOCKWISE_COUNTERCAP_LSB     (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define COUNTER_CLOCKWISE_COUNTERCAP_LSB_PTR ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define COUNTER_CLOCKWISE_COUNTER_LSB        (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define COUNTER_CLOCKWISE_COUNTER_LSB_PTR    ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define COUNTER_CLOCKWISE_CAPTURE_LSB        (*(reg16 *) COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define COUNTER_CLOCKWISE_CAPTURE_LSB_PTR    ((reg16 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define COUNTER_CLOCKWISE_AUX_CONTROLDP1          (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define COUNTER_CLOCKWISE_AUX_CONTROLDP1_PTR      ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (COUNTER_CLOCKWISE_Resolution == 8) */

   #define COUNTER_CLOCKWISE_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define COUNTER_CLOCKWISE_AUX_CONTROLDP0          (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define COUNTER_CLOCKWISE_AUX_CONTROLDP0_PTR      ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (COUNTER_CLOCKWISE_UsingFixedFunction) */

#if(COUNTER_CLOCKWISE_KillModeMinTime )
    #define COUNTER_CLOCKWISE_KILLMODEMINTIME        (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define COUNTER_CLOCKWISE_KILLMODEMINTIME_PTR    ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (COUNTER_CLOCKWISE_KillModeMinTime ) */

#if(COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS)
    #define COUNTER_CLOCKWISE_DEADBAND_COUNT         (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define COUNTER_CLOCKWISE_DEADBAND_COUNT_PTR     ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define COUNTER_CLOCKWISE_DEADBAND_LSB_PTR       ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define COUNTER_CLOCKWISE_DEADBAND_LSB           (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (COUNTER_CLOCKWISE_UsingFixedFunction)
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT         (*(reg8 *)  COUNTER_CLOCKWISE_PWMHW__CFG0)
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_PTR     ((reg8 *)   COUNTER_CLOCKWISE_PWMHW__CFG0)
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << COUNTER_CLOCKWISE_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT         (*(reg8 *)  COUNTER_CLOCKWISE_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_PTR     ((reg8 *)   COUNTER_CLOCKWISE_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << COUNTER_CLOCKWISE_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define COUNTER_CLOCKWISE_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (COUNTER_CLOCKWISE_UsingFixedFunction) */
#endif /* (COUNTER_CLOCKWISE_DeadBandMode == COUNTER_CLOCKWISE__B_PWM__DBM_256_CLOCKS) */



#if (COUNTER_CLOCKWISE_UsingFixedFunction)
    #define COUNTER_CLOCKWISE_STATUS                 (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__SR0)
    #define COUNTER_CLOCKWISE_STATUS_PTR             ((reg8 *) COUNTER_CLOCKWISE_PWMHW__SR0)
    #define COUNTER_CLOCKWISE_STATUS_MASK            (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__SR0)
    #define COUNTER_CLOCKWISE_STATUS_MASK_PTR        ((reg8 *) COUNTER_CLOCKWISE_PWMHW__SR0)
    #define COUNTER_CLOCKWISE_CONTROL                (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__CFG0)
    #define COUNTER_CLOCKWISE_CONTROL_PTR            ((reg8 *) COUNTER_CLOCKWISE_PWMHW__CFG0)
    #define COUNTER_CLOCKWISE_CONTROL2               (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__CFG1)
    #define COUNTER_CLOCKWISE_CONTROL3               (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__CFG2)
    #define COUNTER_CLOCKWISE_GLOBAL_ENABLE          (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__PM_ACT_CFG)
    #define COUNTER_CLOCKWISE_GLOBAL_ENABLE_PTR      ( (reg8 *) COUNTER_CLOCKWISE_PWMHW__PM_ACT_CFG)
    #define COUNTER_CLOCKWISE_GLOBAL_STBY_ENABLE     (*(reg8 *) COUNTER_CLOCKWISE_PWMHW__PM_STBY_CFG)
    #define COUNTER_CLOCKWISE_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) COUNTER_CLOCKWISE_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define COUNTER_CLOCKWISE_BLOCK_EN_MASK          (COUNTER_CLOCKWISE_PWMHW__PM_ACT_MSK)
    #define COUNTER_CLOCKWISE_BLOCK_STBY_EN_MASK     (COUNTER_CLOCKWISE_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define COUNTER_CLOCKWISE_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define COUNTER_CLOCKWISE_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define COUNTER_CLOCKWISE_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define COUNTER_CLOCKWISE_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define COUNTER_CLOCKWISE_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define COUNTER_CLOCKWISE_CTRL_ENABLE            (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_CTRL_ENABLE_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_RESET             (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_CTRL_RESET_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << COUNTER_CLOCKWISE_CTRL_CMPMODE2_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << COUNTER_CLOCKWISE_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define COUNTER_CLOCKWISE_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define COUNTER_CLOCKWISE_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define COUNTER_CLOCKWISE_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define COUNTER_CLOCKWISE_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define COUNTER_CLOCKWISE_STATUS_TC_INT_EN_MASK_SHIFT            (COUNTER_CLOCKWISE_STATUS_TC_SHIFT - 4u)
    #define COUNTER_CLOCKWISE_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define COUNTER_CLOCKWISE_STATUS_CMP1_INT_EN_MASK_SHIFT          (COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define COUNTER_CLOCKWISE_STATUS_TC              (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_TC_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_CMP1            (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define COUNTER_CLOCKWISE_STATUS_TC_INT_EN_MASK              (uint8)((uint8)COUNTER_CLOCKWISE_STATUS_TC >> 4u)
    #define COUNTER_CLOCKWISE_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)COUNTER_CLOCKWISE_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define COUNTER_CLOCKWISE_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define COUNTER_CLOCKWISE_RT1_MASK              (uint8)((uint8)0x03u << COUNTER_CLOCKWISE_RT1_SHIFT)
    #define COUNTER_CLOCKWISE_SYNC                  (uint8)((uint8)0x03u << COUNTER_CLOCKWISE_RT1_SHIFT)
    #define COUNTER_CLOCKWISE_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define COUNTER_CLOCKWISE_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << COUNTER_CLOCKWISE_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define COUNTER_CLOCKWISE_SYNCDSI_EN            (uint8)((uint8)0x0Fu << COUNTER_CLOCKWISE_SYNCDSI_SHIFT)


#else
    #define COUNTER_CLOCKWISE_STATUS                (*(reg8 *)   COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define COUNTER_CLOCKWISE_STATUS_PTR            ((reg8 *)    COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define COUNTER_CLOCKWISE_STATUS_MASK           (*(reg8 *)   COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__MASK_REG)
    #define COUNTER_CLOCKWISE_STATUS_MASK_PTR       ((reg8 *)    COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__MASK_REG)
    #define COUNTER_CLOCKWISE_STATUS_AUX_CTRL       (*(reg8 *)   COUNTER_CLOCKWISE_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define COUNTER_CLOCKWISE_CONTROL               (*(reg8 *)   COUNTER_CLOCKWISE_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define COUNTER_CLOCKWISE_CONTROL_PTR           ((reg8 *)    COUNTER_CLOCKWISE_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define COUNTER_CLOCKWISE_CTRL_ENABLE_SHIFT      (0x07u)
    #define COUNTER_CLOCKWISE_CTRL_RESET_SHIFT       (0x06u)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define COUNTER_CLOCKWISE_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define COUNTER_CLOCKWISE_CTRL_ENABLE            (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_CTRL_ENABLE_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_RESET             (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_CTRL_RESET_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << COUNTER_CLOCKWISE_CTRL_CMPMODE2_SHIFT)
    #define COUNTER_CLOCKWISE_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << COUNTER_CLOCKWISE_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define COUNTER_CLOCKWISE_STATUS_KILL_SHIFT          (0x05u)
    #define COUNTER_CLOCKWISE_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define COUNTER_CLOCKWISE_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define COUNTER_CLOCKWISE_STATUS_TC_SHIFT            (0x02u)
    #define COUNTER_CLOCKWISE_STATUS_CMP2_SHIFT          (0x01u)
    #define COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define COUNTER_CLOCKWISE_STATUS_KILL_INT_EN_MASK_SHIFT          (COUNTER_CLOCKWISE_STATUS_KILL_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (COUNTER_CLOCKWISE_STATUS_FIFONEMPTY_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (COUNTER_CLOCKWISE_STATUS_FIFOFULL_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_TC_INT_EN_MASK_SHIFT            (COUNTER_CLOCKWISE_STATUS_TC_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_CMP2_INT_EN_MASK_SHIFT          (COUNTER_CLOCKWISE_STATUS_CMP2_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_CMP1_INT_EN_MASK_SHIFT          (COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define COUNTER_CLOCKWISE_STATUS_KILL            (uint8)((uint8)0x00u << COUNTER_CLOCKWISE_STATUS_KILL_SHIFT )
    #define COUNTER_CLOCKWISE_STATUS_FIFOFULL        (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_FIFOFULL_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_FIFONEMPTY_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_TC              (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_TC_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_CMP2            (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_CMP2_SHIFT)
    #define COUNTER_CLOCKWISE_STATUS_CMP1            (uint8)((uint8)0x01u << COUNTER_CLOCKWISE_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define COUNTER_CLOCKWISE_STATUS_KILL_INT_EN_MASK            (COUNTER_CLOCKWISE_STATUS_KILL)
    #define COUNTER_CLOCKWISE_STATUS_FIFOFULL_INT_EN_MASK        (COUNTER_CLOCKWISE_STATUS_FIFOFULL)
    #define COUNTER_CLOCKWISE_STATUS_FIFONEMPTY_INT_EN_MASK      (COUNTER_CLOCKWISE_STATUS_FIFONEMPTY)
    #define COUNTER_CLOCKWISE_STATUS_TC_INT_EN_MASK              (COUNTER_CLOCKWISE_STATUS_TC)
    #define COUNTER_CLOCKWISE_STATUS_CMP2_INT_EN_MASK            (COUNTER_CLOCKWISE_STATUS_CMP2)
    #define COUNTER_CLOCKWISE_STATUS_CMP1_INT_EN_MASK            (COUNTER_CLOCKWISE_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define COUNTER_CLOCKWISE_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define COUNTER_CLOCKWISE_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define COUNTER_CLOCKWISE_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define COUNTER_CLOCKWISE_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define COUNTER_CLOCKWISE_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* COUNTER_CLOCKWISE_UsingFixedFunction */

#endif  /* CY_PWM_COUNTER_CLOCKWISE_H */


/* [] END OF FILE */
