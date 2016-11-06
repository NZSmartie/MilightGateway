/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_button_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_button_radio_tx_example
*
* @brief Radio Transceiver Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "radio/config.h"
#include "nrf_gpio.h"
#include "app_timer.h"
#include "boards.h"
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_error.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "milight.h"
#include "util.h"

#define APP_TIMER_PRESCALER      0                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE  2                           /**< Size of timer operation queues. */

static uint32_t button_state;
static uint8_t packet[12] = {0};
static uint8_t repeats = 10; 

void uart_error_handle(app_uart_evt_t * p_event)
{
   // No implementation needed.
}


/**@brief Function for sending button_state.
 */
void send_packet()
{
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;

    while (NRF_RADIO->EVENTS_READY == 0U);
    
    // Transmit the same packet over and over
    for(uint8_t c = 0; c < repeats; c++){
        NRF_RADIO->EVENTS_END  = 0U;
        NRF_RADIO->TASKS_START = 1U;

        while (NRF_RADIO->EVENTS_END == 0U);
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U);
}


/**@brief Function for handling bsp events.
 */
void bsp_evt_handler(bsp_event_t evt)
{
    uint32_t err_code = NRF_SUCCESS;
    switch (evt)
    {
        case BSP_EVENT_KEY_0:
        case BSP_EVENT_KEY_1:
        case BSP_EVENT_KEY_2:
        case BSP_EVENT_KEY_3:
        case BSP_EVENT_KEY_4:
        case BSP_EVENT_KEY_5:
        case BSP_EVENT_KEY_6:
        case BSP_EVENT_KEY_7:
            // get actual button state
            err_code = bsp_buttons_state_get(&button_state);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            // no implementation needed
            break;
    }
}


/**@brief Function for initialization oscillators.
 */
void clock_initialization()
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }

    /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
}


/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
    milight_command mi_cmd = {
        .id = 0x000BF2EA,
        .brightness = 0x90,
        .color = 0x35,
        .code = MILIGHT_COMMAND_CODE_ON,
        .sequence = 0
    };
    uint32_t err_code = NRF_SUCCESS;

    clock_initialization();
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS,
                        APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
                        bsp_evt_handler);
    APP_ERROR_CHECK(err_code);

    // Set radio configuration parameters
    radio_configure();

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)packet;

    err_code = bsp_indication_text_set(BSP_INDICATE_USER_STATE_OFF, "Press Any Button\r\n");
    APP_ERROR_CHECK(err_code);

    while (true)
    {
        if (button_state != 0)
        {
            if(button_state == 0x01){
                // Turn on all Lights to White
                mi_cmd.code = MILIGHT_COMMAND_CODE_ON | MILIGHT_COMMAND_ALT;
            }else if(button_state == 0x02){
                // Cycle colours
                mi_cmd.color += 10;
                mi_cmd.code = MILIGHT_COMMAND_CODE_COLOR;
            }else if(button_state == 0x04){
                // Turn off all lights
                mi_cmd.code = MILIGHT_COMMAND_CODE_OFF;
            }else if(button_state == 0x08){
                // Turn on all lights
                mi_cmd.code = MILIGHT_COMMAND_CODE_ON;
            }else{
                // don't do anyhting
                mi_cmd.code = MILIGHT_COMMAND_CODE_NONE;
            }

            if(mi_cmd.code != MILIGHT_COMMAND_CODE_NONE){
                milight_write_packet(&mi_cmd, &packet[1]);
                NRF_LOG_INFO("Milight Packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
                                packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);
                NRF_LOG_RAW_INFO(" 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
                                packet[6], packet[7], packet[8], packet[9], packet[10]);
                packet[0] = 0x00;
                shift_swap_nibbles(packet,packet, 11);
                packet[0] = 0x25;
                packet[1] |= 0x05;
                packet[11] |= 0xF0;
                NRF_LOG_INFO("Encoded Packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
                                packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);
                NRF_LOG_RAW_INFO(" 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n", 
                                packet[6], packet[7], packet[8], packet[9], packet[10], packet[11]);
                send_packet();
            }
            button_state = 0;
        }
        NRF_LOG_FLUSH();
        __WFE();
    }
}


/**
 *@}
 **/
