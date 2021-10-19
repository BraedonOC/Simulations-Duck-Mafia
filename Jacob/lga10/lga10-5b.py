#!/usr/bin/env python3
import math
import sys
from numpy import random as rand
import numpy    as np
from matplotlib import pyplot as plt

NUM_JOBS = 3000
NUM_SIMULATIONS = 101

def uniform(a,b):
    return rand.uniform(a,b)

def exponent(a):
    return rand.exponential(a)



def simulate():

    tokens = [[0.00000000001, uniform(0.5, 1.5)]]
    for i in range(NUM_JOBS):
        tokens.append([exponent(1) + tokens[i][0], uniform(0.5, 1.5)])


    d_i, c_i = 0., 0.
    d_sum, w_sum, s_sum = 0., 0., 0.
    v_i = 0
    w_i = 0
    w_i_arr = []
    w_bar = 0
    v_array = [0]

    w_bar_array = []
    i = 0

    '''
        This was from lga7
    '''
    for a_i, s_i in tokens:
        if a_i < c_i:
            d_i = c_i - a_i
            d_sum += d_i
        else:
            d_i = 0.

        c_i = a_i + d_i + s_i
        s_sum += s_i
        w_sum += d_i + s_i

        '''
            Here we can calculate the Welford statistics
            w_i is the sojourn time (total time in queue)
            x_bar from previous lga is replaced by w_bar (the metric we're interested in)
        '''
        w_i = d_i + s_i
        w_i_arr.append(w_i)
        i += 1
        # Update w_bar and v_i
        w_bar = w_bar + 1 / i * (w_i - w_bar)
        w_bar_array.append(w_bar)

        v_i = v_i + (i - 1) / i * (w_i - w_bar_array[i - 1]) ** 2
        v_array.append(v_i)
        s = (v_i / i) ** (1 / 2)
    return sum(w_i_arr[-1000:-1])/1000, s


'''
    Our simulation runs 101 times.
    With the first run we calculate a 95% CI for w_bar
    With the final 100 runs we check what percentage actually have w_bar land in the CI
'''
seeds = range(0,NUM_SIMULATIONS)
values = []
start = np.arange(0,NUM_JOBS)
for seed in seeds:
    #rand.seed(seed)
    values.append(simulate())

sim_1 = values.pop(0)
w_bar_1 = sim_1[0]
s_1 = sim_1[1]

lower_bound = w_bar_1 - 1.960*s_1/(math.sqrt(NUM_JOBS - 1))
upper_bound = w_bar_1 + 1.960*s_1/(math.sqrt(NUM_JOBS - 1))
count = 0
'''
    Count is our number of simulations within our 95% CI, we find the frequency and
    hope that it occurs around 95% of the time
'''
for sim in values:
    if sim[0] <= upper_bound and sim[0] >= lower_bound:
        count += 1

print(f"Percentage of simulations within 95% CI: {count/NUM_SIMULATIONS}")

start = np.arange(0,NUM_SIMULATIONS - 1)
plot_values = []
for v in values:
    plot_values.append(v[0])
avg = [values[0][0]]

for i, v in enumerate(values):
    sum = 0
    for x in range(i):
        sum += values[x][0]
    if i != 0:
        avg.append(sum/i)

plt.plot(start,plot_values, "o")
plt.plot(start,avg, "o", color='k')
plt.hlines(upper_bound, 0, NUM_SIMULATIONS, color='r')
plt.hlines(lower_bound, 0, NUM_SIMULATIONS, color='r')
plt.show()

sys.exit(0)

