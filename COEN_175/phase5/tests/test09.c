/* can you really handle parameters? */

int z;
void bar(int a, int b, int c, int d, int e, int f);
void print(int a, int b, int c, int d, int e, int f);

int test(int a, int b, int c, int d, int e, int f, int g, int h)
{
    int x, ai[100], y;

    x = 11;
    y = 17;
    print(h, 34, 153, x, y, e);
}

int foo(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
     a = 1;
     b = 2;
     c = 3;
     d = 4;
     e = 5;
     test(1, 3, i, j, h, g, 341, a);
     test(a, b, e, 18, c, d, j, h);
}

int main(void)
{
    int x, y;

    z = 10;
    x = 123;
    y = 456;
    bar(z, x, 3, 4, 1, 3);
}
