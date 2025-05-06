	jmp funcSafetyLabel1:
	fibonacci:
	pop [BX + 0]
		push [BX + 0]
		push 2.00
		lessOrEqualCmp
		push 0
		je endif2:
		push [BX + 0]
		pop AX
		ret
		endif2:

		push [BX + 0]
		push 2.00
		sub
		push BX
		push 1
		add
		pop BX
		
		call fibonacci:
		
		push BX
		push 1
		sub
		pop BX

		push AX
		push [BX + 0]
		push 1.00
		sub
		push BX
		push 1
		add
		pop BX
		
		call fibonacci:
		
		push BX
		push 1
		sub
		pop BX

		push AX
		add
		pop AX
		ret

	funcSafetyLabel1:

	jmp funcSafetyLabel3:
	main:

		push 9.00
		push BX
		push 1
		add
		pop BX
		
		call fibonacci:
		
		push BX
		push 1
		sub
		pop BX

		push AX
		pop [BX + 0]

		push [BX + 0]
		out

		push 0.00
		pop AX
		ret

	funcSafetyLabel3:

call main:
