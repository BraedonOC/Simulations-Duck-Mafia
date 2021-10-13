from matplotlib import pyplot as plt

counts = [33, 10, 41, 71, 12, 18, 7, 18]
bins = range(1, 9)

cum = []
for i in range(len(counts)):
    ble = 0
    for j in range(i):
        ble += counts[j]
    cum.append(ble)

# counts = {1: 33, 2: 10, 3: 41, 4: 71, 5: 12, 6: 18, 7: 7, 8: 18}


plt.bar(bins, cum) 
plt.show()
