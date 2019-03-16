extends SceneTree

func _init():
	var test = preload("test.gd").new()
	var archive = preload("addons/gdArchive/gdArchive.gdns").new()

	var version = archive.get_version_dict()

	test.assert("Major", typeof(version.major) == TYPE_INT )
	test.assert("Minor", typeof(version.minor) == TYPE_INT )
	test.assert("Patch", typeof(version.patch) == TYPE_INT )

	test.done()
	test.free()
	quit()	
