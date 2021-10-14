import math
import matplotlib.pyplot as plt
import sys

technique = sys.argv[1]
filename = sys.argv[2]
points = []
u = []
v = []
n = 0

m = 0
b = 0

with open(filename) as file:
    for line in file:
        x, y = line.strip().split()
        u.append(float(x))
        v.append(float(y))
        n += 1

if technique == "conventional":
    # calc means
    v_bar = sum(v)/n
    u_bar = sum(u)/n
    
    # calc sample std devs
    s_u = 0
    s_v = 0
    for i in range(n):
        s_u += (u[i] - u_bar)**2
        s_v += (v[i] - u_bar)**2
    s_u /= n-1
    s_v /= n-1
    s_u **= 0.5
    s_v **= 0.5

    # calc r from covariance
    cov = 0
    for i in range(n):
        cov += (u[i]-u_bar)*(v[i]-v_bar)
    cov /= n
    r = cov/(s_u*s_v)

    # calc coefficients
    m = r*(s_v/s_u)
    b = v_bar - m*u_bar

    print("m:", m)
    print("b:", b)

elif technique == "MSOD":
    # calc means
    v_bar = sum(v)/n
    u_bar = sum(u)/n

    # calc sample std devs
    s_u = 0
    s_v = 0
    for i in range(n):
        s_u += (u[i] - u_bar)**2
        s_v += (v[i] - u_bar)**2
    s_u /= n-1
    s_v /= n-1
    s_u **= 0.5
    s_v **= 0.5

    # calc cov
    cov = 0
    for i in range(n):
        cov += (u[i]-u_bar)*(v[i]-v_bar)
    cov /= n

    # calc theta
    theta = 0.5*math.atan2(s_u**2 - s_v**2, 2*cov)

    # calc coefficients
    m = math.tan(theta)
    b = v_bar - u_bar*math.tan(theta)

print("m:", m)
print("b:", b)


plt.scatter(u, v)
plt.plot([min(u),max(u)],[m*min(u)+b,m*max(u)+b])
plt.show()