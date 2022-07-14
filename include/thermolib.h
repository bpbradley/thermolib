/**
 * @file thermolib.h
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @brief Public header declarations required to use thermolib
 * @date 2022-07-08
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef __THERMOLIB_H
#define __THERMOLIB_H

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

enum TC_TYPES {
    TC_TYPE_B = 0,
    TC_TYPE_E,
    TC_TYPE_J,
    TC_TYPE_K,
    TC_TYPE_N,
    TC_TYPE_R,
    TC_TYPE_S,
    TC_TYPE_T,
};

/**
 * @brief Convert a voltage measurement in microvolts to a cold-junction calibrated
 * thermocouple temperature
 * 
 * @param type: `enum TC_TYPES` representing the type of thermocouple measured
 * @param temperature: pointer to temperature data to be calculated
 * @param measured_uv: measured thermocouple voltage in microvolts
 * @param cold_junction: measured cold junction temperature in degrees celcius
 * @param interpolate: `true` if interpolation should be used when an exact match isn't found. `false` otherwise
 * 
 * @retval 0 on success
 * @retval -errno otherwise
 */
int thermocouple_temperature(enum TC_TYPES type, float * temperature, int measured_uv, float cold_junction, bool interpolate);

#endif
