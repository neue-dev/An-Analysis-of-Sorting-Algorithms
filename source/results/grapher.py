'''
 # @ Author: Mo David
 # @ Create Time: 2024-06-17 21:23:08
 # @ Modified time: 2024-06-17 21:23:12
 # @ Description:

 This file generates different graphs for all our datasets.
 '''

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def df_cleanup(df):
  '''
  This function returns a cleaned up version of the dataframe.
  It removes whitespace from the edges of strings.
  '''
  return df.apply(lambda s: s.strip() if isinstance(s, str) else s)

# This is where we store all our dataframes
DF_ALL = df_cleanup(pd.read_csv('./all-n-100-10k-and-p-0-1.csv'))
DF_N2_N_TEST = df_cleanup(pd.read_csv('./n2-n-test.csv'))
DF_N2_P_TEST = df_cleanup(pd.read_csv('./n2-p-test.csv'))
DF_NLOGN_N_TEST = df_cleanup(pd.read_csv('./nlogn-n-test.csv'))
DF_NLOGN_P_TEST = df_cleanup(pd.read_csv('./nlogn-p-test.csv'))

def filter_runs_n(df, n):
  return df[df['N'] == n]

def filter_runs_p(df, p):
  return df[df['P'] == p]

def filter_runs_np(df, n, p):
  out = df[df['N'] == n & df['P'] == p]

print(DF_ALL['N'])
# print(filter_runs_np(DF_ALL, 1000, 0.25))

# x = np.arange(0, 5, 0.1)
# y = np.sin(x)
# fig, ax = plt.subplots()
# ax.plot(x, y)
# plt.savefig('./graphs/test.png')

