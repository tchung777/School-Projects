/* can you handle parameters? */

void test_and_recurse(int x), print(int x);

int foo(int x)
{
    test_and_recurse(x);
    print(x);
}

int main(void)
{
    int n;

    n = 8;
    foo(n);
}
