int f(int x)
{
    return "hello world\n"[10];
}

int g(int y, int z, int *p)
{
    return f(y);
}

int h(int z)
{
    return g(z, 0, 0);
}

int main(void)
{
    f(g(h(123), 10, 0));
}
