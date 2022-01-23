import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

argtitle = sys.argv[1]

for i, arg in enumerate(sys.argv):
    if i >=1:
        data = pd.read_csv(arg, header = None)
        arg = arg[9:len(arg) - 4]
        plt.plot(data)
argtitle = argtitle[9:len(arg) - 4]
plt.xlabel("Nombre de rebalancement")
plt.ylabel("pnl")
plt.title(arg)
plt.show()