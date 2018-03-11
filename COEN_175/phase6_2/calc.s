_lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, _lexval
	movl	_NUM, %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_lexan
	.set	lexan.size, 8

_match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$match.size, %esp
	call	_lexan
	movl	, %eax
	movl	%eax, _lookahead
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_match
	.set	match.size, 8

_factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$factor.size, %esp
	movl	_lexval, %eax
	movl	%eax, -4(%ebp)
	movl	_NUM, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_factor
	.set	factor.size, 8

_term:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$term.size, %esp
	call	_factor
	movl	, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_term
	.set	term.size, 8

_expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$expr.size, %esp
	call	_term
	movl	, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_expr
	.set	expr.size, 8

_main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	call	_lexan
	movl	, %eax
	movl	%eax, _lookahead
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_main
	.set	main.size, 8

	.data
	.comm	_NUM, 4, 4
	.comm	_lookahead, 4, 4
	.comm	_c, 4, 4
	.comm	_lexval, 4, 4
