section .data

singleIntFormatSpec db "%d", 10, 0

section .text
extern myPrintfFunction
extern readSingleInt
extern myHaltFunction
extern clearAndOutputBuffer
global _start

function_fact:
	enter 16, 0
	
	push qword [rbp + 16]
	push 1
	xor  rcx, rcx
	pop  rbx
	pop  rax
	cmp  rax, rbx
	setle cl
	push rcx

	push 0
	
	pop rbx
	pop rax
	cmp rax, rbx
	je fact_L0

	push 1
	pop  rax
	leave
	pop  rbx
	add  rsp, 8
	push rax
	push rbx
	ret
	fact_L0:
	push qword [rbp + 16]
	push 1
	pop  rbx
	pop  rax
	sub  rax, rbx
	push rax

	pop  qword [rbp + -8]
	push qword [rbp + -8]
	call function_fact
	push qword [rbp + 16]
	pop rbx
	pop rax
	mul rbx
	push rax

	pop  qword [rbp + -16]
	push qword [rbp + -16]
	pop  rax
	leave
	pop  rbx
	add  rsp, 8
	push rax
	push rbx
	ret
function_fib:
	enter 16, 0
	
	
	push qword [rbp + 16]
	push 2
	xor  rcx, rcx
	pop  rbx
	pop  rax
	cmp  rax, rbx
	setle cl
	push rcx

	push 0
	
	pop rbx
	pop rax
	cmp rax, rbx
	je fib_L0

	push 1
	pop  rax
	leave
	pop  rbx
	add  rsp, 16
	push rax
	push rbx
	ret
	fib_L0:
	push qword [rbp + 16]
	push 1
	pop  rbx
	pop  rax
	sub  rax, rbx
	push rax

	pop  qword [rbp + -8]
	push qword [rbp + 16]
	push 2
	pop  rbx
	pop  rax
	sub  rax, rbx
	push rax

	pop  qword [rbp + -16]
	push qword [rbp + 24]
	push qword [rbp + -8]
	call function_fib
	push qword [rbp + 24]
	push qword [rbp + -16]
	call function_fib
	pop  rbx
	pop  rax
	add  rax, rbx
	push rax

	pop  rax
	leave
	pop  rbx
	add  rsp, 16
	push rax
	push rbx
	ret
_start:
	enter 24, 0
	call readSingleInt
	pop  qword [rbp + -8]
	call readSingleInt
	pop  qword [rbp + -16]
	push qword [rbp + -8]
	push qword [rbp + -16]
	pop  rbx
	pop  rax
	add  rax, rbx
	push rax

	pop  qword [rbp + -24]
	push qword [rbp + -24]
	push singleIntFormatSpec
	call myPrintfFunction
	push 0
	pop  rax
	leave
	call myHaltFunction
