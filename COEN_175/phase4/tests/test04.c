int foo(void)
{
    int x;
    int *p;
    int **q;
    int ***z;

    x == q;		/* invalid operands to binary == */
    z == 1;		/* invalid operands to binary == */
    p != q;		/* invalid operands to binary != */
    p = &x;
    p = q;		/* invalid operands to binary = */
    p + x;
    q - x;
    x + q;
    q = &z;		/* invalid operands to binary = */
    q = &p;
    z = &q;
}
