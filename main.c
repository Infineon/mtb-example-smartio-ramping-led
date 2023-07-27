/*******************************************************************************
* File Name: main.c
*
* Description: This example project demonstrates the Ramping an LED using 
* Smart I/O Example for ModusToolbox.
*
* Related Document: See README.md
*
********************************************************************************
* Copyright 2019-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
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
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/*******************************************************************************
* Global Variables
********************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
* Summary:
* User defined error handling function
*
* Parameters:
*  uint32_t status - status indicates success or failure
*
* Return:
*  void
*
*******************************************************************************/
void handle_error(uint32_t status)
{
    if (status != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function enrty for the CPU. It does...
*    1. Initialize the board, retarget-io and PWM
*    2. Initialize and start the Smart IO to generate a ramping signal to drive
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
    cy_en_smartio_status_t status;

    #if defined(CY_DEVICE_SECURE)
        cyhal_wdt_t wdt_obj;
        /* Clear watchdog timer so that it doesn't trigger a reset */
        result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        cyhal_wdt_free(&wdt_obj);
    #endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    /* Board init failed. Stop program execution */
    handle_error(result);

    /* Initialize retarget-io for uart logs */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                                      CY_RETARGET_IO_BAUDRATE);
    /* Retarget-io init failed. Stop program execution */
    handle_error(result);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("*************** SmartIO: Ramping LED *************** \r\n\n");

    /* Enable interrupts */
    __enable_irq();

    /* Initialize the counter in the TCPWM block for the PWM operation.
     * The PWM is configured to generate a 25 Hz square wave signal
     * with 50% duty cycle */
    Cy_TCPWM_PWM_Init(PWM_HW, PWM_NUM, &PWM_config);

    /* Enable the TCPWM for PWM mode of operation */

    Cy_TCPWM_PWM_Enable(PWM_HW, PWM_NUM);

    /* Start the TCPWM block */

    Cy_TCPWM_TriggerStart_Single(PWM_HW, PWM_NUM);

    /* Initialize the SmartIO block. The Smart I/O implements a sequential
     * circuit to generate a square wave signal with time varying duty cycle.
     * This signal drives an LED creating visual perception of breathing LED */

    status = Cy_SmartIO_Init(smart_io_HW, &smart_io_config);

    if(CY_SMARTIO_SUCCESS != status)
    {
        printf("SmartIO init failed with error: %lu\r\n",
                                                 (unsigned long) status);
        CY_ASSERT(0);
    }

    /* Enable the Smart I/O */
    Cy_SmartIO_Enable(smart_io_HW);

    printf("SmartIO init done, output is enabled on P%d_%d \r\n",
                                                SMART_IO_OUTPUT_PIN_PORT_NUM,
                                                SMART_IO_OUTPUT_PIN_NUM);

    /* LED breathing effect is generated using PWM and Smart I/O 
     * hardware peripherals - no CPU is used */
    for(;;)
    {

    }
}

/* [] END OF FILE */