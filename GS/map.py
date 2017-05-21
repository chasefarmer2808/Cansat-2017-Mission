import csv
import math
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

def get_map_x_y(speed, heading):
    x = float(speed) * math.cos(math.radians(float(heading)))
    y = float(speed) * math.sin(math.radians(float(heading)))
    return [x,y]

plt.ion()

while True:
    try:
        data = open('pos.csv', "rb")
        reader = csv.reader(data)
        reader = list(reader)

        if len(reader) == 1:
            coords = get_map_x_y(reader[0][0], reader[0][1])
            currx = coords[0]
            curry = coords[1]
        else:
            prev_coords = get_map_x_y(reader[-2][0], reader[-2][1])
            curr_coords = get_map_x_y(reader[-1][0], reader[-1][1])

            currx = prev_coords[0] + curr_coords[0]
            curry = prev_coords[1] + curr_coords[1]

        colors = cm.rainbow(np.linspace(0, 1, len(reader)))
        data.close()

    except IOError:
        print('Error opening CSV')

    plt.scatter(currx, curry, c=colors, alpha=0.5)
    plt.draw()
    plt.pause(0.1)
