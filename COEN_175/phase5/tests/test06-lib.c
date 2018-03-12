void test_and_recurse(int x)
{
    if (x > 0)
	foo(x - 1);
}

void print(int x)
{
    printf("%d\n", x);
}
