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

x_bar = 0
v_i = 0


v_array = [0]
i = 0
s = 0

data = []
try:
    while True:
        data.append(float(next(tokens)))
except StopIteration:
    pass
except :
    raise

x_bar_array = [data[0]]

for x_i in data:
    # Here I stored the values into an array, it can be done without the array
    # but for the purpose of dynamic programming I've left them in
    i += 1
    # Update x_bar and v_i
    x_bar = x_bar + 1/i*(x_i-x_bar)
    x_bar_array.append(x_bar)

    v_i = v_i + (i-1)/i*(x_i-x_bar_array[i-1])**2
    v_array.append(v_i)
    s = (v_i/i)**(1/2)

print(i, "  x_bar:  ", round(x_bar, 8), "   v_i:   ", round(v_i, 8), "  s:  ", round(s,8))