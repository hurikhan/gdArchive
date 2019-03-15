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

//  ____            _        _
// |  _ \ _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___
// | |_) | '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
// |  __/| | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
// |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
//                               |___/|_|

void *gdarchive_constructor(GDNS_CONSTRUCTOR_PARAM);
void gdarchive_destructor(GDNS_DESTRUCTOR_PARAM);

godot_variant gdarchive_get_version_string(GDNS_PARAM);
godot_variant gdarchive_get_version_dict(GDNS_PARAM);
godot_variant gdarchive_get_version_details_string(GDNS_PARAM);
godot_variant gdarchive_list_files(GDNS_PARAM);

//   ____ _
//  / ___| | __ _ ___ ___
// | |   | |/ _` / __/ __|
// | |___| | (_| \__ \__ \
//  \____|_|\__,_|___/___/

typedef struct user_data_struct {
	char data[256];
} user_data_struct;

void *gdarchive_constructor(GDNS_CONSTRUCTOR_PARAM) {
	user_data_struct *user_data = api->godot_alloc(sizeof(user_data_struct));
	strcpy(user_data->data, "World from GDNative!");

	return user_data;
}

void gdarchive_destructor(GDNS_DESTRUCTOR_PARAM) {
	api->godot_free(p_user_data);
}

//  __  __      _   _               _
// |  \/  | ___| |_| |__   ___   __| |___
// | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
// | |  | |  __/ |_| | | | (_) | (_| \__ \
// |_|  |_|\___|\__|_| |_|\___/ \__,_|___/

godot_variant gdarchive_get_version_string(GDNS_PARAM) {

	godot_string s;
	godot_variant ret;

	const char *version = archive_version_string();

	api->godot_string_new(&s);
	api->godot_string_parse_utf8(&s, version);
	api->godot_variant_new_string(&ret, &s);
	api->godot_string_destroy(&s);

	return ret;
}

godot_variant gdarchive_get_version_dict(GDNS_PARAM) {

	godot_dictionary dict;
	godot_variant key;
	godot_variant value;
	godot_variant ret;

	godot_string s;

	int major = archive_version_number() / 1000000;
	int minor = (archive_version_number() / 1000) % 1000;
	int patch = archive_version_number() % 1000;

	api->godot_dictionary_new(&dict);
	api->godot_string_new(&s);

	api->godot_string_parse_utf8(&s, "major");
	api->godot_variant_new_string(&key, &s);
	api->godot_variant_new_int(&value, major);
	api->godot_dictionary_set(&dict, &key, &value);
	api->godot_variant_destroy(&key);
	api->godot_variant_destroy(&value);

	api->godot_string_parse_utf8(&s, "minor");
	api->godot_variant_new_string(&key, &s);
	api->godot_variant_new_int(&value, minor);
	api->godot_dictionary_set(&dict, &key, &value);
	api->godot_variant_destroy(&key);
	api->godot_variant_destroy(&value);

	api->godot_string_parse_utf8(&s, "patch");
	api->godot_variant_new_string(&key, &s);
	api->godot_variant_new_int(&value, patch);
	api->godot_dictionary_set(&dict, &key, &value);
	api->godot_variant_destroy(&key);
	api->godot_variant_destroy(&value);

	api->godot_variant_new_dictionary(&ret, &dict);

	api->godot_string_destroy(&s);
	api->godot_dictionary_destroy(&dict);

	return ret;
}

godot_variant gdarchive_get_version_details_string(GDNS_PARAM) {

	godot_string s;
	godot_variant ret;

	const char *version_details = archive_version_details();

	api->godot_string_new(&s);
	api->godot_string_parse_utf8(&s, version_details);
	api->godot_variant_new_string(&ret, &s);
	api->godot_string_destroy(&s);

	return ret;
}

godot_variant gdarchive_list_files(GDNS_PARAM) {

	// TODO: dynamic allocation of char array filename
	char filename[1024];
	memset(filename, 0, 1024);

	char *arg0 = gdns_get_variant_cstr(p_args[0]);

	if (memcmp(arg0, "user://", 7) == 0) {
		godot_object *os;
		godot_method_bind *mb;

		os = api->godot_global_get_singleton((char *)"OS");
		mb = api->godot_method_bind_get_method("_OS", "get_user_data_dir");

		godot_string path;
		godot_string_new(&path);
		const void *args[1] = {};
		api->godot_method_bind_ptrcall(mb, os, args, &path);

		char *user_path = gdns_get_string_cstr(&path);
		strcat(filename, user_path);

		size_t user_path_len = strlen(user_path);
		size_t arg0_len = strlen(arg0);
		memcpy(filename + user_path_len, arg0 + 7 - 1, arg0_len - 7 + 1);

		godot_string_destroy(&path);
		free(user_path);
	} else {
		strcat(filename, arg0);
	}
	free(arg0);

	godot_array arr;
	godot_string s;
	godot_variant element;

	godot_array_new(&arr);

	// --------------------

	struct archive *a;
	struct archive_entry *entry;
	int r;

	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);

	r = archive_read_open_filename(a, filename, 10240); // Note 1

	if (r != ARCHIVE_OK)
		printf("Could not open archive!\n");

	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
		api->godot_string_new(&s);
		api->godot_string_parse_utf8(&s, archive_entry_pathname(entry));
		api->godot_variant_new_string(&element, &s);
		api->godot_string_destroy(&s);

		api->godot_array_append(&arr, &element);
		api->godot_variant_destroy(&element);

		archive_read_data_skip(a); // Note 2
	}

	r = archive_read_free(a); // Note 3
	if (r != ARCHIVE_OK)
		printf("Archive could not be freed!\n");

	godot_variant ret;
	godot_variant_new_array(&ret, &arr);
	godot_array_destroy(&arr);

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

	GDNS_REGISTER_METHOD(ARCHIVE, get_version_string, &gdarchive_get_version_string)
	GDNS_REGISTER_METHOD(ARCHIVE, get_version_dict, &gdarchive_get_version_dict)
	GDNS_REGISTER_METHOD(ARCHIVE, get_version_details_string, &gdarchive_get_version_details_string)
	GDNS_REGISTER_METHOD(ARCHIVE, list_files, &gdarchive_list_files)
}