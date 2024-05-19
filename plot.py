#!/bin/python3
import sys
import csv
import matplotlib.pyplot as plt

COLUMNS = ['best', 'average', 'worst']
COLUMN_COLOR = ['g-', 'b-', 'r-']

csv_file_path = None
plot_out_path = None
show_plot = False

for arg in sys.argv[1:]:
    if arg[0] == '-': # Flags
        if arg[1] == '-': # Word flags
            show_plot = arg == "--show-plot" or show_plot
        else:
            show_plot = "p" in arg or show_plot
    else: # Arguments
        if csv_file_path is None:
            csv_file_path = arg
        elif plot_out_path is None:
            plot_out_path = arg

if csv_file_path is None:
    print("Missing csv file path argument")
    exit(1)

data = {c: [] for c in COLUMNS}

with open(csv_file_path, newline='') as csv_file:
    dict_reader = csv.DictReader(csv_file)
    for row in dict_reader:
        for column in COLUMNS:
            data[column].append(float(row[column]))

plt.xlabel("Geração")
plt.ylabel("Fitness")

for column, color in zip(COLUMNS, COLUMN_COLOR):
    plt.plot(data[column], color, label=column)
plt.legend(loc="lower right")

if plot_out_path is not None:
    plt.savefig(plot_out_path, dpi=100)

if show_plot:
    plt.show()