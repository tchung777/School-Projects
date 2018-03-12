_lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	_c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%EBP)
	cmpl	$0, -12(%EBP)
	je	_L0
	call	_getchar
	movl	, %eax
_L0:
	jmp	_L3
_L2:
	call	_getchar
	movl	, %eax
_L3:
	movl	%eax, 0(%esp)
	call	_isspace
	cmpl	$0, 
	je	_L5
	movl	_c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -20(%EBP)
	cmpl	$0, -20(%EBP)
	je	_L5
	movl	$1, %eax
	jmp	_L4
_L5:
	movl	$0, %eax
_L4:
	movl	%eax, -16(%EBP)
	cmpl	$0, -16(%EBP)
	jne	_L2
	movl	%eax, 0(%esp)
	call	_isdigit
	movl	, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -24(%EBP)
	cmpl	$0, -24(%EBP)
	je	_L6
	movl	_c, %eax
	movl	$0, %eax
	movl	-8(%ebp), %eax
	jmp	0x0
_L6:
	movl	$0, %eax
	jmp	_L9
_L8:
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -28(%EBP)
	movl	-28(%EBP), %eax
	addl	_c, %eax
	movl	%eax, -32(%EBP)
	movl	-32(%EBP), %eax
	subl	$48, %eax
	movl	%eax, -36(%EBP)
	movl	-36(%EBP), %eax
	call	_getchar
	movl	, %eax
_L9:
	movl	%eax, 0(%esp)
	call	_isdigit
	cmpl	$0, 
	jne	_L8
	movl	-4(%ebp), %eax
	movl	_NUM, %eax
	jmp	0x0
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_lexan
	.set	lexan.size, 40

_match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$match.size, %esp
	movl	_lookahead, %eax
	cmpl	8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -4(%EBP)
	cmpl	$0, -4(%EBP)
	je	_L10
	movl	%eax, 4(%esp)
	leal	, %eax
	movl	%eax, -8(%EBP)
	movl	%eax, 0(%esp)
	call	_printf
	movl	%eax, 0(%esp)
	call	_exit
_L10:
	call	_lexan
	movl	, %eax
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_match
	.set	match.size, 24

_factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$factor.size, %esp
	movl	_lookahead, %eax
	cmpl	$40, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%EBP)
	cmpl	$0, -8(%EBP)
	je	_L13
	movl	%eax, 0(%esp)
	call	_match
	call	_expr
	movl	, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp	0x0
_L13:
	movl	_lexval, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp	0x0
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_factor
	.set	factor.size, 24

_term:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$term.size, %esp
	call	_factor
	movl	, %eax
	jmp	_L16
_L15:
	movl	_lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%EBP)
	cmpl	$0, -8(%EBP)
	je	_L19
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	imull	, %eax
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	jmp	_L17
_L19:
	movl	_lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%EBP)
	cmpl	$0, -16(%EBP)
	je	_L22
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	movl	, %ecx
	cltd
	idivl	%ecx
	movl	%eax, -20(%EBP)
	movl	-20(%EBP), %eax
	jmp	_L20
_L22:
_L20:
_L17:
_L16:
	movl	$1, %eax
	cmpl	$0, %eax
	jne	_L15
	movl	-4(%ebp), %eax
	jmp	0x0
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_term
	.set	term.size, 24

_expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$expr.size, %esp
	call	_term
	movl	, %eax
	jmp	_L24
_L23:
	movl	_lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%EBP)
	cmpl	$0, -8(%EBP)
	je	_L27
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	addl	, %eax
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	jmp	_L25
_L27:
	movl	_lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%EBP)
	cmpl	$0, -16(%EBP)
	je	_L30
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	subl	, %eax
	movl	%eax, -20(%EBP)
	movl	-20(%EBP), %eax
	jmp	_L28
_L30:
_L28:
_L25:
_L24:
	movl	$1, %eax
	cmpl	$0, %eax
	jne	_L23
	movl	-4(%ebp), %eax
	jmp	0x0
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_expr
	.set	expr.size, 24

_main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	call	_lexan
	movl	, %eax
	jmp	_L32
_L31:
	call	_expr
	movl	, %eax
	movl	%eax, 4(%esp)
	leal	, %eax
	movl	%eax, -8(%EBP)
	movl	%eax, 0(%esp)
	call	_printf
	jmp	_L35
_L34:
	movl	%eax, 0(%esp)
	call	_match
_L35:
	movl	_lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%EBP)
	cmpl	$0, -12(%EBP)
	jne	_L34
_L32:
	movl	$1, %eax
	negl	%eax
	movl	%eax, -16(%EBP)
	movl	_lookahead, %eax
	cmpl	-16(%EBP), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -20(%EBP)
	cmpl	$0, -20(%EBP)
	jne	_L31
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_main
	.set	main.size, 40

	.data
	.comm	_NUM, 4, 4
	.comm	_lookahead, 4, 4
	.comm	_c, 4, 4
	.comm	_lexval, 4, 4
