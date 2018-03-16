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
	je		_L1
	call	_getchar
	movl	, %eax
	movl	%eax, _c
_L1:
_L2:
_L3:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isspace
movl	,%eax
cmpl	$0,%eax
je	_L5
	movl	_c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
movl	-16(%ebp),%eax
cmpl	$0,%eax
_L5:
	setne	%al
	movzbl	%al,%eax
	movl	%eax,-20(%ebp)
	movl	-20(%ebp), %eax
	cmpl	$0, %eax
	je		_L4
	call	_getchar
	movl	, %eax
	movl	%eax, _c
	jmp		_L3
_L4:
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
	je		_L6
	movl	_c, %eax
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	movl	%eax, _c
	movl	-8(%ebp), %eax
	jmp	_L0
_L6:
_L7:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
_L8:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isdigit
	movl	, %eax
	cmpl	$0, %eax
	je		_L9
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	addl	_c, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	subl	$48, %eax
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	movl	%eax, -4(%ebp)
	call	_getchar
	movl	, %eax
	movl	%eax, _c
	jmp		_L8
_L9:
	movl	-4(%ebp), %eax
	movl	%eax, _lexval
	movl	_NUM, %eax
	jmp	_L0
_L0:
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
	je		_L11
	movl	_lookahead, %eax
	movl	%eax, 4(%esp)
	leal	, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
	movl	$1, %eax
	movl	%eax, 0(%esp)
	call	_exit
_L11:
_L12:
	call	_lexan
	movl	, %eax
	movl	%eax, _lookahead
_L10:
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
	je		_L15
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
	jmp	_L14
_L15:
_L16:
	movl	_lexval, %eax
	movl	%eax, -4(%ebp)
	movl	_NUM, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp	_L14
_L14:
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
_L18:
	movl	$1, %eax
	cmpl	$0, %eax
	je		_L19
	movl	_lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je		_L20
	movl	$42, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	imull	, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		_L21
_L20:
	movl	_lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	$0, %eax
	je		_L22
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
	jmp		_L23
_L22:
	jmp		_L19
_L23:
_L21:
	jmp		_L18
_L19:
	movl	-4(%ebp), %eax
	jmp	_L17
_L17:
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
_L25:
	movl	$1, %eax
	cmpl	$0, %eax
	je		_L26
	movl	_lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je		_L27
	movl	$43, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	addl	, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		_L28
_L27:
	movl	_lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	$0, %eax
	je		_L29
	movl	$45, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	subl	, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp		_L30
_L29:
	jmp		_L26
_L30:
_L28:
	jmp		_L25
_L26:
	movl	-4(%ebp), %eax
	jmp	_L24
_L24:
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
_L32:
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
	je		_L33
	call	_expr
	movl	, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
_L35:
	movl	_lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	cmpl	$0, %eax
	je		_L36
	movl	$10, %eax
	movl	%eax, 0(%esp)
	call	_match
	jmp		_L35
_L36:
	jmp		_L32
_L33:
_L31:
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
