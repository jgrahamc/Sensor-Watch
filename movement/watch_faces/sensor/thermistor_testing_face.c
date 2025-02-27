/*
 * MIT License
 *
 * Copyright (c) 2022 Joey Castillo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include "thermistor_testing_face.h"
#include "thermistor_driver.h"
#include "watch.h"

// This watch face is designed for testing temperature sensor boards.
// It displays temperature readings at a relatively fast rate of 8 Hz,
// and disables low energy mode so my testing device doesn't sleep.
// You more than likely want to use thermistor_readout_face instead.

static void _thermistor_testing_face_update_display() {
    thermistor_driver_enable();
    float temperature_c = thermistor_driver_get_temperature();
    char buf[14];
    sprintf(buf, "%4.1f#C", temperature_c);
    watch_display_string(buf, 4);
    thermistor_driver_disable();
}

void thermistor_testing_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    (void) context_ptr;
    // force one setting: never enter low energy mode.
    // I'm using this watch face to test the temperature sensor boards; there's no need for it.
    settings->bit.le_interval = 0;
}

void thermistor_testing_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
    watch_display_string("TE", 0);
    movement_request_tick_frequency(8);
}

bool thermistor_testing_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    (void) context;
    (void) settings;
    switch (event.event_type) {
        case EVENT_MODE_BUTTON_UP:
            movement_move_to_next_face();
            break;
        case EVENT_ALARM_BUTTON_DOWN:
            break;
        case EVENT_ACTIVATE:
        case EVENT_TICK:
            _thermistor_testing_face_update_display();
            break;
        default:
            break;
    }

    return true;
}

void thermistor_testing_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
}
