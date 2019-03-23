extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdArchive/gdArchive.gdns").new()

	var f = File.new()
	f.open("res://test.gd", 1)
	var file_abs = f.get_path_absolute()
	var res_path = file_abs.get_base_dir()
	f.close()

	var archives = [
		"/data/测试_0001.tar.gz",
		"/data/测试_0001.tar.bz2",	
		"/data/测试_0001.tar.xz",
		#"/data/测试_0001.tar.zst",	# TODO: tar.zst is not working... Return a warning at archive_read_free() instead of ARCHIVE_OK
	]

	var files = [
		"截图_0001.bmp",
		"截图_0002.bmp",
		"截图_0003.bmp", 
	]

	print ()

	for a in archives:
		test.assert("Open " + a, archive.open(res_path + a))

		var ret = archive.list()

		test.assert("Check return value ", typeof(ret) == TYPE_ARRAY );

		for f in files:
			var filename = f

			test.assert("Check returned array has value " + filename , ret.has(filename))

		test.assert("Close " + a, archive.close())
		print()

	for a in archives:
		print("  archive.list() double call...")

		test.assert("Open " + a, archive.open(res_path + a))

		var ret1 = archive.list()
		var ret2 = archive.list()

		test.assert("Check return value 1. call ", typeof(ret1) == TYPE_ARRAY );
		test.assert("Check return value 2. call ", typeof(ret2) == TYPE_ARRAY );

		for f in files:
			var filename = f

			test.assert("Check 1. returned array has value " + filename , ret1.has(filename))
			test.assert("Check 2. returned array has value " + filename , ret2.has(filename))

		test.assert("Close " + a, archive.close())
		print()

	test.done()
	test.free()
	quit()	
