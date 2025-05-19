global readSingleInt
extern myPrintfFunction
extern clearAndOutputBuffer

MAX_INPUT_BUFF_LEN equ 10

section .data

inputPrompt db "Print: ", 0
inputBuffer db MAX_INPUT_BUFF_LEN dup(0)

section .text

; Entry: rsi: address of the buffer
;        rcx: string len
; Destr: 
; result: rax
stringToInt:
    xor r9, r9
    mov al, byte [rsi]
    cmp al, '-'
    jne positiveSign
        inc rsi
        dec rcx
        mov r9, 1 ; set flag that sign is -1
    positiveSign:

    xor rax, rax
    dec rcx
    digitLoop:
        imul rax, 10
        xor rbx, rbx
        mov bl,  byte [rsi]
        add rax, rbx
        sub rax, '0'
        inc rsi

        dec rcx
        cmp rcx, 0
        jne digitLoop

    ; check if r9 = 1 <=> sign of number is -1
    test r9, r9
    jz positiveNumber
        neg rax
    positiveNumber:

    ret

; Entry: none
; Destr:
; Exit : pushes number that was read to stack
readSingleInt:
    push inputPrompt
    call myPrintfFunction
    ; call clearAndOutputBuffer

    ; Read input
    ; we don't use xor here to specifically state used values
    mov rax, 0                     ; sys_read function id
    mov rdi, 0                     ; stdin id
    mov rsi, inputBuffer           ; input buffer address
    mov rdx, MAX_INPUT_BUFF_LEN    ; buffer size = max number of chars that can be read
    syscall
    ; rax contains number of bytes read (including newline)

    lea rsi, [inputBuffer]
    mov rcx, rax
    call stringToInt

    pop  rbx ;    save return address
    push rax ; put result to stack
    push rbx ; restore return address
    ret
