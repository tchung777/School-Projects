_lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	_c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je		.L1
	call	_getchar
	movl	, %eax
	movl	%eax, _c
.L1:
.L2:
.L3:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isspace
movl	,%eax
cmpl	$0,%eax
je	.L5
	movl	_c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
movl	-16(%ebp),%eax
cmpl	$0,%eax
.L5:
	setne	%al
	movzbl	%al,%eax
	movl	%eax,-20(%ebp)
	movl	-20(%ebp), %eax
	cmpl	$0, %eax
	je		.L4
	call	_getchar
	movl	, %eax
	movl	%eax, _c
	jmp		.L3
.L4:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isdigit
	movl	, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	cmpl	$0, %eax
	je		.L6
	movl	_c, %eax
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	movl	%eax, _c
	movl	-8(%ebp), %eax
	jmp		.L0
.L6:
.L7:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L8:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isdigit
	movl	, %eax
	cmpl	$0, %eax
	je		.L9
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	addl	_c, %eax
	movl	%eax,-32(%ebp)
	movl	-32(%ebp), %eax
	subl	$48, %eax
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	movl	%eax, -4(%ebp)
	call	_getchar
	movl	, %eax
	movl	%eax, _c
	jmp		.L8
.L9:
	movl	-4(%ebp), %eax
	movl	%eax, _lexval
	movl	_NUM, %eax
	jmp		.L0
.L0:
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
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	$0, %eax
	je		.L11
	movl	_lookahead, %eax
	movl	%eax, 4(%esp)
	leal	.L13, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
	movl	$1, %eax
	movl	%eax, 0(%esp)
	call	_exit
.L11:
.L12:
	call	_lexan
	movl	, %eax
	movl	%eax, _lookahead
.L10:
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
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je		.L15
	movl	$40, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_expr
	movl	, %eax
	movl	%eax, -4(%ebp)
	movl	$41, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp		.L14
.L15:
.L16:
	movl	_lexval, %eax
	movl	%eax, -4(%ebp)
	movl	_NUM, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp		.L14
.L14:
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
	movl	%eax, -4(%ebp)
.L18:
	movl	$1, %eax
	cmpl	$0, %eax
	je		.L19
	movl	_lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je		.L20
	movl	$42, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	imull	, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		.L21
.L20:
	movl	_lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	$0, %eax
	je		.L22
	movl	$47, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	movl	, %ecx
	cltd
	idivl	%ecx
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		.L23
.L22:
	jmp		.L19
.L23:
.L21:
	jmp		.L18
.L19:
	movl	-4(%ebp), %eax
	jmp		.L17
.L17:
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
	movl	%eax, -4(%ebp)
.L25:
	movl	$1, %eax
	cmpl	$0, %eax
	je		.L26
	movl	_lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je		.L27
	movl	$43, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	addl	, %eax
	movl	%eax,-12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		.L28
.L27:
	movl	_lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	$0, %eax
	je		.L29
	movl	$45, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	subl	, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		.L30
.L29:
	jmp		.L26
.L30:
.L28:
	jmp		.L25
.L26:
	movl	-4(%ebp), %eax
	jmp		.L24
.L24:
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
	movl	%eax, _lookahead
.L32:
	movl	$1, %eax
	negl	%eax
	movl	%eax, -8(%ebp)
	movl	_lookahead, %eax
	cmpl	-8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je		.L33
	call	_expr
	movl	, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	.L34, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
.L35:
	movl	_lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	cmpl	$0, %eax
	je		.L36
	movl	$10, %eax
	movl	%eax, 0(%esp)
	call	_match
	jmp		.L35
.L36:
	jmp		.L32
.L33:
.L31:
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
.L13:	.asciz	"syntax error at %d\n"
.L34:	.asciz	"%d\n"
