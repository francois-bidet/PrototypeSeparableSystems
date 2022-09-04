#!/usr/bin/python3

def alternativeMerge(lst1, lst2):
    return [lst[i] for i in range(len(lst2)) for lst in [lst1,lst2]]

def double(lst):
    return [v for v in lst for j in range(2)]

def createBox(x1,x2,y1,y2):
    x = [x1,x1,x2,x2,x1]
    y = [y1,y2,y2,y1,y1]
    return x,y

import sys

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from matplotlib.patches import Patch

filename = "data.dat"
if len(sys.argv) > 1:
    filename = sys.argv[1]

data = pd.read_csv(filename, sep='\t')
print(data)

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
    y1 = data[label]
    y2 = data[columns[2 * i + 1]]
    boxes = [createBox(a,b,c,d) for a,b,c,d in zip(x1,x2,y1,y2)]
    first = True
    for x,y in boxes:
        if first:
            first = False
            plt.plot(x,y,c=color,label=label)
        plt.plot(x,y,c=color)

plt.xlabel(columns[2 * dimX])
if len(dimYs) == 1:
    plt.ylabel(columns[2*dimYs[0]])
plt.legend(fontsize = 'x-large')
plt.savefig("img.pdf")
plt.show()
