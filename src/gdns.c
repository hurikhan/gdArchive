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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//   ____ _       _           _
//  / ___| | ___ | |__   __ _| |___
// | |  _| |/ _ \| '_ \ / _` | / __|
// | |_| | | (_) | |_) | (_| | \__ \
//  \____|_|\___/|_.__/ \__,_|_|___/

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

//  _   _      _                 _____
// | | | | ___| |_ __   ___ _ __|  ___|   _ _ __   ___ ___
// | |_| |/ _ \ | '_ \ / _ \ '__| |_ | | | | '_ \ / __/ __|
// |  _  |  __/ | |_) |  __/ |  |  _|| |_| | | | | (__\__ \
// |_| |_|\___|_| .__/ \___|_|  |_|   \__,_|_| |_|\___|___/
//              |_|

void gdns_print(const char *p_message) {
	godot_string s;

	api->godot_string_new(&s);
	api->godot_string_parse_utf8(&s, p_message);
	api->godot_print(&s);
	api->godot_string_destroy(&s);
}

char *gdns_cstr_new_string(godot_string *p_string) {
	godot_char_string char_str;

	char_str = api->godot_string_utf8(p_string);
	const char *cstr = api->godot_char_string_get_data(&char_str);
	api->godot_char_string_destroy(&char_str);

	char *ret = api->godot_alloc(strlen(cstr));
	strcpy(ret, cstr);

	return ret;
}

char *gdns_cstr_new_variant(godot_variant *p_variant) {
	godot_string str;

	str = godot_variant_as_string(p_variant);
	char *cstr = gdns_cstr_new_string(&str);
	godot_string_destroy(&str);

	char *ret = api->godot_alloc(strlen(cstr + 1));
	strcpy(ret, cstr);
	api->godot_free(cstr);

	return ret;
}

godot_string gdns_string_new_cstr(const char *p_cstr) {
	godot_string ret;

	api->godot_string_new(&ret);
	api->godot_string_parse_utf8(&ret, p_cstr);

	return ret;
}

godot_variant gdns_variant_new_cstr(const char *p_cstr) {
	godot_string s;
	godot_variant ret;

	api->godot_string_new(&s);
	api->godot_string_parse_utf8(&s, p_cstr);
	api->godot_variant_new_string(&ret, &s);
	api->godot_string_destroy(&s);

	return ret;
}

godot_variant *gdns_variant_new_int(int p_value) {
	godot_variant *ret;

	ret = api->godot_alloc(GODOT_VARIANT_SIZE);
	api->godot_variant_new_int(ret, p_value);

	return ret;
}

void gdns_dictionary_new(godot_dictionary *p_dict) {
	api->godot_dictionary_new(p_dict);
}

void gdns_dictionary_destroy(godot_dictionary *p_dict) {
	api->godot_dictionary_destroy(p_dict);
}

void gdns_dictionary_set_int(godot_dictionary *p_dict, const char *p_key, int p_value) {
	godot_variant key;
	godot_variant value;

	key = gdns_variant_new_cstr(p_key);
	api->godot_variant_new_int(&value, p_value);

	api->godot_dictionary_set(p_dict, &key, &value);
}

void gdns_dictionary_set_cstr(godot_dictionary *p_dict, const char *p_key, const char *p_value) {
	godot_variant key;
	godot_variant value;

	key = gdns_variant_new_cstr(p_key);
	value = gdns_variant_new_cstr(p_value);

	api->godot_dictionary_set(p_dict, &key, &value);
}

void gdns_free(void *ptr) {
	api->godot_free(ptr);
}
