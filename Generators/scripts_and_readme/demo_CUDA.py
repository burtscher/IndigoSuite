#!/usr/bin/python3 -u

import os
import sys

args = sys.argv
if (len(args) != 3):
    sys.exit('USAGE: threads_per_block number_of_blocks\n')

# test a bug-free microbenchmark
print('atomic_count.cu')
print('undirect_power_law_50n_400e.egr')
os.system('nvcc -Iinclude CUDA/atomic_count/atomic_count.cu -o microbenchmark')
os.system('cuda-memcheck --tool memcheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool initcheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool synccheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))

# test a microbenchmark that has atomic bug
print('atomic_count_atomicBug.cu')
print('undirect_power_law_50n_400e.egr')
os.system('nvcc -Iinclude CUDA/atomic_count/atomic_count_atomicBug.cu -o microbenchmark')
os.system('cuda-memcheck --tool memcheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool initcheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool synccheck ./microbenchmark inputs/power_law/undirect_power_law_50n_400e.egr %s %s' % (str(args[1]), str(args[2])))


# test a microbenchmark that has memory bound bug
# it may or may not cause out-of-bound memory access depending on the input
print('atomic_count_neighbor_cond_boundsBug2.cu')
print('1dim_grid_31n_30e.egr')
os.system('nvcc -Iinclude CUDA/atomic_count_neighbor_cond/atomic_count_neighbor_cond_boundsBug2.cu -o microbenchmark')
os.system('cuda-memcheck --tool memcheck ./microbenchmark inputs/k_dim_grid/1dim_grid_31n_30e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool initcheck ./microbenchmark inputs/k_dim_grid/1dim_grid_31n_30e.egr %s %s' % (str(args[1]), str(args[2])))
os.system('cuda-memcheck --tool synccheck ./microbenchmark inputs/k_dim_grid/1dim_grid_31n_30e.egr %s %s' % (str(args[1]), str(args[2])))
