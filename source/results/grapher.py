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
FONT = 'Ubuntu'

# Global settings for plt
plt.rcParams['font.family'] = FONT
plt.rcParams["legend.loc"] = 'upper left'

def plt_new_plot(title, width=960, height=540):
  '''
  Helper function to create a new plot of out taste.
  '''
  dpi = 96
  figure = plt.figure(figsize=(width / dpi, height / dpi), dpi=dpi)
  figure.patch.set_facecolor(WHITE)
  plt.title(title).set_color(BLACK)

def plt_do_plot_line(X, Y, sorters):
  '''
  Plots the data we have for each sorter against the given X axis.
  Note that Y doesn't need to have all sorters, in case we want a filtered graph.
  '''
  for sorter in sorters:
    if sorter in Y:
      line, = plt.plot(X, Y[sorter], '-.o', label=sorter, linewidth=3.0)
      line.set_color(COLORS[sorter])

def plt_do_plot_scatter(X, Y, sorters):
  '''
  Plots the data we have for each sorter against the given X axis.
  Note that Y doesn't need to have all sorters, in case we want a filtered graph.
  '''
  for sorter in sorters:
    if sorter in Y:
      plt.scatter(X, Y[sorter], label=sorter, color=COLORS[sorter])

def plt_style_plot():
  '''
  Styles the plot with our preferred way of design.
  '''

  # Create a legend with no box
  legend = plt.legend()  
  legend.get_frame().set_alpha(0)
  
  for text in legend.get_texts():
    text.set_color(BLACK)

  for line in legend.legend_handles:
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

def plt_log_x():
  '''
  Sets the y-axis of the plot to log scales.
  '''
  plt.xscale('log')

