/**
 * @file tables.h
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @date 2022-07-11
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef __THERMOLIB_TABLES_H
#define __THERMOLIB_TABLES_H

#include <stdint.h>
#include <thermolib.h>

/**
 * @brief Table data item for thermocouple data.
 * 
 * Each item is a data paid containing:
 * 
 * @param t: Temperature in celcius
 * @param uv: its associated voltage in microvolts.
 * 
 * A table is composed primarily of these items which can be searched
 * by either item. i.e. there will be search functions T(v) and V(t)
 * to return either a temperature based on a voltage, or a voltage based
 * on a temperature, respectively. Ideally, these items would be sorted
 * by one or both of these indeces to improve the ability to search.
 */
struct tc_table_data {
    int16_t t;
    int32_t uv;
};

/**
 * @brief Table containing `tc_table_data` data, as well as additional
 * metadata to assist in searching or reporting.
 * 
 * @param name: string representing the name of the dataset
 * @param t_sorted: true if data is sorted by temperature
 * @param uv_sorted: true if data is sorted by voltage
 * @param items: number of data elements in the table
 * @param t_min: minimum temperature in table (degrees C)
 * @param t_max: maximum temperature in table (degrees C)
 * @param uv_min: minimum voltage in table (microvolts)
 * @param uv_max: maximum voltage in table (microvolts)
 * @param table: pointer to table data composed of `tc_table_data` entries
 * 
 * @note: t_sorted and uv_sorted are not mutually exclusive.
 * Generally, if data is sorted by temperature, it should also
 * be sorted by voltage by nature of the Seebeck effect. However,
 * if for some reason that isn't the case, it would need to be known
 * so as to avoid searching unsorted data assuming it is sorted.
 */
struct tc_table {
    const char * name;
    bool t_sorted;
    bool uv_sorted;
    uint16_t items;
    int16_t t_min;
    int16_t t_max;
    int32_t uv_min;
    int32_t uv_max;
    const struct tc_table_data * data;
};

/**
 * @brief Select a thermocouple table by indexed type
 * 
 * @param type: `enum TC_TYPES` representing the index for the thermocouple table desired
 * @retval pointer to `struct tc_table` containing the desired data, if found
 * @retval NULL otherwise
 */
struct tc_table * select_thermocouple_table(enum TC_TYPES type);

/**
 * @brief Get a thermocouple table by indexed type
 * 
 * @param name: string representing the name assigned directly to the thermocouple data. 
 * Each tables `name` property will be checked against this for a match
 * 
 * @retval pointer to `struct tc_table` containing the desired data, if found
 * @retval NULL otherwise
 */
struct tc_table * find_thermocouple_table(const char * name);

#endif
