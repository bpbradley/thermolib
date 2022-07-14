/**
 * @file tables.c
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @brief 
 * @date 2022-07-11
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <tables.h>
#include <utils.h>
#include <string.h>

struct tc_table_entry{
    enum TC_TYPES type;
    struct tc_table * table;
};

#define THERMOCOUPLE_TABLE_DECLARE(name)        extern struct tc_table name;
#define TC_TABLE_ENTRY_ADD(_type, _table)       {.type = _type, .table = &_table}

THERMOCOUPLE_TABLE_DECLARE(k_table);

struct tc_table_entry thermocouple_tables[] = {
    TC_TABLE_ENTRY_ADD(TC_TYPE_K, k_table),
};

struct tc_table * select_thermocouple_table(enum TC_TYPES type){
    const int tables = ARRAY_SIZE(thermocouple_tables);
    for (int i = 0; i < tables; i++){
        struct tc_table_entry t = thermocouple_tables[i];
        if (t.type == type){
            return t.table;
        }
    }
    return NULL;
}

struct tc_table * find_thermocouple_table(const char * name){
    const int tables = ARRAY_SIZE(thermocouple_tables);
    for (int i = 0; i < tables; i++){
        struct tc_table * t = thermocouple_tables[i].table;
        if (strcmp(t->name, name) == 0){
            return t;
        }
    }
    return NULL;
}
