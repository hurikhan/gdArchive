// Copyright (c) 2019 Mario Schlack
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "gdns.h"
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define VERSION "0.0.1"
#define VERSION_STRING "gdarchive 0.0.1"
#define FILENAME_SIZE 2048

//  ____            _        _
// |  _ \ _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___
// | |_) | '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
// |  __/| | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
// |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
//                               |___/|_|

void *gdarchive_constructor(GDNS_CONSTRUCTOR_PARAM);
void gdarchive_destructor(GDNS_DESTRUCTOR_PARAM);

godot_variant gdarchive_get_version(GDNS_PARAM);
godot_variant gdarchive_get_info(GDNS_PARAM);
godot_variant gdarchive_open(GDNS_PARAM);
godot_variant gdarchive_close(GDNS_PARAM);
godot_variant gdarchive_list(GDNS_PARAM);
godot_variant gdarchive_extract(GDNS_PARAM);

char *_gdarchive_path(char *p_path);
int _gdarchive_copy_data(struct archive *ar, struct archive *aw); // Helper for gdarchive_extract

//   ____ _
//  / ___| | __ _ ___ ___
// | |   | |/ _` / __/ __|
// | |___| | (_| \__ \__ \
//  \____|_|\__,_|___/___/

typedef struct user_data_struct {
	char filename[FILENAME_SIZE];
	struct archive *a;
	bool opened; // Archive is opened (successfully)
	bool used; // archive.list() was called
} user_data_struct;

void *gdarchive_constructor(GDNS_CONSTRUCTOR_PARAM) {
	user_data_struct *user_data = api->godot_alloc(sizeof(user_data_struct));
	memset(user_data, 0, sizeof(user_data_struct));

	return user_data;
}

void gdarchive_destructor(GDNS_DESTRUCTOR_PARAM) {
	user_data_struct *self;
	self = p_user_data;

	if (self->opened) {
		int r = archive_read_free(self->a);
		if (r != ARCHIVE_OK)
			gdns_print("[gdArchive-destructor] Archive could not be freed!");
	}

	api->godot_free(p_user_data);
}

//  __  __      _   _               _
// |  \/  | ___| |_| |__   ___   __| |___
// | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
// | |  | |  __/ |_| | | | (_) | (_| \__ \
// |_|  |_|\___|\__|_| |_|\___/ \__,_|___/

godot_variant gdarchive_get_version(GDNS_PARAM) {
	godot_variant ret = gdns_variant_new_cstr(VERSION_STRING);

	return ret;
}

godot_variant gdarchive_get_info(GDNS_PARAM) {
	godot_dictionary dict;
	godot_string s;

	godot_dictionary_new(&dict);

	// gdArchive Version
	gdns_dictionary_set_cstr(&dict, "gdarchive", VERSION);

	// libarchive version details
	const char *details = archive_version_details();
	char *buffer = api->godot_alloc(strlen(details));
	memcpy(buffer, details, strlen(details));

	// count space delimters
	char *tmp = buffer;
	size_t count = 0;
	while (*tmp) {
		if (' ' == *tmp) {
			count++;
		}
		tmp++;
	}

	// get libarchive version "libarchive x.y.z ..."
	// archive_version_details() example:
	// 	"libarchive 3.3.3 zlib/1.2.11 liblzma/5.2.4 bz2lib/1.0.6"
	//       [---------------]
	char *libarchive_name = strtok(buffer, " ");
	char *libarchive_version = strtok(NULL, " ");
	count -= 2;

	// Add libarchive entry to the dictionary
	if (libarchive_name != NULL && libarchive_version != NULL)
		gdns_dictionary_set_cstr(&dict, libarchive_name, libarchive_version);

	// Add following libs entry from libarchive to the dictionary
	// archive_version_details() example:
	// 	"libarchive 3.3.3 zlib/1.2.11 liblzma/5.2.4 bz2lib/1.0.6"
	//                        [------------------------------------]
	if (count > 0) {
		char **libs = api->godot_alloc(sizeof(char *) * count);
		size_t libs_idx = 0;

		char *lib_entry = "";

		// get entries seperated by ' '
		// 	"libarchive 3.3.3 zlib/1.2.11 liblzma/5.2.4 bz2lib/1.0.6"
		//                        [---------] [-----------] [----------]
		while (lib_entry) {
			lib_entry = strtok(NULL, " ");
			if (lib_entry) {
				char *e = api->godot_alloc(strlen(lib_entry));
				memcpy(e, lib_entry, strlen(lib_entry));

				libs[libs_idx] = e;
				libs_idx++;
			}
		}

		// split found entries seperated by '/'
		// and add them to the dictionary
		// 	"libarchive 3.3.3 zlib/1.2.11 liblzma/5.2.4 bz2lib/1.0.6"
		//                        [--] [----] [-----] [---] [----] [---]
		for (int i = 0; i < libs_idx; i++) {
			char *e = libs[i];

			char *name = strtok(e, "/");
			char *version = strtok(NULL, "/");

			gdns_dictionary_set_cstr(&dict, name, version);
			api->godot_free(e);
		}

		api->godot_free(libs);
	}
	api->godot_free(buffer);

	godot_variant ret;
	api->godot_variant_new_dictionary(&ret, &dict);
	gdns_dictionary_destroy(&dict);

	return ret;
}

