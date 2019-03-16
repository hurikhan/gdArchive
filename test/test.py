#!/usr/bin/env python3

from distutils.dir_util import copy_tree
from subprocess import check_output, TimeoutExpired, CalledProcessError, STDOUT
from sys import argv

import toml


copy_tree("../addons/gdArchive/", "./addons/gdArchive/")

config = toml.load("test.toml")
godot = config['godot']['Server']['64']

tests = [
	"test_001.gd",
	"test_002.gd",
	"test_003.gd"
	]


for test in tests:
	try:
		ret = ""
		ret = check_output([godot, "--script", test ], stderr=STDOUT, timeout=5)
		ret = ret.decode("utf-8")

		if ret.endswith("\n[TEST OK]\n"):
			print( test + "................................[OK]" )
		else:
			print( test + "................................[FAILED]" )

	except TimeoutExpired:
		print( test + "................................[TIMEOUT]" )
	except CalledProcessError:
		print( test + "................................[FAILED]" )

	if len(argv) == 2:
		if argv[1] == "-v":
			print(ret)	
