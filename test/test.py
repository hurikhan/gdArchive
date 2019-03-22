#!/usr/bin/env python3

from distutils.dir_util import copy_tree
from subprocess import check_output, TimeoutExpired, CalledProcessError, STDOUT
from sys import argv

import toml


copy_tree("../addons/gdArchive/", "./addons/gdArchive/")

config = toml.load("test.toml")
godot = config['godot']['Server']['64']

tests = [
	"0001_get_version.gd",
	"0002_get_info.gd",
	"0003_open_close.gd",
	"0004_list.gd",
	"0005_extract.gd"
	]

def _result(s1, s2):
	text_len = len(s1)
	padding_len = 50 - text_len

	padding = ""
	for i in range(padding_len):
		padding = padding + "."

	print( s1 + padding + s2 )



for test in tests:
	try:
		ret = ""
		ret = check_output([godot, "--script", test ], stderr=STDOUT, timeout=5)
		ret = ret.decode("utf-8")

		if ret.endswith("\n[TEST OK]\n"):
			_result(test, "[OK]")
		else:
			_result(test, "[FAILED]")

	except TimeoutExpired:
		_result(test, "[TIMEOUT]")
	except CalledProcessError:
		_result(test, "[RETURN != 0]")

	if len(argv) == 2:
		if argv[1] == "-v":
			print(ret)	
