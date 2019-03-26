extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdarchive/gdarchive.gdns").new()

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
		[ "截图_0001.bmp", "ad87e0abf32c3318dae86c18890c051e8e20e6d5ce56de4ff0f4ac7efd706da0" ],
		[ "截图_0002.bmp", "d57c29ddf4c324c580feb9db033a8481ccc59ca45d7e07bf3fda50528454c2ec" ],
		[ "截图_0003.bmp", "ab65336af519b6f6c6304d7142c66b18830f0c91a4d2b038a08355b617b51df9" ]
	]

	print ()

	for a in archives:
		test.assert("Open " + a, archive.open(res_path + a))

		var ret = archive.extract()

		test.assert("Check return value ", ret );

		for f in files:
			var filename = f[0]
			var sha256 = f[1]

			var _file = File.new()
			_file.open(filename, File.READ)
			var godot_sha256 = _file.get_sha256(filename)

			test.assert("Check " + filename + " sha256 ", sha256 == godot_sha256)

			_file.close()

			var dir = Directory.new()
			dir.remove(filename)

		test.assert("Close " + a, archive.close())
		print()

	for a in archives:
		print("  archive.extract() double call...")

		test.assert("Open " + a, archive.open(res_path + a))

		var ret = archive.extract()
		ret = archive.extract()

		test.assert("Check return value ", ret );

		for f in files:
			var filename = f[0]
			var sha256 = f[1]

			var _file = File.new()
			_file.open(filename, File.READ)
			var godot_sha256 = _file.get_sha256(filename)

			test.assert("Check " + filename + " sha256 ", sha256 == godot_sha256)

			_file.close()

			var dir = Directory.new()
			dir.remove(filename)

		test.assert("Close " + a, archive.close())
		print()

	test.done()
	test.free()
	quit()	
