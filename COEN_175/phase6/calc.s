	.globl	lexan
lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
	cmpl	$0, -4(%ebp)
	je	.IF0
	call	getchar
	movl	, %eax
	movl	%eax, c
.IF0:
	jmp	.WHILE1
.WHILE0:
	call	getchar
	movl	, %eax
	movl	%eax, c
.WHILE1:
	pushl	c
	call	isspace
	addl	$4, %esp
	cmpl	$0, 
	je	.AND1
	movl	c, %eax
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
	pushl	c
	call	isdigit
	addl	$4, %esp
	movl	, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	.IF1
	movl	c, %eax
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	movl	%eax, c
	movl	-8(%ebp), %eax
	jmp	
.IF1:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
	jmp	.WHILE3
.WHILE2:
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	addl	c, %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	subl	$48, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	movl	%eax, -4(%ebp)
	call	getchar
	movl	, %eax
	movl	%eax, c
.WHILE3:
	pushl	c
	call	isdigit
	addl	$4, %esp
	cmpl	$0, 
	jne	.WHILE2
	movl	-4(%ebp), %eax
	movl	%eax, lexval
	movl	NUM, %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret
	.globl	match
match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0, %esp
	movl	lookahead, %eax
	cmpl	8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -32(%ebp)
	cmpl	$0, -32(%ebp)
	je	.IF2
	pushl	lookahead
	leal	.L0, %eax
	movl	%eax, -36(%ebp)
	pushl	-36(%ebp)
	call	printf
	addl	$8, %esp
	pushl	$1
	call	exit
	addl	$4, %esp
.IF2:
	call	lexan
	movl	, %eax
	movl	%eax, lookahead
	movl	%ebp, %esp
	popl	%ebp
	ret
	.globl	factor
factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	lookahead, %eax
	cmpl	$40, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -40(%ebp)
	cmpl	$0, -40(%ebp)
	je	.IF3
	pushl	$40
	call	match
	addl	$4, %esp
	call	expr
	movl	, %eax
	movl	%eax, -4(%ebp)
	pushl	$41
	call	match
	addl	$4, %esp
	movl	-4(%ebp), %eax
	jmp	
.IF3:
	movl	lexval, %eax
	movl	%eax, -4(%ebp)
	pushl	NUM
	call	match
	addl	$4, %esp
	movl	-4(%ebp), %eax
	jmp	
	movl	%ebp, %esp
	popl	%ebp
	ret
	.globl	term
term:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	call	factor
	movl	, %eax
	movl	%eax, -4(%ebp)
	jmp	.WHILE5
.WHILE4:
	movl	lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -44(%ebp)
	cmpl	$0, -44(%ebp)
	je	.IF5
	pushl	$42
	call	match
	addl	$4, %esp
	call	factor
	movl	-4(%ebp), %eax
	imull	, %eax
	movl	%eax, -48(%ebp)
	movl	-48(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.IF4
.IF5:
	movl	lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -52(%ebp)
	cmpl	$0, -52(%ebp)
	je	.IF7
	pushl	$47
	call	match
	addl	$4, %esp
	call	factor
	movl	-4(%ebp), %eax
	movl	, %ecx
	cltd
	idivl	%ecx
	movl	%eax, -56(%ebp)
	movl	-56(%ebp), %eax
	movl	%eax, -4(%ebp)
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
	.globl	expr
expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	call	term
	movl	, %eax
	movl	%eax, -4(%ebp)
	jmp	.WHILE7
.WHILE6:
	movl	lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -60(%ebp)
	cmpl	$0, -60(%ebp)
	je	.IF9
	pushl	$43
	call	match
	addl	$4, %esp
	call	term
	movl	-4(%ebp), %eax
	addl	, %eax
	movl	%eax, -64(%ebp)
	movl	-64(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.IF8
.IF9:
	movl	lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -68(%ebp)
	cmpl	$0, -68(%ebp)
	je	.IF11
	pushl	$45
	call	match
	addl	$4, %esp
	call	term
	movl	-4(%ebp), %eax
	subl	, %eax
	movl	%eax, -72(%ebp)
	movl	-72(%ebp), %eax
	movl	%eax, -4(%ebp)
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
	.globl	main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	call	lexan
	movl	, %eax
	movl	%eax, lookahead
	jmp	.WHILE9
.WHILE8:
	call	expr
	movl	, %eax
	movl	%eax, -4(%ebp)
	pushl	-4(%ebp)
	leal	.L1, %eax
	movl	%eax, -76(%ebp)
	pushl	-76(%ebp)
	call	printf
	addl	$8, %esp
	jmp	.WHILE11
.WHILE10:
	pushl	$10
	call	match
	addl	$4, %esp
.WHILE11:
	movl	lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -80(%ebp)
	cmpl	$0, -80(%ebp)
	jne	.WHILE10
.WHILE9:
	movl	lookahead, %eax
	cmpl	$-1, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -88(%ebp)
	cmpl	$0, -88(%ebp)
	jne	.WHILE8
	movl	%ebp, %esp
	popl	%ebp
	ret
	.data 
	.comm	NUM, 4, 4
	.comm	lookahead, 4, 4
	.comm	c, 4, 4
	.comm	lexval, 4, 4
