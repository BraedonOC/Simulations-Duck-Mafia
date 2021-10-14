#!/usr/bin/env python3
import math
import sys

midpoints = []
frequencies = []

# Read in histogram data
data = open( sys.argv[1] if len(sys.argv) > 1 else "/dev/null" )
lines = data.read().splitlines()
for line in lines:
    l = line.split()
    midpoints.append(float(l[0]))
    frequencies.append(float(l[1]))


x_bar = 0
var = 0

'''
    X bar can be found by summing up the individual midpoints times their frequencies,
    then dividing by the frequency total.
    Another way to do it is to first find the frequency total and divide each frequency by the
    total frequency to get the probability p. 
'''

for i, m in enumerate(midpoints):
    p = frequencies[i]/sum(frequencies)
    x_bar += m * p
'''
    Similarly for s^2, we find the difference for each midpoint from X bar and square that,
    multiply it be the probability p for that bin, and sum all of the individual variances. 
'''

for i, m in enumerate(midpoints):
    p = frequencies[i]/sum(frequencies)
    var += (m - x_bar)**2 * p

'''
    Before finding s, we must also include our delta (bin-width) since we are only estimating.
    The bin width should be the same as the difference between any 2 midpoints
'''
delta = midpoints[1] - midpoints[0]
s = math.sqrt(var + delta**2/12)

print("x_bar:  ", round(x_bar, 8), "   s:   ", round(s, 8))
