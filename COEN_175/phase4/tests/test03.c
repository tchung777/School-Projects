int foo(void)
{
    int x, y, *z;

    x + y;
    x - y;
    x / y;
    x * 'a';
    x == y;
    x < y;
    x > y;
    'b' >= y;
    x <= y;
    !x;

    x + z;
    x % z;		/* invalid operands to binary %  */
    x / z;		/* invalid operands to binary /  */
    z * x;		/* invalid operands to binary *  */
    x != z;		/* invalid operands to binary != */
    z < x;		/* invalid operands to binary <  */
    x > z;		/* invalid operands to binary >  */
    x >= z;		/* invalid operands to binary >= */
    x <= z;		/* invalid operands to binary <= */
    !z;
}
