/**
 * Copyright (c) 2021 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h> 
#include "pico/stdlib.h"
#include "nec_transmit.h"                       // library header
#include "hardware/pio.h"
#include "quadrature.pio.h"

#define QUADRATURE_A_PIN 2
#define QUADRATURE_B_PIN 3

int main() {
    stdio_init_all();

    PIO pio = pio0;                             // PIO to use
    PIO encoderPIO = pio1;

    uint gpio = 15;                              // GPIO for IR signal

    int sm = nec_tx_init(pio, gpio);            // initialise library

    if (sm == -1) {
        return -1;                              // error initialising library
    }

    /*
    generate "NEC" IR codes using 38kHz carrier
    (https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol)

    all the following examples generate the same bit sequence 'on the wire':
    0000 0000 1111 1111 1011 0101 0100 1010

    the functions return immediately unless the state machine FIFO fills up
    but most receivers need a little recovery time between frames

    note that in extended and raw modes you should avoid sending codes
    where the low and high address bytes are the inverse of each other
    otherwise you may interfere with other devices nearby
    */


    uint offsetA = pio_add_program(encoderPIO, &quadratureA_program);
    uint smA = pio_claim_unused_sm(encoderPIO, true);

    uint offsetB = pio_add_program(encoderPIO, &quadratureB_program);
    uint smB = pio_claim_unused_sm(encoderPIO, true);

    quadratureA_program_init(encoderPIO, smA, offsetA, QUADRATURE_A_PIN, QUADRATURE_B_PIN);
    quadratureB_program_init(encoderPIO, smB, offsetB, QUADRATURE_A_PIN, QUADRATURE_B_PIN);


    while (true) {

        // normal mode (8 bit address and data, lsb first)
        //      8 bit address,
        //      8 bit inverted address,
        //      8 bit data,
        //      8 bit inverted data
        nec_tx(pio, sm, 0x06, 0x11);
        sleep_ms(150);

        nec_tx(pio, sm, 0x06, 0x12);
        sleep_ms(150);

        // extended mode (16 bit address and data, lsb first)
        //      16 bit address,
        //      16 bit data
        //nec_tx_extended(pio, sm, 0xff00, 0x52ad);
        //sleep_ms(150);

        // raw frame (32 bit data, lsb first)
        //nec_tx_raw(pio, sm, 0x52adff00);
        //sleep_ms(150);

    }
}
