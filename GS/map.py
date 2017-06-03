import csv
import math
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

def get_map_x_y(speed, heading):
    #get x and y coords from speed and heading
    x = float(speed) * math.cos(math.radians(float(heading))) #x = Acos(theta)
    y = float(speed) * math.sin(math.radians(float(heading))) #y = Asin(theta)
    return [x,y]

plt.ion()

while True:
    try:
        data = open('pos.csv', "rb")
        reader = csv.reader(data)
        reader = list(reader)

        if len(reader) == 1: #only one data point
            coords = get_map_x_y(reader[0][0], reader[0][1]) #get x and y coords

            #currx and curry will be passed into scatter function
            currx = coords[0]
            curry = coords[1]
        else: #current point depends on previous
            #get [n-1] coords
            prev_coords = get_map_x_y(reader[-2][0], reader[-2][1])

            #get [n] coords
            curr_coords = get_map_x_y(reader[-1][0], reader[-1][1])

            #current coord = [n-1] coord + [n] coord
            currx = prev_coords[0] + curr_coords[0]
            curry = prev_coords[1] + curr_coords[1]

        colors = cm.rainbow(np.linspace(0, 1, len(reader)))
        data.close()

    except IOError: #sometimes happends when there is a resourse collision between this program and another getting the csv
        print('Error opening CSV')

    plt.scatter(currx, curry, c='b', alpha=0.5)
    plt.plot(currx, curry, c='r')
    plt.draw()
    plt.pause(0.1)
