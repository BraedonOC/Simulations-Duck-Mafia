import sys
filename = sys.argv[1]

m = [] # mid points
p = [] # frequencies

with open(filename) as file:
    for line in file:
        line = line.strip().split()
        m.append(float(line[0]))
        p.append(float(line[1]))

n = len(m)
x_bar = 0
for i in range(n):
    x_bar += m[i]*p[i]

s = 0
for i in range(n):
    s += (m[i] - x_bar)**2 * p[i]
s **= 0.5

print("x_bar:", x_bar)
print("s:", s)

