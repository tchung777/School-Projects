void bar(void);

void foo(int x, int y)
{
    char *p, z;

    x || y;
    x || 0;
    p || z;
    foo && p;			/* invalid operands to binary && */

    z = sizeof p;
    z = sizeof bar();		/* invalid operand to unary sizeof */
    z = - p;			/* invalid operand to unary - */
}
