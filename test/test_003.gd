extends SceneTree

func _init():
	var test = preload("test.gd").new()
	var archive = preload("addons/gdArchive/gdArchive.gdns").new()

	var version = archive.get_version_details_string()

	print(version)

	test.assert("check return type", typeof(version) == TYPE_STRING )
	test.assert("check for libarchive", version.find("libarchive"))
	test.assert("check for zlib", version.find("zlib"))
	test.assert("check for liblzma", version.find("liblzma"))
	test.assert("check for bz2lib", version.find("bz2lib"))

	test.done()
	test.free()
	quit()	
