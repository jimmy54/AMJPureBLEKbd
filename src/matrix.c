/*
 * =====================================================================================
 *
 *       Filename:  matrix.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/03/2016 10:52:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_delay.h"

#include "matrix.h"

static uint32_t rows[ROW_COUNT] = {ROW_PIN_0, ROW_PIN_1, ROW_PIN_2, ROW_PIN_3};

void matrix_init(void)
{
    // row output and hight
    for (uint8_t i = 0; i<ROW_COUNT; i++ ){
        nrf_gpio_cfg_output(rows[i]);
        nrf_gpio_pin_write(rows[i], true);
    }

    // col read
    nrf_gpio_cfg_output(COL_IC_CP_PIN);
    nrf_gpio_pin_write(COL_IC_CP_PIN, true);
    nrf_gpio_cfg_output(COL_IC_CE_PIN);
    nrf_gpio_pin_write(COL_IC_CE_PIN, true);
    nrf_gpio_cfg_output(COL_IC_PL_PIN);
    nrf_gpio_pin_write(COL_IC_PL_PIN, true);

    nrf_gpio_cfg_input(COL_IC_DIN_PIN_0, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(COL_IC_DIN_PIN_1, NRF_GPIO_PIN_PULLUP);


}

void matrix_select_row(uint8_t row)
{
    NRF_LOG_RAW_INFO("(ROW %d )", row);
    nrf_gpio_pin_write(rows[row], 0);
}

void matrix_unselect_row(uint8_t row)
{
    nrf_gpio_pin_write(rows[row], 1);
    NRF_LOG_RAW_INFO("\r\n");
}

uint16_t matrix_read_col(void)
{
    uint16_t col_value = 0;
    nrf_gpio_pin_write(COL_IC_PL_PIN, false);
    nrf_gpio_pin_write(COL_IC_PL_PIN, true);
    nrf_gpio_pin_write(COL_IC_CE_PIN, false);
    nrf_delay_us(5);
    for (uint8_t i=0; i < 8; i++) {
        nrf_gpio_pin_write(COL_IC_CP_PIN, true);
        nrf_delay_us(2);
        NRF_LOG_RAW_INFO("[");
        if( ! nrf_gpio_pin_read(COL_IC_DIN_PIN_0)){
            col_value += 1<<i;
            NRF_LOG_RAW_INFO("X] ");
        }
        else{
            NRF_LOG_RAW_INFO("O] ");
        }
        if ( i < 4) {
            NRF_LOG_RAW_INFO("<");
            if( ! nrf_gpio_pin_read(COL_IC_DIN_PIN_1)){
                NRF_LOG_RAW_INFO("X> ");
                col_value += 1<<(i + 8);
            }
            else{
                NRF_LOG_RAW_INFO("O> ");
            }
        }
        nrf_gpio_pin_write(COL_IC_CP_PIN, false);
    }
    nrf_gpio_pin_write(COL_IC_CP_PIN, true);
    nrf_gpio_pin_write(COL_IC_CE_PIN, true);
    return col_value;
}