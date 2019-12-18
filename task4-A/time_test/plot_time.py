#!/usr/bin/python3

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv('time.txt', sep=' ', names=['x', 'y'])
x = data['x'].values
y = data['y'].values
p = np.poly1d(np.polyfit(x, y, 2))
xp = np.linspace(x[0], x[-1], 10 * (x[-1] - x[0]))
plt.plot(x, y)
plt.plot(xp, p(xp), '--')
plt.text(10, 1000, str(round(p.c[0], 2)) + "x^2 + " +
    str(round(p.c[1], 2)) + "x + " + str(round(p.c[2], 2)))
plt.title('Division time')
plt.xlabel('n')
plt.ylabel('Time, ms')
plt.savefig('plot.png', dpi=300)
plt.show()
