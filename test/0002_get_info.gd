extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdArchive/gdArchive.gdns").new()
	var version = archive.get_info()

	test.assert_type(version, TYPE_DICTIONARY)

	test.assert("Dict has key 'gdarchive'", version.has('gdarchive'))
	test.assert_type(version['gdarchive'], TYPE_STRING)

	test.assert("Dict has key 'libarchive'", version.has('libarchive'))
	test.assert_type(version['libarchive'], TYPE_STRING)
	
	test.done()
	test.free()
	quit()	
