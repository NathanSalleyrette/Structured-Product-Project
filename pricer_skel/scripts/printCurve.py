import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

argtitle = sys.argv[1]
x = pd.read_csv("../build/dates.txt", header = None, parse_dates = [0])
if( x[0].dtypes != "double" ):
    pd.to_datetime(x[0])
print(x.dtypes)
for i, arg in enumerate(sys.argv):
    if i >=1:
        data = pd.read_csv(arg, header = None)
        arg = arg[9:len(arg) - 4]
        plt.plot(x[0],data[0], label = arg)
        plt.legend()
plt.xlabel("date")
plt.ylabel("valeur")
plt.title(arg)
plt.show()