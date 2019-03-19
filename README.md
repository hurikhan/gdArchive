# gdArchive
gdnative plugin for libarchive in pure C

## Methods

```gdscript
  get_version()   # Returns the current version of gdArchive as a string (e.g. "gdarchive 0.0.1")
  get_info()      # Returns the current used libarchive version and compression libs as a dictionary
  open(filename)  # Opens an archive as readonly. Returns true on success
  close()         # Closes an already opened archive. Returns true on success
  list()          # Lists the file of an opened archive. Returns an array.
```

## Usage
```gdscript
  var archive = preload("res://addons/gdArchive/gdArchive.gdns").new()

	print(archive.get_version())
	print(archive.get_info())
  
	archive.open("user://downloads/lutris/quake-shareware.tar.gz")
	var files = archive.list()
	archive.close()
  
  for f in files:
	  print(f)
```
