

def calc_height(S, B_j, d):
    count = 0
    for x in S:
        if x >= B_j[0] and x < B_j[1]:
            count += 1
    return count / (d * len(S))

def calc_hist_vals(datafile, k):
    input_data = []
    with open(datafile, 'r') as f:
        for line in f:
            input_data.append(float(line.strip()))
    a = min(input_data)
    b = max(input_data)
    B_width = (b - a) / k
    B_ranges = []
    for i in range(k):
        B_ranges.append([a + i * B_width, a + (i + 1) * B_width])
    B_heights = [] 
    B_segments = [(a, 0)]
    B_midpoints = []
    for B_j in B_ranges:
        bin_height = calc_height(input_data, B_j, B_width)
        B_heights.append(bin_height)
        B_midpoints.append((B_j[1] + B_j[0]) / 2)
        B_segments.append([B_j[0], bin_height])
        B_segments.append([B_j[1], bin_height])


    
    with open(datafile + '.mid', 'w') as f:
        for j in range(k):
            f.write(f'{B_midpoints[j]}\t{B_heights[j]}\n')
            
    with open(datafile + '.sq', 'w') as f:
        for j in range(len(B_segments)):
            f.write(f'{B_segments[j][0]}\t{B_segments[j][1]}\n')

calc_hist_vals("datafile.dat", 5)