// TODO: add tests/ for "res://.." and absolute pathes
godot_variant gdarchive_open(GDNS_PARAM) {
	user_data_struct *self;
	self = p_user_data;

	char *arg0 = gdns_cstr_new_variant(p_args[0]);
	char *path = _gdarchive_path(arg0);
	memcpy(self->filename, path, strlen(path) + 1);
	gdns_free(path);
	gdns_free(arg0);

	self->a = archive_read_new();
	archive_read_support_filter_all(self->a);
	archive_read_support_format_all(self->a);

	int r = archive_read_open_filename(self->a, self->filename, 10240);

	godot_variant ret;

	if (r == ARCHIVE_OK) {
		self->opened = true;
		api->godot_variant_new_bool(&ret, true);
	} else {
		gdns_print("[gdArchive] Could not open archive!");
		memset(self->filename, 0, sizeof(self->filename));
		api->godot_variant_new_bool(&ret, false);
	}

	return ret;
}

godot_variant gdarchive_close(GDNS_PARAM) {
	user_data_struct *self;
	self = p_user_data;

	godot_variant ret;

	if (self->opened) {
		int r = archive_read_free(self->a);

		if (r == ARCHIVE_OK) {
			api->godot_variant_new_bool(&ret, true);
		} else {
			gdns_print("[gdArchive] Archive could not be freed!");
			api->godot_variant_new_bool(&ret, false);
		}
	} else {
		api->godot_variant_new_bool(&ret, false);
	}
	self->opened = false;
	self->used = false;

	memset(self->filename, 0, sizeof(self->filename));

	return ret;
}

godot_variant gdarchive_list(GDNS_PARAM) {
	user_data_struct *self;
	self = p_user_data;

	// used for unicode support
	char *old_locale = NULL;

	if (self->opened && self->used) {
		// archive is already opened(self->opened) and the
		// archive_read_next_header() function was already called (self->used).
		//
		// To get a fresh struct archive *a (self->a):
		//   1. gdarchive_close() is called
		//   2. gdarchive_open(filename) is called
		//
		godot_variant filename = gdns_variant_new_cstr(self->filename);
		godot_variant *_p_args[1];
		_p_args[0] = &filename;

		godot_variant ret_close = gdarchive_close(p_instance,
				NULL,
				p_user_data,
				0,
				NULL);

		godot_variant ret_open = gdarchive_open(p_instance,
				NULL,
				p_user_data,
				1,
				_p_args);

		api->godot_variant_destroy(&filename);
	}

	// unicode support
	old_locale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");

	struct archive_entry *entry;
	godot_array arr;
	godot_string s;
	godot_variant element;

	godot_array_new(&arr);

	if (self->opened) {
		while (archive_read_next_header(self->a, &entry) == ARCHIVE_OK) {
			self->used = true;

			api->godot_string_new(&s);
			api->godot_string_parse_utf8(&s, archive_entry_pathname(entry));
			api->godot_variant_new_string(&element, &s);
			api->godot_string_destroy(&s);

			api->godot_array_append(&arr, &element);
			api->godot_variant_destroy(&element);

			archive_read_data_skip(self->a);
		}
	}

	if (old_locale != NULL)
		setlocale(LC_ALL, old_locale);

	godot_variant ret;
	godot_variant_new_array(&ret, &arr);
	godot_array_destroy(&arr);

	return ret;
}

