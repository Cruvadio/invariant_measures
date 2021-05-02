from itertools import islice
from random import random
from time import perf_counter
from calculations import fast_start
import numpy as np
import matplotlib.pyplot as plt


def format_func_x(value):
    return -2 + value * 0.005


def format_func_y(value):
    return 2 - value * 0.005

fig = plt.figure()
ax = plt.axes(projection="3d")

x_max = 2
x_min = -2

delta = 0.005

iter_loc = 1
iter_bal = 100

a = 0.0
b = -0.2


cols = (int)((x_max - x_min) / delta);


print("Start calculating....")
measures = fast_start(delta, a, b, iter_loc, iter_bal)
print("Finished calculating.")
num = len(measures)

x_pos = [0] * num
y_pos = [0] * num
z_pos = [0] * num

x_size = np.ones(num)
y_size = np.ones(num)
z_size = [0] * num

for i in range(num):
    x_pos[i] = format_func_x(i/cols)
    y_pos[i] = format_func_y(i%cols)
    z_size[i] = measures[i]


pnt3d=ax.bar3d(x_pos, y_pos, z_pos,x_size, y_size, z_size)
#ax.set_zlim3d(0,0.1)
ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func_x))
ax.yaxis.set_major_formatter(plt.FuncFormatter(format_func_y))
ax.grid(False)
ax.set_frame_on(False)
#plt.hist(x_pos, orientation="horizontal")
plt.show()
