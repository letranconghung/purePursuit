import numpy as np
import matplotlib.pyplot as plt
def draw(i):
  spacing, w_data, w_smooth, tolerance, smo, wps = list(map(float, lines[i].split()))
  xS=[]
  yS=[]
  smo = int(smo)
  wps = int(wps)
  for j in range(smo):
    xval, yval =  list(map(float, lines[i+j+1].split()))
    xS.append(xval)
    yS.append(yval)

  xW = []
  yW = []
  for k in range(wps):
    xval, yval =  list(map(float, lines[i+smo+k+1].split()))
    xW.append(xval)
    yW.append(yval)
  
  title = 'spacing = {}, w_data = {}, w_smooth = {}, tolerance = {}'.format(spacing, w_data, w_smooth, tolerance)
  fig = plt.figure()
  plt.plot(xS, yS, color = 'red', marker = '.', alpha = 0.5)
  plt.plot(xW, yW, color = 'blue', marker = '.', alpha = 0.5)
  plt.title(title)
  plt.grid()
  plt.xlabel('x')
  plt.ylabel('y')
  # plt.show()
  fig.savefig("imgs/{}.png".format(title))
  i += (1 + smo + wps)
  return i


f = open("plot.txt", "r")
lines = f.readlines()
i= 0
print(len(lines))
# while(i < len(lines)):
print(i)
i = draw(i)
