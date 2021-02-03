import numpy as np
import matplotlib.pyplot as plt
f = open("../txt/kinematics.txt", "r")
lines = f.readlines()
n = len(lines)
dist = []
vel = []
for line in lines:
  inp = line.split()
  dist.append(float(inp[0]))
  vel.append(float(inp[1]))

fig = plt.figure()
# plt.plot(range(n), dist, color = 'red', marker = '.', alpha = 0.5, label = "dist")
# plt.plot(range(n), vel, color = 'blue', marker = '.', alpha = 0.5, label = "vel")
plt.plot(dist, vel, color ='red', marker = '.', label = "vel")
# plt.plot(range(n), curv, color = 'green', marker = '.', alpha = 0.5, label = "curv")
plt.legend(loc="upper left")
plt.title('kinematics')
plt.grid()
plt.xlabel('wp')
plt.ylabel('dist/vel')
plt.show()