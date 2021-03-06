extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdarchive/gdarchive.gdns").new()
	var version = archive.get_version()

	print(version)
	test.assert_type(version, TYPE_STRING )
	test.assert("begins with gdarchive", version.begins_with("gdarchive"))

	test.done()
	test.free()
	quit()	
