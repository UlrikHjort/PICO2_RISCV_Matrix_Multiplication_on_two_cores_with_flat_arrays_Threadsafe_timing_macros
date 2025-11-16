/**************************************************************************
--           PICO2 RISC-V Threadsafe UART TX PIO SM
-- 
--           Copyright (C) 2025 By Ulrik Hørlyk Hjort
--
--  This Program is Free Software; You Can Redistribute It and/or
--  Modify It Under The Terms of The GNU General Public License
--  As Published By The Free Software Foundation; Either Version 2
--  of The License, or (at Your Option) Any Later Version.
--
--  This Program is Distributed in The Hope That It Will Be Useful,
--  But WITHOUT ANY WARRANTY; Without Even The Implied Warranty of
--  MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See The
--  GNU General Public License for More Details.
--
-- You Should Have Received A Copy of The GNU General Public License
-- Along with This Program; if not, See <Http://Www.Gnu.Org/Licenses/>.
***************************************************************************/
#include <stdint.h>
#include "pico/stdio/driver.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "uart_pio.pio.h"
#include "pico/mutex.h"

#define LOW 0
#define HIGH 1

/* UART configutation:
 * 8N1
 * 8 data bits, No parity, 1 stop bit
 */

uint32_t bit_time_us = 0;
uint tx_pin = 0;

PIO pio;
uint sm;


static mutex_t pio_uart_mutex;

// Thread safe printf redirect
static void printf_redirect(const char *buf, int length) {
	mutex_enter_blocking(&pio_uart_mutex);
    for (int i = 0; i < length; i++) {
		pio_sm_put_blocking(pio, sm, (uint32_t)*(buf+i));        
    }
	mutex_exit(&pio_uart_mutex);
}


static stdio_driver_t uart_pio_stdio = {
    .out_chars = printf_redirect,
    .in_chars = NULL,  
    .next = NULL,
};


void init_uart(uint32_t baud_rate, uint tx_pin_) {
	tx_pin = tx_pin_;
	pio = pio0;
	mutex_init(&pio_uart_mutex);
    uint offset = pio_add_program(pio, &uart_tx_program);
    sm = pio_claim_unused_sm(pio, true);

	
	uart_tx_program_init(pio, sm, offset, baud_rate, tx_pin);
	stdio_set_driver_enabled(&uart_pio_stdio, true);
}


