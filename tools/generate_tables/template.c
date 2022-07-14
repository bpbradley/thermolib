/**
 * @file {FILENAME}.c
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @brief Autogenerated tables based on NIST data -- DO NOT EDIT
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <stdint.h>
#include <tables.h>

const struct tc_table_data {DATA_NAME}[{ITEM_COUNT}] = {
{TABLE_DATA}
};

struct tc_table {TABLE_NAME} = {
    .name = "{FILENAME}"
    .t_sorted = true,
    .uv_sorted = {VOLTAGE_SORTED},
    .t_min = {MINIMUM_TEMPERATURE},
    .t_max = {MAXIMUM_TEMPERATURE},
    .uv_min = {MINIMUM_VOLTAGE},
    .uv_max = {MAXIMUM_VOLTAGE},
    .items = {ITEM_COUNT},
    .data = {DATA_NAME}
};