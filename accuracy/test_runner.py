import subprocess
import csv
import sys

starcode_result_filename = 'starcode_markedup_test_input.txt'
lsh_result_filename = 'result.txt'
w = 110
max_counter = 5

results = []

def write_results():
    file = open('res.csv', 'w+', newline ='')
    with file:
        write = csv.writer(file)
        write.writerows(results)


def get_clusters(filename):
    clusters = []
    with open(filename) as result:
        for line in result:
            current_cluster = set()
            current_line = line.split(',')
            for idx in current_line:
                current_cluster.add(int(idx))
            clusters.append(current_cluster)
    return clusters


def compute_diff():
    starcode_clusters = get_clusters(starcode_result_filename)
    lsh_clusters = get_clusters(lsh_result_filename)
    # print('starcode clusters: {}, lsh clusters: {}'.format(len(starcode_clusters), len(lsh_clusters)))

    total_diff = 0

    for starcode_cluster in starcode_clusters:
        smallest_diff = 1000000
        smallest_idx = -1
        i = 0
        for lsh_cluster in lsh_clusters:
            diff = lsh_cluster.symmetric_difference(starcode_cluster)
            # print(diff)
            # print(len(diff))
            if len(diff) < smallest_diff:
                smallest_diff = len(diff)
                smallest_idx = i
            i += 1
        if len(lsh_clusters) != 0:
            lsh_clusters.pop(smallest_idx)
            total_diff += smallest_diff
        else:
            total_diff += len(starcode_cluster)


    # print(total_diff)
    return total_diff

def process_run(repetitions=3):
    for z in range(1, max_counter):
        for r in range(1, max_counter):
            for m in range(1, 40, 3):
                print('z: {}, r: {}, m: {}'.format(z, r, m))
                largest_repetition_diff = -1
                for i in range(repetitions):
                    subprocess.call(['../lsh', 'test_input.txt', '-z {}'.format(z), '-r {}'.format(r), '-m {}'.format(m), '-w {}'.format(w)])
                    cur_diff = compute_diff()
                    if cur_diff > largest_repetition_diff:
                        largest_repetition_diff = cur_diff
                print('final_diff: {}'.format(largest_repetition_diff))
                results.append([z, r, m, largest_repetition_diff])
        write_results()


process_run(3)
