#!/usr/bin/python3 -u

import itertools
import sys
import os
import re

# filter data type
def filter_datat(idg_file, code_file, dataType):
	if 'all' in dataType:
		return True
	datat = ['int', 'short', 'long', 'float', 'double', 'char']
	file_datat = []
	f = code_file.replace(idg_file, '')
	f = code_file.replace(' ', '')
	f = f.split('_')
	for i in f:
		if i in datat:
			file_datat.append(i)
	if len(dataType) == 0:
		sys.exit('ERROR, please select at least 1 data type')
	elif 'all' in dataType:
		return True
	else:
		if (len(file_datat) == 0):
			if 'int' in dataType:
				return True
		else:
			for d in file_datat:
				if ('~' not in dataType) and (d in dataType):
					return True
				elif ('~' in dataType) and (d not in dataType):
					return True
	return False

# filter options
def filter_opt(idg_file, code_file, options):
	if 'all' in options:
		return True
	f = code_file.replace(idg_file, '')
	f = f.split('_')
	f.remove('')
	datat = ['int', 'short', 'long', 'float', 'double', 'char']
	for i in datat:
		if i in f:
			f.remove(i)
	else:
		for b in options:
			if 'only' in b:
				b = b.replace('only_', '')
				if (b in f) and (len(f) == 1):
					return True
			elif '~' in b:
				b = b.replace('~', '')
				if b not in f:
					return True
			else:
				b = b.replace('all_', '')
				if b in f:
					return True
	return False

# read configure file
def read_configure(cfile):
	input_file = open(cfile, 'r')
	lines = input_file.readlines()
	codek = ['bug:', 'pattern:', 'option:', 'dataType:']
	num_codek = 4
	code_c = [[] for i in range(num_codek)]

	# read the code configure
	readc = False
	for l in lines:
		if '**/' in l:
			readc = True
		elif readc:
			for r in ((' ', ''), ('{', ''), ('}', ''), ('\t', ''), ('\n', '')):
				l = l.replace(*r)
			for k in codek:
				if k in l:
					idx = codek.index(k)
					l = l.replace(k, '')
					for i in l.split(','):
						code_c[idx].append(i)
		if readc and ('INPUTS' in l):
			break;
	return code_c

# read the command line
args = sys.argv
if (len(args) <= 3):
	sys.exit('USAGE: input_path copyright_path output_path\n')

# change the save path
cur_path = os.getcwd()
out_path = args[3]
in_file_path = args[1]
save_path = os.path.join(out_path, (os.path.split(in_file_path))[1].replace('.idg', ''))
file_name = (os.path.split(in_file_path))[1].replace('.idg', '')

# read code configuration
code_c = read_configure('configure.txt')

# filter pattern
f_pattern = False
pattern_c = code_c[1]
if len(pattern_c) == 0:
	sys.exit('ERROR, please select at least 1 pattern\n')
elif 'all' in pattern_c:
	f_pattern = True
else:
	for p in pattern_c:
		if p in file_name:
			f_pattern = True

num_codes = 0
num_bugs = 0
if (f_pattern):
	all_tags = [] # a list of tags per line
	input_file = open(in_file_path, 'r')
	lines = input_file.readlines()

	# read code and tags
	for l in lines:
		if (re.search('\/\*\@[a-zA-Z]*[0-9]*\@\*\/', l)): # search the tag
			re_tags = re.findall('\/\*\@[a-zA-Z]*[0-9]*\@\*\/', l)
			re_tags = [substr.replace('/*@', '@') for substr in re_tags]
			re_tags = [substr.replace('@*/', '@') for substr in re_tags]
			line_tags = [substr.replace('@', '') for substr in re_tags]
			first_tag = line_tags[0]
			new_tag = True
			for i in all_tags:
				if i[1] == first_tag:
					new_tag = False
					break
			if new_tag:
				line_tags.insert(0, '')
				all_tags.append(list(line_tags))

	# generate all combinations
	p_all_tags = list(itertools.product(*all_tags))

	# write to output folder
	for i in range(0, len(p_all_tags)):
		out_file_name = file_name # append the tags to the end of file name
		for j in range(0, len(p_all_tags[i])):
			if p_all_tags[i][j]:
				out_file_name = out_file_name + '_' + p_all_tags[i][j].strip()
		buggy = ('bug' in out_file_name) or ('Bug' in out_file_name) or ('race' in out_file_name)

		# filter bug, option, and data type
		ge_all = False
		f_bug = False
		bug_c = code_c[0]
		num_bugc = len(bug_c)
		if (num_bugc == 0) or (num_bugc > 2):
			sys.exit('ERROR, number bug configuration options must be smaller than 3.\n')
		else:
			if (num_bugc == 1):
				if 'hasbug' in bug_c:
					f_bug = True
				elif 'all' in bug_c:
					ge_all = True
				elif 'nobug' not in bug_c:
					sys.exit('ERROR, wrong configuration in bug.\n')
			else:
				ge_all = True
		f_type = filter_opt(file_name, out_file_name, code_c[2]) and filter_datat(file_name, out_file_name, code_c[3])

		# generate code
		if (ge_all or (f_bug == buggy)) and f_type:
			num_codes = num_codes + 1
			if buggy:
				num_bugs = num_bugs + 1
			if not os.path.exists(save_path):
				os.chdir(out_path)
				os.mkdir(save_path)
				os.chdir(cur_path)
			out_file_name = out_file_name + '.c'
			complete_file_name = os.path.join(save_path, out_file_name)
			output_file = open(complete_file_name, 'w')
			num_codes = num_codes + 1
			lspace = 0

			copyright_file = open(args[2], 'r')
			c_lines = copyright_file.readlines()
			output_file.write('/* ')
			for l in c_lines: # write copyright
				output_file.write(l)
			output_file.write(' */\n\n')
			copyright_file.close()

			for l in lines: # write codes
				ostr = ''
				if (re.search('\/\*\@[a-zA-Z]*[0-9]*\@\*\/', l)): # search for the tag
					re_tags = re.findall('\/\*\@[a-zA-Z]*[0-9]*\@\*\/', l)
					re_tags = [substr.replace('/*@', '@') for substr in re_tags]
					re_tags = [substr.replace('@*/', '@') for substr in re_tags]
					line_tags = [substr.replace('@', '') for substr in re_tags]
					line_tags.insert(0, '')
					re_split = re.split('\/\*\@[a-zA-Z]*[0-9]*\@\*\/', l)

					code = ''
					index = 0
					for j in range(0, len(line_tags)):
						for k in p_all_tags[i]:
							if line_tags[j] == k:
								code = re_split[j].strip() # remove the leading and trailing space
								break
					if code:
						ostr = code
				else:
			 		ostr = l.strip()
				if ostr.startswith('}'):
					lspace = lspace - 2
				if ostr:
					output_file.write(' ' * lspace + ostr + '\n')
				elif l == '\n':
					output_file.write(l)
				if ostr.endswith('{'):
					lspace = lspace + 2
			output_file.close()
	input_file.close()
print('Generating: %s' % file_name)
print('Number of OpenMP files generated: %d\n' % num_codes)
print('Number of buggy OpenMP files generated: %d\n' % num_bugs)