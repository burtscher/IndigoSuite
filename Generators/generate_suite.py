#!/usr/bin/python3 -u

import itertools
import sys
import os
import re

def write_copyright(in_path, out_path, c_lines, in_file_name):
	in_file = open(in_path, 'r')
	in_lines = in_file.readlines()
	in_file.close()
	out_file = open(out_path, 'w')
	idx = 0
	if in_lines[idx].startswith('#!'):
		out_file.write(in_lines[idx] + '\n')
		if in_lines[idx + 1] == '\n':
			idx += 2
		else:
			idx += 1
	if in_file_name.endswith('.py'):
		out_file.write("''' ")
		for l in c_lines:
			out_file.write(l)
		out_file.write(" '''")
	if in_file_name.endswith('.cpp') or in_file_name.endswith('.h'):
		out_file.write('/* ')
		for l in c_lines:
			out_file.write(l)
		out_file.write(' */')
	out_file.write('\n\n')
	for i in range(idx, len(in_lines)):
		out_file.write(in_lines[i])
	out_file.close()

# read the copyright file to determine the version number and suite directory name
current_path = os.getcwd()
copyright_path = os.path.join(current_path, 'copyright.txt')
copyright_file = open(copyright_path, 'r')
c_lines = copyright_file.readlines()
copyright_file.close()
v_list = c_lines[0].split(' ')
vnum = ''
for i in range(0, len(v_list)):
	if v_list[i] == 'version':
		vnum = v_list[i + 1].strip()
		if vnum.endswith('.'):
			vnum = vnum[:-1]
out_directory = 'IndigoSuite' + '_' + vnum

# input path
cuda_path = os.path.join(current_path, 'CUDA_sources')
openmp_path = os.path.join(current_path, 'OpenMP_sources')
include_path = os.path.join(current_path, 'sources_include')
script_path = os.path.join(current_path, 'scripts_and_readme')

# output path
suite_out = os.path.join(current_path, out_directory)
graph_out = os.path.join(suite_out, 'inputs')
cuda_out = os.path.join(suite_out, 'CUDA')
openmp_out = os.path.join(suite_out, 'OpenMP')
include_out = os.path.join(suite_out, 'include')

# make new directory if it does not exist
if not os.path.exists(suite_out):
	os.mkdir(suite_out)
	os.chdir(suite_out)
	os.mkdir(cuda_out)
	os.mkdir(openmp_out)
	os.mkdir(include_out)
	os.mkdir(graph_out)
	os.chdir(current_path)

# write the copyright to the script files and put in suite
walk_script = os.walk(script_path)
for root, directories, files in walk_script:
	for script_file in files:
		script_source = os.path.join(script_path, script_file)
		if script_file.endswith('.py'):
			save_script = os.path.join(suite_out, script_file)
			write_copyright(script_source, save_script, c_lines, script_file)
		else:
			os.system('cp %s %s' % (script_source, suite_out))

# write the copyright to the include files and put in suite
walk_include = os.walk(include_path)
for root, directories, files in walk_include:
	for include_file in files:
		include_source = os.path.join(include_path, include_file)
		save_include = os.path.join(include_out, include_file)
		write_copyright(include_source, save_include, c_lines, include_file)

# generate all the cuda codes
walk_cuda = os.walk(cuda_path)
for root, dirctories, files in walk_cuda:
	for cuda_file in files:
		if (cuda_file.endswith('.idg')):
			cuda_file_path = os.path.join(cuda_path, cuda_file)
			os.system('chmod +x generate_CUDA_codes.py')
			os.system('python3 generate_CUDA_codes.py %s %s %s' % (cuda_file_path, copyright_path, cuda_out))

# generate all the openmp codes
walk_openmp = os.walk(openmp_path)
for root, dirctories, files in walk_openmp:
	for openmp_file in files:
		if (openmp_file.endswith('.idg')):
			openmp_file_path = os.path.join(openmp_path, openmp_file)
			os.system('chmod +x generate_OpenMP_codes.py')
			os.system('python3 generate_OpenMP_codes.py %s %s %s' % (openmp_file_path, copyright_path, openmp_out))

# generate graphs
os.system('chmod +x generate_graphs.py')
os.system('./generate_graphs.py %s' % graph_out)

zip_filename = out_directory + '.zip'
tgz_filename = out_directory + '.tgz'
# compress the suite
os.system('zip -r %s %s' % (zip_filename, out_directory))
os.system('tar -czf %s %s' % (tgz_filename, out_directory))
