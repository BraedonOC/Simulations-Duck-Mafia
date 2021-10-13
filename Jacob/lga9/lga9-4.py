#!/usr/bin/env python3

import sys

def getTokens( inf ) :
    # word up!
    for lt in [ line.split() for line in inf ] :
        if not lt :
            continue
        for t in lt :
            yield t

data = open( sys.argv[1] if len(sys.argv) > 1 else "/dev/null" )

tokens = iter(getTokens(data))


i = 0
s = 0

# read in numbers to an array since we're doing 2 calculations
data = []
try:
    while True:
        data.append(float(next(tokens)))
except StopIteration:
    pass
except :
    raise

def two_pass():
    squared_sum = 0
    x_bar = 0
    for x_i in data:
        # First we have to calculate the mean each pass (hence 2-pass)
        sum = 0
        for x in data:
            sum += x
        x_bar = sum/len(data)

        # Now we can calculate the square differences
        squared_sum += (x_bar - x_i)**2

    return (x_bar, squared_sum/len(data))

def one_pass():
    # Now we can find X-bar before doing anything
    sum = 0
    for x in data:
        sum += x
    x_bar = sum / len(data)

    # Then we sum the square of the x_i
    sum = 0
    for x_i in data:
        sum += x_i**2

    # And calculate the difference
    s_sq = sum/len(data)-x_bar**2
    return (x_bar, s_sq)


t_p = two_pass()
o_p = one_pass()

print("two-pass       ", round(t_p[0],8) , "   ", round(t_p[1]**(1/2), 8))
print("one-pass       ", round(o_p[0],8) , "   ", round(o_p[1]**(1/2), 8))