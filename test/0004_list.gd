extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdArchive/gdArchive.gdns").new()

	test.assert("Open without filename", !archive.open())
	test.assert("Open with wrong path", !archive.open("../data/test.tar.gzip"))
	test.assert("Open with wrong path", !archive.open("../data/test.tar.bz2"))
	test.assert("Open with wrong path", !archive.open("../data/test.tar.xz"))
	test.assert("Open with wrong path", !archive.open("../data/test.tar.zst"))

	var f = File.new()
	f.open("res://test.gd", 1)
	var file_abs = f.get_path_absolute()
	var res_path = file_abs.get_base_dir()
	f.close()

	var a = ""
	var arr = []
	var files = [ "Screenshot_0001.bmp", "Screenshot_0002.bmp", "Screenshot_0003.bmp" ]


	a = "/data/test_0001.tar.gz"	
	test.assert("Open " + a, archive.open(res_path + a))
	arr = archive.list()
	test.assert("Return type ", typeof(arr) == TYPE_ARRAY)
	test.assert("Array size ", arr.size() == 3)
	test.assert("Array has " + files[0], arr.has(files[0]))
	test.assert("Array has " + files[1], arr.has(files[1]))
	test.assert("Array has " + files[2], arr.has(files[2]))
	test.assert("Close " + a, archive.close())

	a = "/data/test_0001.tar.bz2"	
	test.assert("Open " + a, archive.open(res_path + a))
	arr = archive.list()
	test.assert("Return type ", typeof(arr) == TYPE_ARRAY)
	test.assert("Array size ", arr.size() == 3)
	test.assert("Array has " + files[0], arr.has(files[0]))
	test.assert("Array has " + files[1], arr.has(files[1]))
	test.assert("Array has " + files[2], arr.has(files[2]))
	test.assert("Close " + a, archive.close())

	a = "/data/test_0001.tar.xz"	
	test.assert("Open " + a, archive.open(res_path + a))
	arr = archive.list()
	test.assert("Return type ", typeof(arr) == TYPE_ARRAY)
	test.assert("Array size ", arr.size() == 3)
	test.assert("Array has " + files[0], arr.has(files[0]))
	test.assert("Array has " + files[1], arr.has(files[1]))
	test.assert("Array has " + files[2], arr.has(files[2]))
	test.assert("Close " + a, archive.close())

	a = "/data/test_0001.tar.zst"	
	test.assert("Open " + a, archive.open(res_path + a))
	arr = archive.list()
	test.assert("Return type ", typeof(arr) == TYPE_ARRAY)
	test.assert("Array size ", arr.size() == 3)
	test.assert("Array has " + files[0], arr.has(files[0]))
	test.assert("Array has " + files[1], arr.has(files[1]))
	test.assert("Array has " + files[2], arr.has(files[2]))
	test.assert("Close " + a, archive.close())

	test.assert("Close without open", !archive.close())

	test.done()
	test.free()
	quit()	
