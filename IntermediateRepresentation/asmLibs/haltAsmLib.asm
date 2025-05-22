global myHaltFunction
extern clearAndOutputBuffer

; finishes program with given exit code
; Entry: exit code from the top of the stack
myHaltFunction:
    ; call clearAndOutputBuffer
	mov rdi, [rsp + 8]
	mov  rax, 60
	syscall

    ret ; actually that's redundant as it won't be executed
    