	jmp funcSafetyLabel1:
	main:

		push [BX + 0]
		push 5.00
		greaterCmp
		push 0
		je endif2:
			push 10.0000000000
			out

		endif2:

	funcSafetyLabel1:

call main:
