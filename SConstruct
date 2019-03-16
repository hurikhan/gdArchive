#!/usr/bin/env python

# System
import glob
import os
import string
import sys

platform = "linux"
include = "#/src/godot_headers"
libdir = "#/$PLATFORM"
bindir = "#/"

env = Environment(
	PLATFORM = platform,
	BINDIR = bindir,
	INCDIR = include,
	LIBDIR = libdir,
	CPPPATH = [include],
	LIBPATH = [libdir],
	LIBS = "archive")

env.Install("#addons/gdArchive", "#build/linux/libgdarchive.so")

Export('env')

env.SConscript('src/SConscript', variant_dir='build/$PLATFORM', duplicate=0)
