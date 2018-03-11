int main(void)
{
    int *p, **q, x;

    *p = 1;
    *q = 1;		/* invalid operands to binary = */
    **q = 1;
    *q = x;		/* invalid operands to binary = */
    ***q = 1;		/* invalid operand to unary * */
    q[1][x] = 1;
}