def plt_log_y():
  '''
  Sets the x-axis of the plot to log scales.
  '''
  plt.yscale('log')

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
  plt.close();

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

  N_array = df['N'].unique()
  P_array = df['P'].unique()

  N_array.sort()
  P_array.sort()

  return {
    'N_array': N_array,
    'P_array': P_array,
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

def df_create_p_lineplot(df, name, sorters, yvar='time'):
  '''
  This function creates a graph of the sort times of each algorithm for a given P.
  '''

  # Define the unit of the title
  unit = '(ms)'
  if yvar == 'freq':
    unit = '(ops)'
  
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
    for sorter in sorters:
      Y[sorter] = []

    # For each N we want to plot
    for N in N_array:
      
      # Define the slice of the df we're using
      sub_df = df_filter_runs_np(df, N, P)
      
      # Update the series
      X.append(N)

      for sorter in sorters:
        Y[sorter].append(sub_df[sorter + ' ' + yvar].sum() / sub_df[sorter + ' ' + yvar].count())

    # After generating the series data, we plot what we have
    # The first plot is a linear plot
    plt_new_plot('Sort ' + yvar + ' ' + unit + ' vs. N for P=' + str(P), 600, 600)
    plt_do_plot_line(X, Y, sorters)
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-t-vs-N,for-P=' + str(P) + '.png')

    # The second plot is a log vs log plot
    plt_new_plot('ln(Sort ' + yvar + ') vs. ln(N) for P=' + str(P), 600, 600)
    plt_do_plot_line(X, Y, sorters)
    plt_log_axes()
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-lnt-vs-lnN,for-P=' + str(P) + '.png')

def df_create_n_lineplot(df, name, sorters, yvar='time'):
  '''
  This function creates a graph of the sort times of each algorithm for a given N.
  '''

  # Define the unit of the title
  unit = 'ms'
  if yvar == 'freq':
    unit = '(ops)'
  
  # The N and P values to check
  N_array = df_get_np(df)['N_array']
  P_array = df_get_np(df)['P_array']
   
  # We hold P constant for each graph
  # We're only plotting against N here
  for N in N_array:
    
    # The x and y axes
    X = []
    Y = {}
    
    # Init the Y series values
    for sorter in sorters:
      Y[sorter] = []

    # For each N we want to plot
    for P in P_array:
      
      # Define the slice of the df we're using
      sub_df = df_filter_runs_np(df, N, P)
      
      # Update the series
      X.append(P)

      for sorter in sorters:
        Y[sorter].append(sub_df[sorter + ' ' + yvar].sum() / sub_df[sorter + ' ' +yvar].count())

    # After generating the series data, we plot what we have
    # The first plot is a linear plot
    plt_new_plot('Sort ' + yvar + ' ' + unit + ' vs. P for N=' + str(N), 600, 600)
    plt_do_plot_line(X, Y, sorters)
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-t-vs-P,for-N=' + str(N) + '.png')

    # The second plot is a log vs log plot
    plt_new_plot('ln(Sort ' + yvar + ') vs. ln(P) for N=' + str(N), 600, 600)
    plt_do_plot_line(X, Y, sorters)
    plt_log_axes()
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-lnt-vs-lnP,for-N=' + str(N) + '.png')

def df_create_entropy_scatterplot(df, name, sorters, yvar='time'):
  '''
  This graphs the runtime of each algorithm with respect to the entropy of the shuffled array.
  We do this per value of N, because the entropy also kinda depends on N.
  '''

  # Define the unit of the title
  unit = 'ms'
  if yvar == 'freq':
    unit = '(ops)'

  # The N values to check
  N_array = df_get_np(df)['N_array']
  
  for N in N_array:

    # Grab the sub df
    sub_df = df_filter_runs_n(df, N)

    # The stuff to plot
    X = []
    Y = {}

    # Define the lists for each of the sorters
    for sorter in sorters:
      Y[sorter] = []

    # Iterate through the rows
    for index, row in sub_df.iterrows():
      
      # Append the current entropy
      X.append(row['entropy'])

      # Append the times of the sorters
      for sorter in sorters:
        Y[sorter].append(row[sorter + ' ' + yvar])

    plt_new_plot('Sort ' + yvar + ' ' + unit + ' vs. Entropy for N=' + str(N), 600, 600)
    plt_do_plot_scatter(X, Y, sorters)
    # plt_log_y()
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-t-vs-entropy,for-N=' + str(N) + '.png')

def df_create_determination_scatterplot(df, name, sorters, yvar='time'):
  '''
  This graphs the runtime of each algorithm with respect to the entropy of the shuffled array.
  We do this per value of N, because the entropy also kinda depends on N.
  '''

  # Define the unit of the title
  unit = 'ms'
  if yvar == 'freq':
    unit = '(ops)'

  # The N values to check
  N_array = df_get_np(df)['N_array']
  
  for N in N_array:

    # Grab the sub df
    sub_df = df_filter_runs_n(df, N)

    # The stuff to plot
    X = []
    Y = {}

    # Define the lists for each of the sorters
    for sorter in sorters:
      Y[sorter] = []

    # Iterate through the rows
    for index, row in sub_df.iterrows():
      
      # Append the current rsquared
      X.append(row['rsquared'])

      # Append the times of the sorters
      for sorter in sorters:
        Y[sorter].append(row[sorter + ' ' + yvar])

    plt_new_plot('Sort ' + yvar + ' ' + unit + ' vs. R^2 for N=' + str(N), 600, 600)
    plt_do_plot_scatter(X, Y, sorters)
    # plt_log_y()
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-t-vs-determination,for-N=' + str(N) + '.png')

def df_create_correlation_scatterplot(df, name, sorters, yvar='time'):
  '''
  This graphs the runtime of each algorithm with respect to the entropy of the shuffled array.
  We do this per value of N, because the entropy also kinda depends on N.
  '''

  # Define the unit of the title
  unit = 'ms'
  if yvar == 'freq':
    unit = '(ops)'

  # The N values to check
  N_array = df_get_np(df)['N_array']
  
  for N in N_array:

    # Grab the sub df
    sub_df = df_filter_runs_n(df, N)

    # The stuff to plot
    X = []
    Y = {}

    # Define the lists for each of the sorters
    for sorter in sorters:
      Y[sorter] = []

    # Iterate through the rows
    for index, row in sub_df.iterrows():
      
      # Append the current rsquared
      X.append(row['r'])

      # Append the times of the sorters
      for sorter in sorters:
        Y[sorter].append(row[sorter + ' ' + yvar])

    plt_new_plot('Sort ' + yvar + ' ' + unit + ' vs. R for N=' + str(N), 600, 600)
    plt_do_plot_scatter(X, Y, sorters)
    # plt_log_y()
    plt_style_plot()
    plt_save_plot('./graphs/' + name + '-t-vs-correlation,for-N=' + str(N) + '.png')
      
# This graphs execution times
for i in range(2):
  
  # This is where we store all our dataframes
  DF_ALL = df_cleanup(pd.read_csv('./all-n-100-10k-and-p-0-1.csv'))
  DF_N2_N_TEST = df_cleanup(pd.read_csv('./n2-n-test.csv'))
  DF_N2_P_TEST = df_cleanup(pd.read_csv('./n2-p-test.csv'))
  DF_NLOGN_N_TEST = df_cleanup(pd.read_csv('./nlogn-n-test.csv'))
  DF_NLOGN_P_TEST = df_cleanup(pd.read_csv('./nlogn-p-test.csv'))
  
  # Invert the datasets
  if i == 1:
    DF_N2_N_TEST = df_cleanup(pd.read_csv('./n2-p-test.csv'))
    DF_N2_P_TEST = df_cleanup(pd.read_csv('./n2-n-test.csv'))
    DF_NLOGN_N_TEST = df_cleanup(pd.read_csv('./nlogn-p-test.csv'))
    DF_NLOGN_P_TEST = df_cleanup(pd.read_csv('./nlogn-n-test.csv'))

  df_create_n_lineplot(DF_ALL, 'n-graphs/time-all', SORTERS)
  df_create_n_lineplot(DF_N2_N_TEST, 'n-graphs/time-n2', SORTERS[:2])
  df_create_n_lineplot(DF_NLOGN_N_TEST, 'n-graphs/time-nlogn', SORTERS[2:])

  df_create_p_lineplot(DF_ALL, 'p-graphs/time-all', SORTERS)
  df_create_p_lineplot(DF_N2_P_TEST, 'p-graphs/time-n2', SORTERS[:2])
  df_create_p_lineplot(DF_NLOGN_P_TEST, 'p-graphs/time-nlogn', SORTERS[2:])

  df_create_entropy_scatterplot(DF_ALL, 'entropy-graphs/time-all', SORTERS);
  df_create_entropy_scatterplot(DF_N2_P_TEST, 'entropy-graphs/time-insertion', SORTERS[0:1]);
  df_create_entropy_scatterplot(DF_N2_P_TEST, 'entropy-graphs/time-selection', SORTERS[1:2]);
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/time-heap', SORTERS[2:3]);
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/time-merge', SORTERS[3:4]);
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/time-smooth', SORTERS[4:5]);
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/time-tim', SORTERS[5:6]);

  df_create_determination_scatterplot(DF_ALL, 'determination-graphs/time-all', SORTERS);
  df_create_determination_scatterplot(DF_N2_P_TEST, 'determination-graphs/time-insertion', SORTERS[0:1]);
  df_create_determination_scatterplot(DF_N2_P_TEST, 'determination-graphs/time-selection', SORTERS[1:2]);
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/time-heap', SORTERS[2:3]);
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/time-merge', SORTERS[3:4]);
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/time-smooth', SORTERS[4:5]);
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/time-tim', SORTERS[5:6]);

  df_create_correlation_scatterplot(DF_ALL, 'correlation-graphs/time-all', SORTERS);
  df_create_correlation_scatterplot(DF_N2_P_TEST, 'correlation-graphs/time-insertion', SORTERS[0:1]);
  df_create_correlation_scatterplot(DF_N2_P_TEST, 'correlation-graphs/time-selection', SORTERS[1:2]);
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/time-heap', SORTERS[2:3]);
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/time-merge', SORTERS[3:4]);
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/time-smooth', SORTERS[4:5]);
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/time-tim', SORTERS[5:6]);

  # This graphs the frequency counts
  df_create_n_lineplot(DF_ALL, 'n-graphs/freq-all', SORTERS, 'freq')
  df_create_n_lineplot(DF_N2_N_TEST, 'n-graphs/freq-n2', SORTERS[:2],'freq')
  df_create_n_lineplot(DF_NLOGN_N_TEST, 'n-graphs/freq-nlogn', SORTERS[2:], 'freq')

  df_create_p_lineplot(DF_ALL, 'p-graphs/freq-all', SORTERS, 'freq')
  df_create_p_lineplot(DF_N2_P_TEST, 'p-graphs/freq-n2', SORTERS[:2], 'freq')
  df_create_p_lineplot(DF_NLOGN_P_TEST, 'p-graphs/freq-nlogn', SORTERS[2:], 'freq')

  df_create_entropy_scatterplot(DF_ALL, 'entropy-graphs/freq-all', SORTERS, 'freq');
  df_create_entropy_scatterplot(DF_N2_P_TEST, 'entropy-graphs/freq-insertion', SORTERS[0:1], 'freq');
  df_create_entropy_scatterplot(DF_N2_P_TEST, 'entropy-graphs/freq-selection', SORTERS[1:2], 'freq');
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/freq-heap', SORTERS[2:3], 'freq');
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/freq-merge', SORTERS[3:4], 'freq');
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/freq-smooth', SORTERS[4:5], 'freq');
  df_create_entropy_scatterplot(DF_NLOGN_P_TEST, 'entropy-graphs/freq-tim', SORTERS[5:6], 'freq');

  df_create_determination_scatterplot(DF_ALL, 'determination-graphs/freq-all', SORTERS, 'freq');
  df_create_determination_scatterplot(DF_N2_P_TEST, 'determination-graphs/freq-insertion', SORTERS[0:1], 'freq');
  df_create_determination_scatterplot(DF_N2_P_TEST, 'determination-graphs/freq-selection', SORTERS[1:2], 'freq');
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/freq-heap', SORTERS[2:3], 'freq');
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/freq-merge', SORTERS[3:4], 'freq');
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/freq-smooth', SORTERS[4:5], 'freq');
  df_create_determination_scatterplot(DF_NLOGN_P_TEST, 'determination-graphs/freq-tim', SORTERS[5:6], 'freq');

  df_create_correlation_scatterplot(DF_ALL, 'correlation-graphs/freq-all', SORTERS, 'freq');
  df_create_correlation_scatterplot(DF_N2_P_TEST, 'correlation-graphs/freq-insertion', SORTERS[0:1], 'freq');
  df_create_correlation_scatterplot(DF_N2_P_TEST, 'correlation-graphs/freq-selection', SORTERS[1:2], 'freq');
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/freq-heap', SORTERS[2:3], 'freq');
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/freq-merge', SORTERS[3:4], 'freq');
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/freq-smooth', SORTERS[4:5], 'freq');
  df_create_correlation_scatterplot(DF_NLOGN_P_TEST, 'correlation-graphs/freq-tim', SORTERS[5:6], 'freq');
