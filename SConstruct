#!/usr/bin/env python

env = Environment()

opts = Variables(None, ARGUMENTS)
opts.Add("platform", "Target platform (linux)", "")
opts.Add(BoolVariable("src_format", "Format source code in src/ with clang-format",False))
opts.Add("env_print", "Prints the scons enviroment (all|key)", "")
opts.Update(env)
Help(opts.GenerateHelpText(env))

if env['src_format']:
	Execute("clang-format -i --style=file src/*.h")
	Execute("clang-format -i --style=file src/*.c")

if env['platform'] == "linux":
	Export('env')
	env.SConscript('src/SConscript', variant_dir='build/$platform', duplicate=0)
	env.Install("#addons/gdArchive", "#build/linux/libgdarchive.so")
