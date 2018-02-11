int main(void)
{
    int a[10];
    int b[10], i;

    for (i = sizeof(a) - 1; i >= 0; i = i - 1) {
	a[i] = b[i];

	if (a[i] == 0)
	    break;
    }

    return sizeof(sizeof b);
}
