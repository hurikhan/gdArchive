# gdArchive
gdnative plugin for libarchive in pure C

## Status
* Version: 0.0.1
* Platforms: linux
* Dependencies: libarchive 3.3.3+

## Clone
```bash
  git clone https://github.com/hurikhan/gdArchive.git
  git submodule init
  git submodule update
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
  var archive = preload("res://addons/gdarchive/gdarchive.gdns").new()

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
- [x] Extract files from archive
- [x] UTF-8 support
- [x] Complete manual invoked tests/ for the current API
- [x] Builtin libarchive sources + scons workflow
- [ ] Create Docker-Container for libgdarchive.so + libarchive.so compilation

### Version 0.2.0
- [ ] Nativescript 1.1 support
- [ ] Supported platforms: linux, windows
- [ ] Support for zstd compression
- [ ] Invoke test/test.py with scons

### Version 0.3.0
- [ ] Define final API (Version 1.0.0)

