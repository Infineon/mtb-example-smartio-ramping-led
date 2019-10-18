/******************************************************************************
* File Name: main.c
*
* Description: This is the source code for the PSoC 6 MCU Ramping LED using 
* Smart I/O Example for ModusToolbox.
*
* Related Document: See Readme.md
*
*******************************************************************************
* (c) 2019, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cycfg.h"
#include "cybsp.h"

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It does...
*    1. Initialize and Start the TCPWM
*    2. Intialize and Start the Smart IO to generate a ramping signal to drive
*       an external LED.  
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts. */
    __enable_irq();

    /* Initialize the counter in the TCPWM block for the PWM operation.
     * The PWM is configured to generate a 25 Hz square wave signal
     * with 50% duty cycle */

    Cy_TCPWM_PWM_Init(PWM_HW, PWM_NUM, &PWM_config);

    /* Enable the TCPWM for PWM mode of operation */

    Cy_TCPWM_PWM_Enable(PWM_HW, PWM_NUM);

    /* Start the TCPWM block */
    Cy_TCPWM_TriggerStart(PWM_HW, PWM_MASK);

    /* Initialize the SmartIO block. The Smart I/O implements a sequential
     * circuit to generate a square wave signal with time varying duty cycle.
     * This signal drives an LED creating visual perception of breathing LED */

    Cy_SmartIO_Init(SMARTIO_PRT9, &SmartIO_config);

    /* Enable the Smart I/O */
    Cy_SmartIO_Enable(SMARTIO_PRT9);

    /* LED breathing effect is generated using PWM and Smart I/O 
     * hardware peripherals - no CPU is used */
    for(;;)
    {

    }
}

/* [] END OF FILE */
