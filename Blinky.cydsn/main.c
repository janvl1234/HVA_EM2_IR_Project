/* ========================================
 *
 * De eerste opdracht.
 *
 * ========================================
*/
#include <project.h>

/* Set to TV address 1 */
#define TARGET_ADDRESS  0x00u

/* Button press - Decrease screen brightness */
#define SYSTEM_COMMAND_0    0x13u

/* Capsense slider - Increase volume */
#define SYSTEM_COMMAND_1    0x0Fu

/* Capsense slider - Decrease volume */
#define SYSTEM_COMMAND_2    0x10u

/* Shifts and masks */
#define ADDRESS_MASK    0x10u
#define ADDRESS_SHIFT   0x04u
#define COMMAND_MASK    0x20u
#define COMMAND_SHIFT   0x05u

/* Packet variables */
uint8 startBit = 0x01u;
uint8 fieldBit = 0x01u;
uint8 controlBit = 0x00u;
uint8 address, addressBit;
uint8 command, commandBit;

/* Control variables */
uint8 previousCommand = 0u;
uint8 transmitState = 0u;
uint8 buttonState = 0u;
uint8 buttonRelease = 0u;
uint8 autoRepeat = 0u;
uint8 capsenseInitial = 1u;
uint16 curPos, oldPos;

int main(void);
//void  sendSAMSUNG (unsigned long data,  int nbits);
void sendCommand(void);

int main(void)
{
    /* Enable variable used to prevent unintended
        transmission at start-up. */
    capsenseInitial = 1u;
    
    IR_PWM_Start();
    
    while(1)
    {        
		/* Check to see if a command is detected */
        SendCommand();
    }
}

void sendCommand(void)
{
    
    ControlReg_Write(transmitState);
}

/* [] END OF FILE */
