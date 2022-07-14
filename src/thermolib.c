/**
 * @file thermolib.c
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @date 2022-07-08
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <thermolib.h>
#include <tables.h>
#include <stdio.h>
#include <utils.h>

typedef enum TC_PARAM{
    TEMPERATURE = 0,
    VOLTAGE
}tc_param_t;

/* Helper macros for general purpose interpolation */
#define INTERPOLATE_BASE(dst, src, r1, r2, cmp) \
    CLAMP(\
        (float)((r2.dst - r1.dst) * (cmp - r1.src) / (r2.src - r1.src)) + r1.dst, \
        MIN(r1.dst, r2.dst), \
        MAX(r1.dst, r2.dst)\
    )

#define INTERPOLATE_SRC_VOLTAGE(r1, r2, cmp) INTERPOLATE_BASE(t, uv, r1, r2, cmp)
#define INTERPOLATE_SRC_TEMPERATURE(r1, r2, cmp) INTERPOLATE_BASE(uv, t, r1, r2, cmp)
#define INTERPOLATE_BY_SRC(src, r1, r2, cmp) (src == TEMPERATURE ? INTERPOLATE_SRC_TEMPERATURE(r1, r2, cmp) : INTERPOLATE_SRC_VOLTAGE(r1, r2, cmp)) 
#define CLOSEST_MATCH_BASE(member, r1, r2, cmp) ((r1.member - cmp) < (cmp - r2.member) ? r1.member : r2.member)
#define CLOSET_MATCH_BY_DST(dst, r1, r2, cmp) (dst == VOLTAGE ? CLOSEST_MATCH_BASE(uv, r1, r2, cmp) : CLOSEST_MATCH_BASE(t, r1, r2, cmp))

/**
 * @brief Searches provided `table` for the closest match to the input `cmp`, and provides the corresponding
 * output as either an exact match, a closest match, or an interpolated match depending on the input parameters.
 * 
 * Currently, this uses a binary search which requires the data is sorted. If data is unsorted, it will return an
 * error and will be unable to proceed. Eventually, it should fall back to a linear search in this case.
 * 
 * @param dst : `tc_param_t` representing what the computed output should be, either `VOLTAGE` or `TEMPERATURE`
 * e.g. if a temperature is known, and a voltage is needed, the `dst` would be VOLTAGE.
 * @param result : float pointer to the output result. If `dst` is `VOLTAGE` this would be a voltage, in microvolts, for instance.
 * @param cmp : float representing the known value. i.e. if searching for a `VOLTAGE`, provide a `TEMPERATURE` in degrees C
 * @param table : thermocouple table being searched
 * @param interpolate : `true` if interpolation should be used when an exact match isn't found. `false` otherwise
 * @retval 0 on sucess
 * @retval -errno otherwise
 */
static int search_table(tc_param_t dst, float * result, float cmp, const struct tc_table * table, bool interpolate);

int thermocouple_temperature(enum TC_TYPES type, float * temperature, int measured_uv, float cold_junction, bool interpolate){
    if (temperature == NULL){
        return -EFAULT;
    }
    const struct tc_table * table = select_thermocouple_table(type);
    float compensation_uv = 0;
    int rc = search_table(VOLTAGE, &compensation_uv, cold_junction, table, interpolate);
    if (rc < 0){
        return rc;
    }
    rc = search_table(TEMPERATURE, temperature, measured_uv + compensation_uv, table, interpolate);
    return rc;
}

static int search_table(tc_param_t dst, float * result, float cmp, const struct tc_table * table, bool interpolate){
    if (table == NULL){
        return -ENODEV;
    }
    /* If our output (dst) is VOLTAGE, then our input (src) is TEMPERATURE, and vice-versa */
    tc_param_t src = dst == VOLTAGE ? TEMPERATURE : VOLTAGE;

    int low = 0;
    int high = table->items - 1;
    int minval;
    int maxval;

    /* Check for edge cases. Also verify this data is SORTED,
    otherwise we cannot perform a binary search */ 
    switch(src){
        case TEMPERATURE:
            if (!table->t_sorted){
                return -ENOEXEC;
            }
            minval = table->t_min;
            maxval = table->t_max;
        break;
        case VOLTAGE:
            if (!table->uv_sorted){
                return -ENOEXEC;
            }
            minval = table->uv_min;
            maxval = table->uv_max;
        break;
    }

    if (cmp < minval){
        return low;
    }
    else if (cmp > maxval){
        return high;
    }

    const struct tc_table_data * data = table->data;
    int value;
    /* Binary search for closest match in table to src comparison */
    while(low < high){
        int middle = (low + high)/2;
        switch(src){
            case TEMPERATURE:
                value = data[middle].t;
            break;
            case VOLTAGE:
                value = data[middle].uv;
            break;
        }
        if (cmp < value) {
            high = middle - 1;
        } else if (cmp > value) {
            low = middle + 1;
        } 
        if (cmp == value || high == low){
            /* Exact match found */
            if (low == high){
                /* If high == low, middle == low == high */
                middle = low;
            }
            switch(dst){
                case VOLTAGE:
                    *result = data[middle].uv; return 0;
                case TEMPERATURE:
                    *result = data[middle].t; return 0;
            }
        }
    }
    
    if (interpolate){
        *result = INTERPOLATE_BY_SRC(src, data[low], data[high], cmp);
    }
    else{
        *result = CLOSET_MATCH_BY_DST(dst, data[low], data[high], cmp);
    }
    return 0;
}
