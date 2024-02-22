#!/usr/bin/python3 -u

import os
import sys

# read the command line
args = sys.argv
if (len(args) != 3):
    sys.exit('USAGE: threads_per_block number_of_blocks\n')

code_path = os.getcwd() + '/CUDA/'
input_path = os.getcwd() + '/inputs/'
walk_code = os.walk(code_path)
walk_graph = os.walk(input_path)

input_subdir = []
code_subdir = []

for code_root, code_dirs, code_files in walk_code:
    for subdir in code_dirs:
        code_subdir.append(subdir)

for graph_root, graph_dirs, graph_files in walk_graph:
    for subdir in graph_dirs:
        input_subdir.append(subdir)

for subdir in code_subdir:
    test_path = code_path + subdir
    walk_test = os.walk(test_path)
    for root, dircs, code_files in walk_test:
        for code_file in code_files:
            if code_file.endswith('.cu'):
                kernel_path = test_path + '/' + code_file
                print('\ncompile : %s\n' % code_file)
                os.system("nvcc -Iinclude %s -O3 -o microbenchmark" % kernel_path)
                for graph_dir in input_subdir:
                    walk_graph = os.walk(os.path.join(input_path, graph_dir))
                    for root, dircs, graph_files in walk_graph:
                        for graph_file in graph_files:
                            if graph_file.endswith('.egr'):
                                graph_path = os.path.join(input_path, graph_dir, graph_file)
                                print('\ninput: %s\nmicrobenchmark: %s\n' % (graph_file, code_file))
                                print('threads_per_block: %s\nnumber_of_blocks: %s\n' % (args[1], args[2]))
                                os.system('cuda-memcheck --tool memcheck ./microbenchmark %s %s %s' % (graph_path, args[1], args[2]))
                                os.system('cuda-memcheck --tool initcheck ./microbenchmark %s %s %s' % (graph_path, args[1], args[2]))
                                os.system('cuda-memcheck --tool synccheck ./microbenchmark %s %s %s' % (graph_path, args[1], args[2]))
                                sys.stdout.flush()
                if os.path.isfile('microbenchmark'):
                    os.system('rm microbenchmark')
