int f(int x, int y, int *z)
{
    f(x, y, z);
    f(z, x, y);			/* invalid arguments to called function */
    f();			/* invalid arguments to called function */
    f(y, x, z);
}

int g(int x, int y, ...)
{
    g(x, y);
    g(x, y, 10);
    g(x);			/* invalid arguments to called function */
}

int main(void)
{
    int i, j, *p;

    p();			/* called object is not a function */
    f(1, 2, 3);			/* invalid arguments to called function */
    f(i, j, p);
    g(i, p);			/* invalid arguments to called function */
}
