#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

arg = sys.argv[1]
data = pd.read_csv(arg, header = None)
arg = arg[9:len(arg) - 4]
plt.hist(data, density = True)
plt.xlabel("valeur de l'erreur")
plt.ylabel("frequence d'apparition")
plt.title(arg)
plt.show()
