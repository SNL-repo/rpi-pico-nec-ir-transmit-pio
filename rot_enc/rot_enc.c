/**
 * Copyright (c) 2021 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


// SDK types and declarations
//
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"    // for clock_get_hz()


// public API declarations
//
#include "rot_enc.h"


// PIO state machine programs:
//
#include "rot_enc.pio.h"



// public API definitions
//
int rot_enc_init(PIO pio, uint pin_num) {
    // initialise PIO specified GPIO pin and next pin for A/B

    // load and run carrier burst program
    

    uint rot_enc_offset = pio_add_program(pio, &rot_enc_program);
    int rot_enc_sm = pio_claim_unused_sm(pio, true);

    if (rot_enc_sm == -1) {
        return -1;
    }

    rot_enc_program_init(pio, sm, pin_num, 0);                   // 38.222 kHz carrier

    return rot_enc_sm;
}
