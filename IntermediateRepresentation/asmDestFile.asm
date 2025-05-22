section .data

singleIntFormatSpec db "%d", 10, 0

section .text
extern myPrintfFunction
extern readSingleInt
extern myHaltFunction
extern clearAndOutputBuffer
global _start

function_fibonacci:
	enter 0, 0
	
	
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
	je fibonacci_L0

	push 1
	pop  rax
	leave
	pop  rbx
	add  rsp, 16
	push rax
	push rbx
	ret
	fibonacci_L0:
	push qword [rbp + 24]
	push qword [rbp + 16]
	push 1
	pop  rbx
	pop  rax
	sub  rax, rbx
	push rax

	call function_fibonacci
	push qword [rbp + 24]
	push qword [rbp + 16]
	push 2
	pop  rbx
	pop  rax
	sub  rax, rbx
	push rax

	call function_fibonacci
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
	enter 16, 0
	call readSingleInt
	pop  qword [rbp + -8]
	push 10
	push qword [rbp + -8]
	call function_fibonacci
	pop  qword [rbp + -16]
	push qword [rbp + -16]
	push singleIntFormatSpec
	call myPrintfFunction
	push 0
	pop  rax
	leave
	push rax
	call myHaltFunction
