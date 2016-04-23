.data
	scanf_int_format:
		.string "%d%*c"
	printf_int_format:
		.string "%d\n"
	scanf_char_format:
		.string "%c%*c"
	printf_char_format:
		.string "%c\n"
	D:
		.long 0
	M:
		.long 0
.text
.globl main
.type main, @function
main:

pushl $D
pushl $scanf_int_format
call scanf
addl $8, %esp

pushl $M
pushl $scanf_int_format
call scanf
addl $8, %esp

movl M, %eax
addl M, %eax
movl %eax, M

pushl M
pushl $printf_int_format
call printf
addl $8, %esp

.TLOOP0:
movl D, %eax
cmp M, %eax
jg .LOOP0
jmp .ENDLOOP0
.LOOP0:

movl M, %eax
addl M, %eax
movl %eax, M

pushl M
pushl $printf_int_format
call printf
addl $8, %esp

jmp .TLOOP0
.ENDLOOP0:

ret

