extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdArchive/gdArchive.gdns").new()
	var version = archive.get_version()

	test.assert_type(version, TYPE_STRING )
	test.assert("begins with gdarchive", version.begins_with("gdarchive"))

	test.done()
	test.free()
	quit()	
