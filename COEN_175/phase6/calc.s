_lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	_c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
	cmpl	$0, -4(%ebp)
	je	.IF0
	call	_getchar
	movl	, %eax
.IF0:
	jmp	.WHILE1
.WHILE0:
	call	_getchar
	movl	, %eax
.WHILE1:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isspace
	cmpl	$0, 
	je	.AND1
	movl	_c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	.AND1
	movl	$1, %eax
	jmp	.AND0
.AND1:
	movl	$0, %eax
.AND0:
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	jne	.WHILE0
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isdigit
	movl	, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	.IF1
	movl	_c, %eax
	movl	$0, %eax
	movl	-8(%ebp), %eax
	jmp	
.IF1:
	movl	$0, %eax
	jmp	.WHILE3
.WHILE2:
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	addl	_c, %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	subl	$48, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	call	_getchar
	movl	, %eax
.WHILE3:
	movl	_c, %eax
	movl	%eax, 0(%esp)
	call	_isdigit
	cmpl	$0, 
	jne	.WHILE2
	movl	-4(%ebp), %eax
	movl	_NUM, %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_lexan
	.set	lexan.size, 24

_match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$match.size, %esp
	movl	_lookahead, %eax
	cmpl	8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -32(%ebp)
	cmpl	$0, -32(%ebp)
	je	.IF2
	movl	_lookahead, %eax
	movl	%eax, 4(%esp)
	leal	.L0, %eax
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
	movl	$1, %eax
	movl	%eax, 0(%esp)
	call	_exit
.IF2:
	call	_lexan
	movl	, %eax
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_match
	.set	match.size, 8

_factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$factor.size, %esp
	movl	_lookahead, %eax
	cmpl	$40, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -40(%ebp)
	cmpl	$0, -40(%ebp)
	je	.IF3
	movl	$40, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_expr
	movl	, %eax
	movl	$41, %eax
	movl	%eax, 0(%esp)
	call	_match
	movl	-4(%ebp), %eax
	jmp	
.IF3:
	movl	_lexval, %eax
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
	jmp	.WHILE5
.WHILE4:
	movl	_lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -44(%ebp)
	cmpl	$0, -44(%ebp)
	je	.IF5
	movl	$42, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	imull	, %eax
	movl	%eax, -48(%ebp)
	movl	-48(%ebp), %eax
	jmp	.IF4
.IF5:
	movl	_lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -52(%ebp)
	cmpl	$0, -52(%ebp)
	je	.IF7
	movl	$47, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_factor
	movl	-4(%ebp), %eax
	movl	, %ecx
	cltd
	idivl	%ecx
	movl	%eax, -56(%ebp)
	movl	-56(%ebp), %eax
	jmp	.IF6
.IF7:
.IF6:
.IF4:
.WHILE5:
	movl	$1, %eax
	cmpl	$0, %eax
	jne	.WHILE4
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
	jmp	.WHILE7
.WHILE6:
	movl	_lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -60(%ebp)
	cmpl	$0, -60(%ebp)
	je	.IF9
	movl	$43, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	addl	, %eax
	movl	%eax, -64(%ebp)
	movl	-64(%ebp), %eax
	jmp	.IF8
.IF9:
	movl	_lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -68(%ebp)
	cmpl	$0, -68(%ebp)
	je	.IF11
	movl	$45, %eax
	movl	%eax, 0(%esp)
	call	_match
	call	_term
	movl	-4(%ebp), %eax
	subl	, %eax
	movl	%eax, -72(%ebp)
	movl	-72(%ebp), %eax
	jmp	.IF10
.IF11:
.IF10:
.IF8:
.WHILE7:
	movl	$1, %eax
	cmpl	$0, %eax
	jne	.WHILE6
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
	jmp	.WHILE9
.WHILE8:
	call	_expr
	movl	, %eax
	movl	-4(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	.L1, %eax
	movl	%eax, -76(%ebp)
	movl	-76(%ebp), %eax
	movl	%eax, 0(%esp)
	call	_printf
	jmp	.WHILE11
.WHILE10:
	movl	$10, %eax
	movl	%eax, 0(%esp)
	call	_match
.WHILE11:
	movl	_lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -80(%ebp)
	cmpl	$0, -80(%ebp)
	jne	.WHILE10
.WHILE9:
	movl	$1, %eax
	negl	%eax
	movl	%eax, -84(%ebp)
	movl	_lookahead, %eax
	cmpl	-84(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -88(%ebp)
	cmpl	$0, -88(%ebp)
	jne	.WHILE8
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	_main
	.set	main.size, 24

	.data
	.comm	_NUM, 4, 4
	.comm	_lookahead, 4, 4
	.comm	_c, 4, 4
	.comm	_lexval, 4, 4
