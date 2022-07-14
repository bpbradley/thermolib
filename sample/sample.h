/**
 * @file sample.h
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @date 2022-07-14
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef __THERMOLIB_SAMPLE_H
#define __THERMOLIB_SAMPLE_H

#include <errno.h>
struct tc_items{
    int celcius;
    int microvolts;
};

int get_tc_sample(struct tc_items * sample, int idx);

#endif
