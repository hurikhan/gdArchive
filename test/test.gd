extends Object

var counter = 0
var passed = true

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

func done():
	if counter > 0:
		if passed:
			print("[TEST OK]")
		else:
			print("[TEST FAILED]")
	else:
		print("[NO ASSERT EXECUTED]")
