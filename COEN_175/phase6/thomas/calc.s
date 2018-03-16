lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je	.L1
	call	getchar
	movl	%eax, -16(%ebp)
	movl	-16(%ebp),%eax
	movl	%eax, c
.L1:
.L2:
.L3:
	pushl	c
	call	isspace
	addl	$4, %esp
	movl	%eax, -20(%ebp)
movl	-20(%ebp),%eax
cmpl	$0,%eax
je	.L4
	movl	c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -28(%ebp)
movl	-28(%ebp),%eax
cmpl	$0,%eax
.L4:
	setne	%al
	movzbl	%al,%eax
	movl	%eax,-24(%ebp)
	movl	-24(%ebp), %eax
	cmpl	$0, %eax
	je	.L5
	call	getchar
	movl	%eax, -32(%ebp)
	movl	-32(%ebp),%eax
	movl	%eax, c
	jmp	.L3
.L5:
	pushl	c
	call	isdigit
	addl	$4, %esp
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -40(%ebp)
	movl	-40(%ebp), %eax
	cmpl	$0, %eax
	je	.L6
	movl	c,%eax
	movl	%eax, -8(%ebp)
	movl	$0,%eax
	movl	%eax, c
	movl	-8(%ebp), %eax
	jmp	.L0
.L6:
.L7:
	movl	$0,%eax
	movl	%eax, -4(%ebp)
.L8:
	pushl	c
	call	isdigit
	addl	$4, %esp
	movl	%eax, -44(%ebp)
	movl	-44(%ebp), %eax
	cmpl	$0, %eax
	je	.L9
	movl	-4(%ebp), %eax
	imull	$10, %eax
	movl	%eax, -48(%ebp)
	movl	-48(%ebp), %eax
	addl	c, %eax
	movl	%eax,-52(%ebp)
	movl	-52(%ebp), %eax
	subl	$48, %eax
	movl	%eax, -56(%ebp)
	movl	-56(%ebp),%eax
	movl	%eax, -4(%ebp)
	call	getchar
	movl	%eax, -60(%ebp)
	movl	-60(%ebp),%eax
	movl	%eax, c
	jmp	.L8
.L9:
	movl	-4(%ebp),%eax
	movl	%eax, lexval
	movl	NUM, %eax
	jmp	.L0
.L0:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	lexan
	.set	lexan.size, 60

match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$match.size, %esp
	movl	lookahead, %eax
	cmpl	8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	$0, %eax
	je	.L11
	pushl	lookahead
	leal	.L12, %eax
	movl	%eax, -12(%ebp)
	pushl	-12(%ebp)
	call	printf
	addl	$8, %esp
	movl	%eax, -8(%ebp)
	pushl	$1
	call	exit
	addl	$4, %esp
	movl	%eax, -16(%ebp)
.L11:
.L13:
	call	lexan
	movl	%eax, -20(%ebp)
	movl	-20(%ebp),%eax
	movl	%eax, lookahead
.L10:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	match
	.set	match.size, 20

factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$factor.size, %esp
	movl	lookahead, %eax
	cmpl	$40, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$0, %eax
	je	.L15
	pushl	$40
	call	match
	addl	$4, %esp
	movl	%eax, -12(%ebp)
	call	expr
	movl	%eax, -16(%ebp)
	movl	-16(%ebp),%eax
	movl	%eax, -4(%ebp)
	pushl	$41
	call	match
	addl	$4, %esp
	movl	%eax, -20(%ebp)
	movl	-4(%ebp), %eax
	jmp	.L14
.L15:
.L16:
	movl	lexval,%eax
	movl	%eax, -4(%ebp)
	pushl	NUM
	call	match
	addl	$4, %esp
	movl	%eax, -24(%ebp)
	movl	-4(%ebp), %eax
	jmp	.L14
.L14:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	factor
	.set	factor.size, 24

term:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$term.size, %esp
	call	factor
	movl	%eax, -8(%ebp)
	movl	-8(%ebp),%eax
	movl	%eax, -4(%ebp)
.L18:
	movl	$1, %eax
	cmpl	$0, %eax
	je	.L19
	movl	lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je	.L20
	pushl	$42
	call	match
	addl	$4, %esp
	movl	%eax, -16(%ebp)
	call	factor
	movl	%eax, -20(%ebp)
	movl	-4(%ebp), %eax
	imull	-20(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp),%eax
	movl	%eax, -4(%ebp)
	jmp	.L21
.L20:
	movl	lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	cmpl	$0, %eax
	je	.L22
	pushl	$47
	call	match
	addl	$4, %esp
	movl	%eax, -32(%ebp)
	call	factor
	movl	%eax, -36(%ebp)
	movl	-4(%ebp), %eax
	movl	-36(%ebp), %ecx
	cltd	
	idivl	%ecx
	movl	%eax, -40(%ebp)
	movl	-40(%ebp),%eax
	movl	%eax, -4(%ebp)
	jmp	.L23
.L22:
	jmp		.L19
.L23:
.L21:
	jmp	.L18
.L19:
	movl	-4(%ebp), %eax
	jmp	.L17
.L17:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	term
	.set	term.size, 40

expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$expr.size, %esp
	call	term
	movl	%eax, -8(%ebp)
	movl	-8(%ebp),%eax
	movl	%eax, -4(%ebp)
.L25:
	movl	$1, %eax
	cmpl	$0, %eax
	je	.L26
	movl	lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je	.L27
	pushl	$43
	call	match
	addl	$4, %esp
	movl	%eax, -16(%ebp)
	call	term
	movl	%eax, -20(%ebp)
	movl	-4(%ebp), %eax
	addl	-20(%ebp), %eax
	movl	%eax,-24(%ebp)
	movl	-24(%ebp),%eax
	movl	%eax, -4(%ebp)
	jmp	.L28
.L27:
	movl	lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	cmpl	$0, %eax
	je	.L29
	pushl	$45
	call	match
	addl	$4, %esp
	movl	%eax, -32(%ebp)
	call	term
	movl	%eax, -36(%ebp)
	movl	-4(%ebp), %eax
	subl	-36(%ebp), %eax
	movl	%eax, -40(%ebp)
	movl	-40(%ebp),%eax
	movl	%eax, -4(%ebp)
	jmp	.L30
.L29:
	jmp		.L26
.L30:
.L28:
	jmp	.L25
.L26:
	movl	-4(%ebp), %eax
	jmp	.L24
.L24:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	expr
	.set	expr.size, 40

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	call	lexan
	movl	%eax, -8(%ebp)
	movl	-8(%ebp),%eax
	movl	%eax, lookahead
.L32:
	movl	$1, %eax
	negl	%eax
	movl	%eax, -12(%ebp)
	movl	lookahead, %eax
	cmpl	-12(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	$0, %eax
	je	.L33
	call	expr
	movl	%eax, -20(%ebp)
	movl	-20(%ebp),%eax
	movl	%eax, -4(%ebp)
	pushl	-4(%ebp)
	leal	.L34, %eax
	movl	%eax, -28(%ebp)
	pushl	-28(%ebp)
	call	printf
	addl	$8, %esp
	movl	%eax, -24(%ebp)
.L35:
	movl	lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	cmpl	$0, %eax
	je	.L36
	pushl	$10
	call	match
	addl	$4, %esp
	movl	%eax, -36(%ebp)
	jmp	.L35
.L36:
	jmp	.L32
.L33:
.L31:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	main
	.set	main.size, 36

	.data
	.comm	NUM, 4, 4
	.comm	lookahead, 4, 4
	.comm	c, 4, 4
	.comm	lexval, 4, 4
.L12:	.asciz	"syntax error at %d\n"
.L34:	.asciz	"%d\n"
