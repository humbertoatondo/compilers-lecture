#!/usr/bin/python3

import sys
import re

def filter_list(lines_list):
	instructions_list = []
	functions_list = []
	for l in lines_list:
		if len(l) > 0 and l[0] == " ":
			new_line = clean_instructions(l)
			if len(new_line) > 3: instructions_list.append(new_line)
		elif len(l) > 0 and l[0] == "0":
			new_line = clean_functions(l)
			functions_list.append(new_line)
	return (instructions_list, functions_list)


def clean_instructions(line):
	new_line = line.replace("\n", "")
	return re.split(r"\t|\s{2,}|\s[\*|\-|\%|\n]", new_line)

def clean_functions(line):
	new_line = line.translate({ord(i): None for i in '<>\n:'})
	return re.split(r"\s", new_line)


def count_instructions(instructions_list):
	dict = {}
	for instruction in instructions_list:
		instruction = re.split(r"\s\d", instruction[3])[0]
		if instruction in dict:
			dict[instruction] += 1
		elif instruction != "\s":
			dict[instruction] = 1

	print("\n    You have %s kind of instructions in this object file:" %len(dict))
		
	for element in dict:
		print("        %-25s : Executed %-3s times" %(element, dict[element]))


def count_functions(functions_list):
	print("\n    You have", len(functions_list), "functions:")
	for function in functions_list:
		print("        %-25s : Located at %s addr" %(function[1], function[0]))

def main():
	try:
		file = open(sys.argv[1], "r")
	except:
		print("Please provide a file.")
		sys.exit()

	lines = file.readlines()
	
	instructions, functions = filter_list(lines)

	
	print("Hi, this is the output of the analysis:")
	count_instructions(instructions)
	count_functions(functions)
	
	file.close()	
	

if __name__ == "__main__":
	main()
