from matplotlib import pyplot as plt
import sys

def is_float(n):
    try:
        float(n)
        return True
    except:
        return False

means = []
vars = []
with open(sys.argv[1], 'r') as f:
    count = 0
    for line in f.readlines():
        if count % 2 == 0:
            means.append(float(line))
        else:
            vars.append(float(line))
        count+=1

x = range(0, len(means))
plt.plot(x, means)
plt.xlabel("Number of servers")
plt.ylabel("Probabilty >= n/2 servers are in service")
plt.savefig("plot.jpg")
plt.show()


