import sys

starcode_result_filename = 'starcode_markedup_test_input.txt'
lsh_result_filename = sys.argv[1]
diff_file = 'lsh_and_starcode_diff.txt'


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

starcode_clusters = get_clusters(starcode_result_filename)
lsh_clusters = get_clusters(lsh_result_filename)
print('starcode clusters: {}, lsh clusters: {}'.format(len(starcode_clusters), len(lsh_clusters)))

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


print(total_diff)


# f = open(output_file_name, 'w')
#
# # for idx in idxs:
# #     print(idx)
#
#
# line_num = 1
# with open(input_filename_lines) as input_file:
#     for line in input_file:
#         # print(line_num)
#         if line_num in idxs:
#             f.write(line)
#
#         line_num += 1
#
# f.close()
