int main(void)
{
    int a, b, c, *q;
    void *x, *y, *z, **p;

    a = x;			/* invalid operands to binary = */
    y = b;			/* invalid operands to binary = */
    p = q;			/* invalid operands to binary = */
    *p = q;
    c = **p;			/* invalid operand to unary * */
    y = z;
    q = z;
    a = x[1];			/* invalid operands to binary [] */
    x = p[1];
}
