extends SceneTree

func _init():
	var test = preload("test.gd").new()

	var archive = preload("addons/gdArchive/gdArchive.gdns").new()
	var info = archive.get_info()

	test.assert_type(info, TYPE_DICTIONARY)

	test.assert("Dict has key 'gdarchive'", info.has('gdarchive'))
	test.assert_type(info['gdarchive'], TYPE_STRING)

	test.assert("Dict has key 'libarchive'", info.has('libarchive'))
	test.assert_type(info['libarchive'], TYPE_STRING)
	
	test.done()
	test.free()
	quit()	
