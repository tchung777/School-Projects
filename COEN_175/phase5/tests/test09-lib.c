extern int z;

void print(int a, int b, int c, int d, int e, int f)
{
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    printf("%d\n", d);
    printf("%d\n", e);
    printf("%d\n", f);
    printf("%d\n", z);
}

void bar(int a, int b, int c, int d, int e, int f)
{
    foo(a, b, c, d, e, f, a + b, b - e, c + d, f - c);
}
