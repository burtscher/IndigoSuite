#!/usr/bin/python3 -u

from random import sample
import itertools
import math
import sys
import os
import re

# range of number of vertices and edges
global numv_str
global nume_str
numv_str = '-DNUMV=\\"'
nume_str = '-DNUME=\\"'

# read the configuration file
def read_configure(cfile):
	input_file = open(cfile, 'r')
	lines = input_file.readlines()
	codek = ['direction:', 'pattern:', 'rangeNumV:', 'rangeNumE:' , 'samplingRate:']
	num_codek = 5
	input_c = [[] for i in range(num_codek)]
	# read the code configure
	readc = False
	readi = False
	for l in lines:
		if '**/' in l:
			readc = True
		elif readc and ('INPUTS' in l):
			readi = True;
		if readi:
			for r in ((' ', ''), ('{', ''), ('}', ''), ('\t', ''), ('\n', '')):
				l = l.replace(*r)
			for k in codek:
				if k in l:
					idx = codek.index(k)
					l = l.replace(k, '')
					for i in l.split(','):
						input_c[idx].append(i)

	return input_c

# pattern filter
def filter_pat(pat_c, file_name):
	if (len(pat_c) == 1) and ('all_possible' not in pat_c) and ('all' in pat_c):
		return True
	for c in pat_c:
		if '~' in c:
			c = c.replace('~', '')
			return (c not in file_name)
		if c in file_name:
			return True
	return False

# graph size filter
def filter_gsize(vrange, erange, graph_size, rate, direction):
	global nume_str
	global numv_str
	ge_size = []
	flag = True
	for i in graph_size:
		filter_v = False
		filter_e = False
		numv = i[0]
		nume = i[1]
		for v in vrange:
			v = v.replace('inf', str(99999))
			v = v.split('-')
			if len(v) == 1:
				if flag:
					#numv_file.write('%s\n%s\n' % (v[0], v[0]))
					t_str = numv_str
					add_str = v[0] + '@' + v[0] + '@'
					numv_str = t_str + add_str
				if int(v[0]) == numv:
					filter_v = True
			elif len(v) == 2:
				if flag:
					t_str = numv_str
					add_str = v[0] + '@' + v[1] + '@'
					numv_str = t_str + add_str
				if (numv >= int(v[0])) and (numv <= int(v[1])):
					filter_v = True
		for e in erange:
			e = e.replace('inf', str(99999))
			e = e.split('-')
			if len(e) == 1:
				if flag:
					t_str = nume_str
					add_str = e[0] + '@' + e[0] + '@'
					nume_str = t_str + add_str
				if (int(e[0]) == nume) and ((direction == 0) or (direction == 2)):
					filter_e = True
				elif (int(e[0]) == 2 * nume) and ((direction == 0) or (direction == 1)):
					filter_e = True
			elif len(e) == 2:
				if flag:
					t_str = nume_str
					add_str = e[0] + '@' + e[1] + '@'
					nume_str = t_str + add_str
				if (nume >= int(e[0])) and (nume <= int(e[1])) and ((direction == 0) or (direction == 2)):
					filter_e = True
				elif (2 * nume >= int(e[0])) and (2 * nume <= int(e[1])) and ((direction == 0) or (direction == 1)):
					filter_e = True
		flag = False
		if filter_v and filter_e:
			ge_size.append(i)
	if len(rate) != 1:
		sys.exit('ERROR, have to choose a sampling rate.\n')
	rate = float(rate[0].replace('%', '')) / float(100)
	k = int(math.ceil(len(ge_size) * rate))
	return sample(ge_size, int(k))

############## Usage of each graph generator ######################
# all_possible_graphs:	number_of_vertices, direction
# DAG: 					number_of_vertices, number_of_edges, direction, random_seed
# k_max_degree: 		number_of_vertices, number_of_edges, direction, random_seed, max_degree
# power_law: 			number_of_vertices, number_of_edges, direction, random_seed
# uniform_degree: 		number_of_vertices, number_of_edges, direction, random_seed
# binary_forest: 		number_of_vertices, direction, random_seed
# binary_tree: 			number_of_vertices, direction, random_seed
# k_dim_grid:			number_of_vertices, direction, random_seed, dimension, dimension_size
# k_dim_rand_grid:		number_of_vertices, direction, random_seed, dimension, dimension_size
# k_dim_torus:			number_of_vertices, direction, random_seed, dimension, dimension_size
# k_dim_rand_torus:		number_of_vertices, direction, random_seed, dimension, dimension_size
# rand_neighbor:		number_of_vertices, direction, random_seed
# simple_planar: 		number_of_vertices, direction, random_seed
# star: 				number_of_vertices, direction, random_seed

