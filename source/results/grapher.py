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

# The names of the sorting algorithms we have
SORTERS = [ 'heap sort', 'insertion sort', 'merge sort', 'selection sort', 'smooth sort', 'tim sort' ]

def df_cleanup(df):
  '''
  This function returns a cleaned up version of the dataframe.
  It removes whitespace from the edges of strings.
  '''
  return df.apply(lambda s: s.strip() if isinstance(s, str) else s)

def df_get_np(df):
  '''
  This function returns the unique values of N and P stored by the CSV.
  '''

  return {
    'N_array': df['N'].unique(),
    'P_array': df['P'].unique(),
  }

def df_filter_runs_n(df, n):
  '''
  Returns all entries with the given value of N.
  '''
  return df[df['N'] == n]

def df_filter_runs_p(df, p):
  '''
  Returns all entries with the given value of P.
  '''
  return df[df['P'] == p]

def df_filter_runs_np(df, n, p):
  '''
  Returns all entries for a given (N, P).
  '''
  return df[(df['N'] == n) & (df['P'] == p)]

def df_create_n_graph(df):
  '''
  This function creates a graph of the sort times of each algorithm vs. N.
  '''
  
  # The N and P values to check
  N_array = df_get_np(df)['N_array']
  P_array = df_get_np(df)['P_array']
   
  # We hold P constant for each graph
  # We're only plotting against N here
  for P in P_array:
    
    # The x and y axes
    X = []
    Y = {}
    
    # Init the Y series values
    for sorter in SORTERS:
      Y[sorter] = []

    # For each N we want to plot
    for N in N_array:
      
      # Define the slice of the df we're using
      sub_df = df_filter_runs_np(df, N, P)
      
      # Update the series
      X.append(N)

      for sorter in SORTERS:
        Y[sorter].append(sub_df[sorter].sum() / sub_df[sorter].count())

    # After generating the series data, we plot what we have
    # The first plot is a linear plot
    for sorter in SORTERS:
      plt.plot(X, Y[sorter], label=sorter)

    # Configure the plot then save
    plt.title('Sort Time (ms) vs. N for P=' + str(P))
    plt.legend()
    plt.savefig('./graphs/t-vs-N,for-P=' + str(P) + '.png')
    plt.clf()

    # The second plot is a log vs log plot
    for sorter in SORTERS:
      plt.plot(X, Y[sorter], label=sorter)

    # Configure the plot then save
    plt.title('ln(Sort Time) vs. ln(N) for P=' + str(P))
    plt.xscale('log')
    plt.yscale('log')
    plt.legend()
    plt.savefig('./graphs/lnt-vs-lnN,for-P=' + str(P) + '.png')
    plt.clf()

def df_create_p_graph(df):
  pass

# This is where we store all our dataframes
DF_ALL = df_cleanup(pd.read_csv('./all-n-100-10k-and-p-0-1.csv'))
DF_N2_N_TEST = df_cleanup(pd.read_csv('./n2-n-test.csv'))
DF_N2_P_TEST = df_cleanup(pd.read_csv('./n2-p-test.csv'))
DF_NLOGN_N_TEST = df_cleanup(pd.read_csv('./nlogn-n-test.csv'))
DF_NLOGN_P_TEST = df_cleanup(pd.read_csv('./nlogn-p-test.csv'))

print(df_filter_runs_np(DF_ALL, 10000, 0.5))
print(df_get_np(DF_ALL))

df_create_n_graph(DF_ALL)

# x = np.arange(0, 5, 0.1)
# y = np.sin(x)
# fig, ax = plt.subplots()
# ax.plot(x, y)
# plt.savefig('./graphs/test.png')

