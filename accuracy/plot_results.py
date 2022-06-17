import csv
from matplotlib import pyplot as plt

x_size = 4
y_size = 4
z_size = 13

data_x = [[[] for x in range(x_size)] for y in range(y_size)]
data_y = [[[] for x in range(x_size)] for y in range(y_size)]

with open('res.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    i = 0
    for row in reader:
        x = int(row[0]) - 1
        y = int(row[1]) - 1
        data_x[x][y].append(int(row[2]))
        data_y[x][y].append(int(row[3]))
        i += 1

print(data_x)
print(data_y)

plt.rcParams["figure.figsize"] = (20, 20)

fig, axs = plt.subplots(x_size, y_size)

for i in range(x_size):
    for j in range(y_size):
        x = data_x[i][j][3:]
        y = data_y[i][j][3:]
        # x = data_x[i][j]
        # y = data_y[i][j]
        axs[i, j].plot(x, y, marker='o')
        axs[i, j].set_title('z={}, m={}'.format(i+1, j+1))
        axs[i, j].grid()
        axs[i, j].set_xlabel('m')
        axs[i, j].set_ylabel('errors')
        for ind, val in enumerate(y):
            axs[i, j].text(x[ind], val+45, "%d" %val, ha="center")

        # plt.setp(axs[i, j], ylim=(-500, 28000))
        plt.setp(axs[i, j], ylim=(-5, 2000))

fig.tight_layout()

# for ax in axs.flat:
#    ax.set(xlabel='x-label', ylabel='y-label')

# Hide x labels and tick labels for top plots and y ticks for right plots.
# for ax in axs.flat:
#    ax.label_outer()

plt.savefig('zoom_plot.png')
