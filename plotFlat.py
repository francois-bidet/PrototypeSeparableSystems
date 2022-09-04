#!/usr/bin/python3

def alternativeMerge(lst1, lst2):
    return [lst[i] for i in range(len(lst2)) for lst in [lst1,lst2]]

def double(lst):
    return [v for v in lst for j in range(2)]

import sys

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from matplotlib.patches import Patch

filename = "data.dat"
if len(sys.argv) > 1:
    filename = sys.argv[1]

data = pd.read_csv(filename, sep='\t')

columns = data.columns
dim = len(columns) // 2 - 1

dimX = 0
dimYs = list(range(1,dim+1))

if len(sys.argv) > 2:
    dimX = int(sys.argv[2])
    if len(sys.argv) > 3:
        dimYs = []
        for i in range(3,len(sys.argv)):
            dimYs.append(int(sys.argv[i]))

prop_cycle = plt.rcParams['axes.prop_cycle']
colors = prop_cycle.by_key()['color']
patterns = ('/', '\\', '|', '-', '+', 'x', 'o', 'O', '.', '*')
n = 0
for i in dimYs:
    color = colors[n % len(colors)]
    pattern = patterns[n % len(patterns)]
    n+=1

    label = columns[2 * i]
    x1 = data[columns[2 * dimX]]
    x2 = data[columns[2 * dimX + 1]]
    x = alternativeMerge(x1,x2)
    y1 = double(data[label])
    y2 = double(data[columns[2 * i + 1]])
    p = plt.fill_between(x, y1, y2, label=label, hatch=pattern, alpha = 0.5, edgecolor=color)
    plt.plot(x, y1, c=color)
    plt.plot(x, y2, c=color)

plt.xlabel(columns[2 * dimX])
plt.legend(fontsize = 'x-large')
plt.savefig("img.pdf")
plt.show()