godot_variant gdarchive_extract(GDNS_PARAM) {
	user_data_struct *self;
	self = p_user_data;

	char *arg0 = gdns_cstr_new_variant(p_args[0]);

	char *destination = NULL;
	size_t destination_len = 0;

	// used for unicode support
	char *old_locale = NULL;

	// Eval p_args[0] -> destination
	if (p_num_args == 1) {
		destination = _gdarchive_path(arg0);
		destination_len = strlen(destination);
	}

	// TODO: DRY!
	if (self->opened && self->used) {
		// archive is already opened(self->opened) and the
		// archive_read_next_header() function was already called (self->listed).
		//
		// To get a fresh struct archive *a (self->a):
		//   1. gdarchive_close() is called
		//   2. gdarchive_open(filename) is called
		//
		godot_variant filename = gdns_variant_new_cstr(self->filename);
		godot_variant *_p_args[1];
		_p_args[0] = &filename;

		godot_variant ret_close = gdarchive_close(p_instance,
				NULL,
				p_user_data,
				0,
				NULL);

		godot_variant ret_open = gdarchive_open(p_instance,
				NULL,
				p_user_data,
				1,
				_p_args);

		api->godot_variant_destroy(&filename);
	}
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int flags;
	int r;

	a = self->a;

	// unicode support
	old_locale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");

	/* Select which attributes we want to restore. */
	flags = ARCHIVE_EXTRACT_TIME;
	flags |= ARCHIVE_EXTRACT_PERM;
	flags |= ARCHIVE_EXTRACT_ACL;
	flags |= ARCHIVE_EXTRACT_FFLAGS;

	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);
	archive_write_disk_set_standard_lookup(ext);

	self->used = true;

	for (;;) {
		r = archive_read_next_header(a, &entry);

		if (r == ARCHIVE_EOF)
			break;

		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(a));

		if (r < ARCHIVE_WARN)
			//exit(1);
			break;

		if (destination_len > 0) {
			const char *filename = archive_entry_pathname(entry);

			size_t filename_len = strlen(filename);

			char *fullpath = api->godot_alloc(destination_len + filename_len + 1);

			memset(fullpath, 0, strlen(destination) + 1);
			memcpy(fullpath, destination, strlen(destination));
			strcat(fullpath, filename);

			printf("%s\n", fullpath);
			archive_entry_set_pathname(entry, fullpath);

			api->godot_free(fullpath);
		}

		r = archive_write_header(ext, entry);
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));

		else if (archive_entry_size(entry) > 0) {
			r = _gdarchive_copy_data(a, ext);

			if (r < ARCHIVE_OK)
				fprintf(stderr, "%s\n", archive_error_string(ext));

			if (r < ARCHIVE_WARN)
				exit(1);
		}

		r = archive_write_finish_entry(ext);
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));
		if (r < ARCHIVE_WARN)
			//exit(1);
			break;
	}
	archive_write_close(ext);
	archive_write_free(ext);

	if (destination != NULL)
		gdns_free(destination);

	if (old_locale != NULL)
		setlocale(LC_ALL, old_locale);

	godot_variant ret;
	api->godot_variant_new_bool(&ret, true);
	return ret;
}

