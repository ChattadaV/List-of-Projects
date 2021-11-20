from pylab import *
import matplotlib.pyplot as plt


s, i = meshgrid(arange(0, 100, 0.1), arange(0, 100, 0.1))

k = 30
b = 0.85
n = 3.1

r = ( b*(k**n) ) / ( (i**n) + (k**n) )
y = arange(0, 100, 0.1)

sDot = -r*s*i
iDot = (r*s*i) - (y*i)

streamplot(s, i, sDot, iDot)
show()



