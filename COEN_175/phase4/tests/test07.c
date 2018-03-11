int main(void)
{
    int a[10], b[10], *p;

    p = &a;		/* lvalue required in expression */
    p = a;
    a[1] = 0;
    a = b;		/* lvalue required in expression */
}
