extern int a[];

void foo(int n)
{
    while (-- n >= 0)
	a[n] = n;
}

void bar(int n)
{
    int i;

    for(i = 0; i < n; i ++)
	a[i] ++;
}

void foobar(int n)
{
    int i;

    for (i = 0; i < n; i ++)
	printf("%d\n", a[i]);
}
