# gdArchive
gdnative plugin for libarchive in pure C

## Status
* Version: 0.0.1
* Platforms: linux
* Dependencies: libarchive 3.3.3+

## Clone
```bash
  git clone --recurse-submodules https://github.com/hurikhan/gdArchive.git
```

## Compile
```bash
  scons platform=linux
```

## Methods
```gdscript
  get_version()   # Returns the current version of gdArchive as a string (e.g. "gdarchive 0.0.1")
  get_info()      # Returns the current used libarchive version and compression libs as a dictionary
  open(filename)  # Opens an archive as readonly. Returns true on success
  close()         # Closes an already opened archive. Returns true on success
  list()          # Returns an array of the archive entries
  extract()	  # Extracts the whole archive in the current directory
  extract(path)   # Extracts the whole archive in the specified path
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

## ToDo
### Version 0.1.0
- [x] Supported platforms: linux
- [x] List files in archive
- [ ] Extract files from archive
- [ ] UTF-8 support
- [ ] Complete manual invoked tests/ for the current API
- [ ] Create Docker-Container for libgdarchive.so + libarchive.so compilation
- [ ] Bundle gdArchive with libarchive.so
### Version 0.2.0
- [ ] Nativescript 1.1 support
- [ ] Supported platforms: linux, windows
- [ ] Invoke tests/ with scons
