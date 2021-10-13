import sys

data = []
with open(sys.argv[1], 'r') as f:
    data = f.readlines()

for i in range(len(data)):
    data[i] = float(data[i][:-1])
calculated_mean = sum(data) / len(data)

v_1 = 0
n = len(data)

for i in range(n):
    v_1 += pow(calculated_mean - data[i], 2)

print(f'two pass: {calculated_mean} {1/len(data) * v_1}')

x_sum = 0
x_2_sum = 0
for i in range(n):
    x_2_sum += 1/n * pow(data[i], 2)
    x_sum += data[i]


print(f'one pass: {x_sum / n} {x_2_sum - pow(x_sum / n, 2)}')




