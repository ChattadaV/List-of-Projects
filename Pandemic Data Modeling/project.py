from pylab import *
import matplotlib.pyplot as plt

#s and i in meshgrid with range from -10 to 100 at step increment of 0.1
s, i = meshgrid(arange(-10, 100, 0.1), arange(-10, 100, 0.1))

#constant values
r = 0.02
y = 0.2

#derivative equations (streamline)
sDot = -r*s*i
iDot = r*s*i-y*i

#plot
streamplot(s, i, sDot, iDot)
show()

