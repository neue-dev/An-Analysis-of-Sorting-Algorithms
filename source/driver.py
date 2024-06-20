import os

# Batch testing
# Note that this comes in handy because my OS automatically kills the process if it goes on too long
# By being able to automate the script within a script, the OS thinks a different process is run each time

algos = ''  # The algos to test
out = ''    # The destination

# General test for all algos
algos = 'insertion,selection,heap,merge,smooth,tim'
out= 'results/all-n-100-10k-and-p-0-1.csv'

os.system('./main N=100,250,500,750,1000,2500,5000,7500,10000 P=-0.0000001,-0.1,-0.25,-0.5,-0.75,-0.8,-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
os.system('./main N=100,250,500,750,1000,2500,5000,7500,10000 P=0.0000001,0.1,0.25,0.5,0.75,0.8,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# N test for n^2 algorithms
algos = 'insertion,selection'
out= 'results/n2-n-test.csv'

for i in range(1, 10):
  os.system('./main N={} P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(i * 100, algos, out));
  os.system('./main N={} P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(i * 1000, algos, out));

os.system('./main N=10000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# P test for n^2 algorithms
algos = 'insertion,selection'
out= 'results/n2-p-test.csv'

os.system('./main N=100,500,1000,5000,10000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
os.system('./main N=100,500,1000,5000,10000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

for i in range(-9, 10):
  if i != 0:
    os.system('./main N=100,500,1000,5000,10000 P={} cycles=10 runs=10 algos={} out="{}"'.format(i * 0.01, algos, out));
    os.system('./main N=100,500,1000,5000,10000 P={} cycles=10 runs=10 algos={} out="{}"'.format(i * 0.1, algos, out));

os.system('./main N=100,500,1000,5000,10000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
os.system('./main N=100,500,1000,5000,10000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# # N test for nlogn algorithms
# algos = 'heap,merge,smooth,tim'
# out = 'results/nlogn-n-test.csv'

# os.system('./main N=1000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=2000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=3000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=4000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=5000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=6000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=7000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=8000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=9000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=10000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=20000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=30000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=40000 P=-1.0,-0.667,-0.333,-0.0000001,0.0000001,0.333,0.667,1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=50000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=60000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=70000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=80000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=90000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=-0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=-0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=0.333 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=0.667 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# # P test for nlogn algorithms
# algos = 'heap,merge,smooth,tim'
# out = 'results/nlogn-p-test.csv'

# os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# for i in range(-9, 10):
#   if i != 0:
#     os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P={} cycles=10 runs=10 algos={} out="{}"'.format(i * 0.01, algos, out));
#     os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P={} cycles=10 runs=10 algos={} out="{}"'.format(i * 0.1, algos, out));

# os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P=-1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=1000,2500,5000,10000,15000,20000,25000 P=1.0 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# # Smooth sort vs heap sort
# algos = 'heap,smooth'
# out = 'results/smooth-heap-p-test.csv'

# os.system('./main N=100000 P=-0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));
# os.system('./main N=100000 P=0.0000001 cycles=10 runs=10 algos={} out="{}"'.format(algos, out));

# for i in range(-100, 101):
#   if i != 0:
#     os.system('./main N=100000 P={} cycles=10 runs=10 algos={} out="{}"'.format(i / 100, algos, out));