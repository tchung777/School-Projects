/* can you assign to globals? */

int x, y, z;
int putchar(int c);

int main(void)
{
    x = 104;
    putchar(x);
    y = 105;
    putchar(y);
    z = 10;
    putchar(z);
}
