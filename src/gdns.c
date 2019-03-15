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

char *gdns_get_string_cstr(godot_string *p_string) {
	godot_char_string char_str;

	char_str = godot_string_utf8(p_string);
	const char *cstr = godot_char_string_get_data(&char_str);
	godot_char_string_destroy(&char_str);

	char *ret = malloc(strlen(cstr + 1));
	strcpy(ret, cstr);

	return ret;
}

char *gdns_get_variant_cstr(godot_variant *p_variant) {
	godot_string str;

	str = godot_variant_as_string(p_variant);
	char *cstr = gdns_get_string_cstr(&str);
	godot_string_destroy(&str);

	char *ret = malloc(strlen(cstr + 1));
	strcpy(ret, cstr);
	free(cstr);

	return ret;
}
