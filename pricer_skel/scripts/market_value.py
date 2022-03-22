import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

#argtitle = sys.argv[1]
x = pd.read_csv("../build/dates.txt", header = None, parse_dates = [0])

# if( x[0].dtypes != "float" or  x[0].dtypes != "int"):
#     pd.to_datetime(x[0])
# print(x.dtypes)
data = pd.read_csv( "../build/marketvalue.txt", header = None,  )

fig, ax = plt.subplots( 8 )

for i in range(8):
    ax[i].plot( data[i] )
    print(data[i] )
plt.show()