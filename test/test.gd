extends Object

var counter = 0
var passed = true


# func sub_print(text, post=""):
# 	assert(text.length < 50)
# 	var s = "  " + test
# 
# 	var len = s.length()
# 	var padding_len = 50 - l
# 	var padding = ""
# 
# 	for i in range(padding_len):
# 		padding += "."
# 
# 	return s + padding + post
	


func assert( desc, check ):

	#TODO: Use switch or match statement
	if typeof(check) == TYPE_BOOL:
		if check:
			print("  ", desc, " [OK]")
		else:
			print("  ", desc, " [FAILED]")
			passed = false

	if typeof(check) == TYPE_INT:
		if check != -1:
			print("  ", desc, " [OK]")
		else:
			print("  ", desc, " [FAILED]")
			passed = false

	counter = counter + 1

func assert_type(value, type):

	var s = "  check type "

	if typeof(value) == type:
		print(s, "[OK]")
	else:
		print(s, "[FAILED]")
		passed = false
	
	counter = counter + 1

func done():
	if counter > 0:
		if passed:
			print("[TEST OK]")
		else:
			print("[TEST FAILED]")
	else:
		print("[NO ASSERT EXECUTED]")
