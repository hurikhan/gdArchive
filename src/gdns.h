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

#ifndef GDNS_H
#define GDNS_H

#include <gdnative_api_struct.gen.h>

#define GDNS_PARAM               \
	godot_object *p_instance,    \
			void *p_method_data, \
			void *p_user_data,   \
			int p_num_args,      \
			godot_variant **p_args

#define GDNS_CONSTRUCTOR_PARAM \
	godot_object *p_instance,  \
			void *p_method_data

#define GDNS_DESTRUCTOR_PARAM    \
	godot_object *p_instance,    \
			void *p_method_data, \
			void *p_user_data

#define GDNS_REGISTER_CLASS(CLASSNAME, TYPE, CONSTRUCTOR, DESTRUCTOR)            \
	godot_instance_create_func MACRO_create##CLASSNAME = { NULL, NULL, NULL };   \
	MACRO_create##CLASSNAME.create_func = CONSTRUCTOR;                           \
	godot_instance_destroy_func MACRO_destroy##CLASSNAME = { NULL, NULL, NULL }; \
	MACRO_destroy##CLASSNAME.destroy_func = DESTRUCTOR;                          \
	nativescript_api->godot_nativescript_register_class(p_handle, #CLASSNAME, #TYPE, MACRO_create##CLASSNAME, MACRO_destroy##CLASSNAME);

#define GDNS_REGISTER_METHOD(CLASSNAME, METHODNAME, FUNCREF)                                      \
	godot_instance_method MACRO_##METHODNAME = { NULL, NULL, NULL };                              \
	MACRO_##METHODNAME.method = FUNCREF;                                                          \
	godot_method_attributes MACRO_##METHODNAME##_attributes = { GODOT_METHOD_RPC_MODE_DISABLED }; \
	nativescript_api->godot_nativescript_register_method(p_handle, #CLASSNAME, #METHODNAME, MACRO_##METHODNAME##_attributes, MACRO_##METHODNAME);

//   ____ _       _           _
//  / ___| | ___ | |__   __ _| |___
// | |  _| |/ _ \| '_ \ / _` | / __|
// | |_| | | (_) | |_) | (_| | \__ \
//  \____|_|\___/|_.__/ \__,_|_|___/

extern const godot_gdnative_core_api_struct *api;
extern const godot_gdnative_ext_nativescript_api_struct *nativescript_api;

//  ____            _        _
// |  _ \ _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___
// | |_) | '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
// |  __/| | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
// |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
//                               |___/|_|

char *gdns_get_string_cstr(godot_string *p_string);
char *gdns_get_variant_cstr(godot_variant *p_variant);

#endif
