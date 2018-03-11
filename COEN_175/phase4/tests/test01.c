int foo(void);
int *bar(void);

int main(void)
{
    int x, *p;

    x = foo();
    p = bar();
    p = foo();		/* invalid operands to binary = */
    x = bar();		/* invalid operands to binary = */
}
