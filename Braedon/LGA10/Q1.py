import random
import sys
import matplotlib.pyplot as plt
from statistics import mean, stdev
seed = 100000
R = random.Random(seed)
def chord_in_circle(N, n, rho):
    # use each seed in the seeds list
    # initialize variables
    Pg = []  # list of probability distance > rho at each replication
    # initialize random number generator
    # randomly generate the angle between the two points n times
    for i in range(n):  # run experiment n times
        ng = 0  # counter of the number of times distance > rho
        for trial in range(N):
            # generate rho and calculate distance
            theta = R.uniform(0, 180)
            if theta > 60:
                ng = ng+1
        Pg.append(ng/N)
#    print(f'N: {N} mean: {mean(Pg)} std dev: {stdev(Pg)}')
    return stdev(Pg)

fig = plt.figure()
ax = fig.add_subplot(111)
s_vals = []  # I assume s is the standard deviation
N_vals = []  # this isn't really necessary 

if len(sys.argv) > 1 and sys.argv[1] == 'n':
    for N in range(8, 160):
        s_vals.append(chord_in_circle(25, N, 10))
        N_vals.append(N)
else:
    for N in range(8, 160):
        s_vals.append(chord_in_circle(N, 100, 10))
        N_vals.append(N)


ax.scatter(N_vals, s_vals)
plt.show()

