import matplotlib
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
matplotlib.use('TkAgg')


image = [[70,80,83,70],
         [74,89,70,69],
         [72,92,81,80],
         [70,92,88,76]]

normalised = [[70,80,83,70],
         [74,89,70,69],
         [72,92,81,80],
         [70,92,88,76]]
# image为正规化之前的图像，normalised为正规化之后的图像

# 获取数组行和列
rows=np.size(image,0)
cols=np.size(image,1)

# 获取原图像的最大像素值和最小像素值
minim=255
maximum=-1
for x in list(range(cols)):
    for y in list(range(rows)):
        if minim > image[x][y]:
            minim = image[x][y]
        if maximum < image[x][y]:
            maximum = image[x][y]

# 计算原图像像素值的差
oldRange = maximum - minim;

# 根据正规化公式进行正规化
for X in list(range(cols)):
    #%address a11 columns
    for y in list(range(rows)):
        #%address all rows
        normalised[y][X] = (int)((image[y][X]-minim)*255/oldRange)

# 输出图像
x, y = np.mgrid[0:4:1, 0:4:1]
z = np.array(normalised)
s = np.array(image)
fig = plt.figure(figsize=(8, 6))
ax = plt.axes(projection="3d")

ax.plot_surface(x, y, z, cmap=cm.ocean)
# 设置Z轴范围
# ax.set_zlim(0, 100)
# ax.plot_surface(x, y, s, cmap=cm.ocean)
plt.show()
