#!/bin/python3
import sys
import csv
import matplotlib.pyplot as plt

COLUMNS = ['best', 'average', 'worst']
COLUMN_COLOR = ['r-', 'g-', 'b-']

if len(sys.argv) < 2:
    print("Missing csv file path argument")
    exit(1)

csv_file_path = sys.argv[1]
data = {c: [] for c in COLUMNS}

with open(csv_file_path, newline='') as csv_file:
    dict_reader = csv.DictReader(csv_file)
    for row in dict_reader:
        for column in COLUMNS:
            data[column].append(float(row[column]))

for column, color in zip(COLUMNS, COLUMN_COLOR):
    plt.plot(data[column], color)
plt.show()