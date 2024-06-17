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
SORTERS = [ 'insertion sort', 'selection sort', 'heap sort', 'merge sort', 'smooth sort', 'tim sort' ]
COLORS = { 
  'insertion sort': '#f41214', 
  'selection sort': '#780000', 
  'heap sort': '#89c2d9', 
  'merge sort': '#468faf', 
  'smooth sort': '#014f86', 
  'tim sort': '#012a4a' 
}
BLACK = '#101010'
WHITE = '#efe8d7'
FONT = 'Helvetica'

# Global settings for plt
plt.rcParams['font.family'] = 'Ubuntu'

def plt_new_plot(title, width=960, height=540):
  '''
  Helper function to create a new plot of out taste.
  '''
  dpi = 96
  figure = plt.figure(figsize=(width / dpi, height / dpi), dpi=dpi)
  figure.patch.set_facecolor(WHITE)
  plt.title(title).set_color(BLACK)

def plt_do_plot(X, Y):
  '''
  Plots the data we have for each sorter against the given X axis.
  Note that Y doesn't need to have all sorters, in case we want a filtered graph.
  '''
  for sorter in SORTERS:
    if sorter in Y:
      line, = plt.plot(X, Y[sorter], label=sorter, linewidth=3.0)
      line.set_color(COLORS[sorter])


def plt_style_plot():
  '''
  Styles the plot with our preferred way of design.
  '''

  # Create a legend with no box
  legend = plt.legend()  
  legend.get_frame().set_alpha(0)
  
  for text in legend.get_texts():
    text.set_color(BLACK)

  for line in legend.legendHandles:
    line.set_linewidth(2.0)

  # Style the axes
  # Also, remove the top and right border
  axes = plt.gca()
  
  axes.set_facecolor(WHITE)
  axes.tick_params(axis='x', colors=BLACK)
  axes.tick_params(axis='y', colors=BLACK)
  
  axes.spines['bottom'].set_color(BLACK) 
  axes.spines['left'].set_color(BLACK) 
  axes.spines['top'].set_visible(False) 
  axes.spines['right'].set_visible(False)


def plt_log_axes():
  '''
  Sets the axes of the plot to log scales.
  '''
  plt.xscale('log')
  plt.yscale('log')

def plt_save_plot(filename):
  '''
  Saves the plot with the given name.
  It also closes the plot so we can make a new one.
  '''
  plt.savefig(filename);
  plt.clf();


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
    plt_new_plot('Sort Time (ms) vs. N for P=' + str(P), 600, 600)
    plt_do_plot(X, Y)
    plt_style_plot()
    plt_save_plot('./graphs/t-vs-N,for-P=' + str(P) + '.png')

    # The second plot is a log vs log plot
    plt_new_plot('ln(Sort Time) vs. ln(N) for P=' + str(P), 600, 600)
    plt_do_plot(X, Y)
    plt_log_axes()
    plt_style_plot()
    plt_save_plot('./graphs/lnt-vs-lnN,for-P=' + str(P) + '.png')

def df_create_p_graph(df):
  pass

# This is where we store all our dataframes
DF_ALL = df_cleanup(pd.read_csv('./all-n-100-10k-and-p-0-1.csv'))
DF_N2_N_TEST = df_cleanup(pd.read_csv('./n2-n-test.csv'))
DF_N2_P_TEST = df_cleanup(pd.read_csv('./n2-p-test.csv'))
DF_NLOGN_N_TEST = df_cleanup(pd.read_csv('./nlogn-n-test.csv'))
DF_NLOGN_P_TEST = df_cleanup(pd.read_csv('./nlogn-p-test.csv'))


df_create_n_graph(DF_ALL)

# x = np.arange(0, 5, 0.1)
# y = np.sin(x)
# fig, ax = plt.subplots()
# ax.plot(x, y)
# plt.savefig('./graphs/test.png')

