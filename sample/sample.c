/**
 * @file sample.c
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @date 2022-07-12
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <thermolib.h>
#include <stdio.h>
#include "sample.h"

int main(int argc, char ** argv){
    float temperature = 0;
    struct tc_items sample;
    int idx = 0;
    while (get_tc_sample(&sample, idx++) == 0){
        int rc = thermocouple_temperature(TC_TYPE_K, &temperature, sample.microvolts, sample.celcius, true);
        printf("%d uV @ %d C = %f\n", sample.microvolts, sample.celcius, temperature);
    }
    return 0;
}
