#!/usr/bin/python3 -u

import os
import sys

args = sys.argv
if (len(args) != 2):
    sys.exit('USAGE: number_of_threads\n')

# test a bug-free microbenchmark
print('atomic_count.c')
print('undirect_power_law_200n_400e.egr')
os.system('gcc -fopenmp -O3 -g -Iinclude -fsanitize=thread OpenMP/atomic_count/atomic_count.c -o microbenchmark')
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])

# test a microbenchmark that has atomic bug
print('atomic_count_atomicBug.c')
print('undirect_power_law_200n_400e.egr')
os.system('gcc -fopenmp -O3 -Iinclude -fsanitize=thread OpenMP/atomic_count/atomic_count_atomicBug.c -o microbenchmark')
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/power_law/undirect_power_law_200n_400e.egr %s' % args[1])

# test a microbenchmark that has memory bound bug
# it may or may not cause out-of-bound memory access depending on the input
print('atomic_count_neighbor_cond_boundsBug.cpp')
print('binary_forest_31n_30e.egr')
os.system('gcc -fopenmp -O3 -Iinclude -fsanitize=thread OpenMP/atomic_count_neighbor_cond/atomic_count_neighbor_cond_boundsBug.c -o microbenchmark')
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/binary_forest/binary_forest_31n_30e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/binary_forest/binary_forest_31n_30e.egr %s' % args[1])
os.system('TSAN_OPTIONS="suppressions=tsan_suppress.txt" ./microbenchmark inputs/binary_forest/binary_forest_31n_30e.egr %s' % args[1])
