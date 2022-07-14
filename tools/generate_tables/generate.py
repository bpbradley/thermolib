# Copyright (c) 2022 Brian Bradley
#
# SPDX-License-Identifier: Apache-2.0

import argparse
import os
import collections
import re
import pathlib
import logging

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'Convert NIST thermocouple datas into lookup tables in C')
    parser.add_argument('input', metavar='f', type=str, help='file containing NIST data')
    parser.add_argument('--name', dest='name', default=None,
                    help='Provide a name for the file')
    parser.add_argument('--table_name', dest='table_name', default=None,
                    help='Provide a variable name for the table')
    parser.add_argument('--linewidth', dest='linewidth', type=int, default=10,
                    help='Specify a maximum number of samples placed on each line. (Default: 10)')
    parser.add_argument('--dst', dest='dst', default='output',
                    help='Provide an output directory as a destination for the file')
    
    args = parser.parse_args()
    TEMPLATE_VARS=dict()
    if args.name is None:
        #Make up a name using the basename of the input file.
        name = os.path.splitext(os.path.basename(args.input))[0]
    else:
        name = args.name
        
    f_name = name + ".c"

    table_map=dict()
    with open(args.input, "r") as table:
        for line in table.readlines():
            values = list(filter(None, line.strip().split(" ")))
            base = int(values[0])
            data = [int(1000 * float(v)) for v in values[1:]]
            for v in range(len(data)):
                if data[v] < 0:
                    idx = base - v
                else:
                    idx = base + v
                table_map[idx] = data[v]
            
    sorted_table = collections.OrderedDict(sorted(table_map.items()))
    sorted_values = sorted(sorted_table.values())
    voltage_sorted = list(sorted_table.values()) == sorted_values

    TEMPLATE_VARS["MINIMUM_TEMPERATURE"] = list(sorted_table.keys())[0]
    TEMPLATE_VARS["MAXIMUM_TEMPERATURE"] = list(sorted_table.keys())[-1]
    TEMPLATE_VARS["MINIMUM_VOLTAGE"] = sorted_values[0]
    TEMPLATE_VARS["MAXIMUM_VOLTAGE"] = sorted_values[-1]
    TEMPLATE_VARS["FILENAME"] = name
    TEMPLATE_VARS["DATA_NAME"] = "_{}_data".format(name)
    if args.table_name is None:
        TEMPLATE_VARS["TABLE_NAME"] = "_{}".format(name)
    else:
        TEMPLATE_VARS["TABLE_NAME"] = args.table_name

    TEMPLATE_VARS["TABLE_DATA"] = ""
    if voltage_sorted:
        TEMPLATE_VARS["VOLTAGE_SORTED"] = "true"
    else:
        logging.warn("Data is not voltage sorted")
        TEMPLATE_VARS["VOLTAGE_SORTED"] = "false"
    TEMPLATE_VARS["ITEM_COUNT"] = len(sorted_table)
    for (temp, uv) in sorted_table.items():
        if temp % args.linewidth == 0:
            TEMPLATE_VARS["TABLE_DATA"] += "\n\t"
        TEMPLATE_VARS["TABLE_DATA"] += "{{.t={},.uv={}}},".format(temp, uv)
        
    with open("template.c") as f_template:
            template=f_template.readlines()
    with open(pathlib.Path(args.dst) / f_name, "w") as output:
        for line in template:
            #Check for keywords
            for key in re.findall(r'\{.*?\}', line):
                replace=TEMPLATE_VARS.get(key.strip('{}'), None)
                if replace is not None:
                    line=line.replace(key, str(replace))
            output.write(line)
