extends SceneTree

func _init():
	var test = preload("test.gd").new()
	var archive = preload("addons/gdArchive/gdArchive.gdns").new()

	var version = archive.get_version_string()

	test.assert("is a string", typeof(version) == TYPE_STRING )
	test.assert("begins with libarchive", version.begins_with("libarchive"))

	test.done()
	test.free()
	quit()	