//            _             _   _
//   __ _  __| |_ __   __ _| |_(_)_   _____
//  / _` |/ _` | '_ \ / _` | __| \ \ / / _ \
// | (_| | (_| | | | | (_| | |_| |\ V /  __/
//  \__, |\__,_|_| |_|\__,_|\__|_| \_/ \___|
//  |___/

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	api = p_options->api_struct;
	for (int i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];

			}; break;
			default: break;
		}
	}
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	api = NULL;
	nativescript_api = NULL;
}

//              _   _                          _       _
//  _ __   __ _| |_(_)_   _____  ___  ___ _ __(_)_ __ | |_
// | '_ \ / _` | __| \ \ / / _ \/ __|/ __| '__| | '_ \| __|
// | | | | (_| | |_| |\ V /  __/\__ \ (__| |  | | |_) | |_
// |_| |_|\__,_|\__|_| \_/ \___||___/\___|_|  |_| .__/ \__|

void GDN_EXPORT godot_nativescript_init(void *p_handle) {

	GDNS_REGISTER_CLASS(ARCHIVE, Reference, &gdarchive_constructor, &gdarchive_destructor)

	GDNS_REGISTER_METHOD(ARCHIVE, get_version, &gdarchive_get_version)
	GDNS_REGISTER_METHOD(ARCHIVE, get_info, &gdarchive_get_info)
	GDNS_REGISTER_METHOD(ARCHIVE, open, &gdarchive_open)
	GDNS_REGISTER_METHOD(ARCHIVE, close, &gdarchive_close)
	GDNS_REGISTER_METHOD(ARCHIVE, list, &gdarchive_list)
	GDNS_REGISTER_METHOD(ARCHIVE, extract, &gdarchive_extract)
}

//  _   _      _                 _____
// | | | | ___| |_ __   ___ _ __|  ___|   _ _ __   ___ ___
// | |_| |/ _ \ | '_ \ / _ \ '__| |_ | | | | '_ \ / __/ __|
// |  _  |  __/ | |_) |  __/ |  |  _|| |_| | | | | (__\__ \
// |_| |_|\___|_| .__/ \___|_|  |_|   \__,_|_| |_|\___|___/
//              |_|

char *_gdarchive_path(char *p_path) {

	size_t p_path_len = strlen(p_path);

	if (memcmp(p_path, "user://", 7) == 0) {
		godot_object *os;
		godot_method_bind *mb;

		os = api->godot_global_get_singleton((char *)"OS");
		mb = api->godot_method_bind_get_method("_OS", "get_user_data_dir");

		godot_string path;
		godot_string_new(&path);
		const void *args[1] = {};
		api->godot_method_bind_ptrcall(mb, os, args, &path);

		char *user_path = gdns_cstr_new_string(&path);
		size_t user_path_len = strlen(user_path);

		char *rem = p_path + 7;
		size_t rem_len = strlen(p_path) - 7;

		char *ret = api->godot_alloc(user_path_len + rem_len + 2);
		memcpy(ret, user_path, user_path_len);

		if (ret[user_path_len] != '/')
			strcat(ret, "/"); // TODO: Keep Windows in mind!

		strcat(ret, rem);

		godot_string_destroy(&path);
		gdns_free(user_path);

		return ret;
	}

	char *ret = api->godot_alloc(p_path_len + 1);
	memcpy(ret, p_path, p_path_len + 1);
	return ret;
}

int _gdarchive_copy_data(struct archive *ar, struct archive *aw) {
	int r;
	const void *buff;
	size_t size;
	la_int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);

		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);

		if (r < ARCHIVE_OK)
			return (r);

		r = archive_write_data_block(aw, buff, size, offset);

		if (r < ARCHIVE_OK) {
			fprintf(stderr, "%s\n", archive_error_string(aw));
			return (r);
		}
	}
}