# the master list generates the above inputs except the all possible graphs
graph_size = [
# NumV | NumE | rand_seed | max_degree | dimension | dimension size
     [5, 	 5, 		 1, 		  1, 		  1, 			5],
     [8, 	10,          2,           2,          2,            4],
    [10,    15,          1,           3,          2,            3],
    [10,    20,          2,           3,          3,            2],
    [10,    30,          3,           3,          3,            3],
    [29,    29,          1,           2,          1,           29],
    [31,    62,          2,           4,          1,           31],
    [31,    93,          3,           8,          2,            5],
    [32,    32,          1,           2,          1,           32],
    [32,    64,          2,           4,          1,           33],
    [32,   128,          3,           8,          2,            6],
    [36,    35,          1,           2,          1,           35],
    [36,    70,          2,           5,          1,           36],
	[36,   140,          3,          16,          1,           37],
    [50,    50,          1,           2,          1,           50],
    [50,   200,          2,           4,          1,           51],
    [50,   400,          3,          16,          2,            7],
    [64,    64,          1,           2,          1,           64],
    [64,   128,          2,           4,          2,            8],
    [64,   256,          3,          16,          3,            4],
    [81,    82,          1,           2,          1,           80],
    [81,   164,          2,           4,          1,           81],
    [81,   246,          3,          16,          2,            9],
   [100,   100,          1,           2,          1,          100],
   [100,   200,          2,          16,          2,           10],
   [100,   500,          3,          20,          3,            5],
   [200,   200,          1,           2,          1,          200],
   [200,   400,          2,          16,          2,           15],
   [200,  1000,          3,          32,          3,            6],
   [500,   500,          1,           2,          1,          500],
   [500,  1000,          2,          16,          2,           20],
   [500,  2000,          3,          32,          3,            7],
   [773,  2100,          2,          16,          2,           19]
]

# read the command line
args = sys.argv
if (len(args) != 2):
	sys.exit('USAGE: output_path\n')

# change the save path
cur_path = os.getcwd()
out_path = args[1]
#p_path = os.path.join(cur_path, 'graph_generators')

# read configuration file
input_c = read_configure('configure.txt')

# direction filter
direction = 0 # directed and undirected
if (len(input_c[0]) != 1):
	sys.exit('ERROR, please select from /* all, directed, undirected */')
else:
	if 'undirected' in input_c[0]:
		direction = 1
	elif 'directed' in input_c[0]:
		direction = 2

# graph size filter
ge_size = filter_gsize(input_c[2], input_c[3], graph_size, input_c[4], direction)
numv_str += '\\"'
nume_str += '\\"'

# generate all possible graphs
k = 4 # max number of vertices for all possible graphs
file_name = 'all_possible_graphs.cpp'
file_path = os.path.join(cur_path, 'graph_generators', 'all_possible', file_name)
save_path = os.path.join(out_path, file_name.replace('.cpp', ''))
os.system('g++ -Igraph_include %s %s -O3 --std=c++11 %s -o graphGenerator' % (nume_str, numv_str, file_path))
if not os.path.exists(save_path):
	os.chdir(out_path)
	os.mkdir(save_path)
	os.chdir(cur_path)
for i in range(0, k):
	p_path = os.path.join(cur_path, 'graphGenerator')
	os.chdir(save_path)
	os.system('%s %s %s' % (p_path, k, str(direction)))

# iterate the dynamic_graph directory
dg_path = os.path.join(cur_path, 'graph_generators', 'dynamic_graphs')
walk_dg = os.walk(dg_path)
for dg_root, dg_dirs, dg_files in walk_dg:
	for dg_code in dg_files:
		if dg_code.endswith('.cpp'):
			file_path = os.path.join(dg_path, dg_code)
			save_path = os.path.join(out_path, dg_code.replace('.cpp', ''))
			f_pattern = filter_pat(input_c[1], dg_code)
			if f_pattern:
				if not os.path.exists(save_path):
					os.chdir(out_path)
					os.mkdir(save_path)
					os.chdir(cur_path)
				os.system('echo %s' % dg_code)
				os.system('g++ -Igraph_include %s %s -O3 --std=c++11 %s -o graphGenerator' % ( nume_str, numv_str, file_path))
				p_path = os.path.join(cur_path, 'graphGenerator')
				os.chdir(save_path)
				for i in ge_size:
					os.system('%s %s %s %s %s %s' % (p_path, str(i[0]), str(i[1]), str(direction), str(i[2]), str(i[3])))
				os.chdir(cur_path)

# iterate the static_graph directory
sg_path = os.path.join(cur_path, 'graph_generators', 'static_graphs')
walk_sg = os.walk(sg_path)
has_all_possible_graphs = False
for sg_root, sg_dirs, sg_files in walk_sg:
	for sg_code in sg_files:
		if sg_code.endswith('.cpp'):
			file_path = os.path.join(sg_path, sg_code)
			save_path = os.path.join(out_path, sg_code.replace('.cpp', ''))
			f_pattern = filter_pat(input_c[1], sg_code)
			if f_pattern:
				if not os.path.exists(save_path):
					os.chdir(out_path)
					os.mkdir(save_path)
					os.chdir(cur_path)
				os.system('echo %s' % sg_code)
				os.system('g++ -Igraph_include %s %s -O3 --std=c++11 %s -o graphGenerator' % (nume_str, numv_str, file_path))
				p_path = os.path.join(cur_path, 'graphGenerator')
				os.chdir(save_path)
				for i in ge_size:
					os.system('%s %s %s %s %s %s' % (p_path, str(i[0]), str(direction), str(i[2]), str(i[4]), str(i[5])))
				os.chdir(cur_path)

# remove the executable
os.system('rm graphGenerator')

# copy the pictures of the graphs
p_path = os.path.join(cur_path, 'graph_generators')
walk_p = os.walk(p_path)
for p_root, p_dirs, p_files in walk_p:
	for p_file in p_files:
		if p_file.endswith('.pdf'):
			in_file = os.path.join(p_path, p_file)
			os.system('cp %s %s' % (in_file, out_path))