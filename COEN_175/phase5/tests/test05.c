/* can you assign to both locals and globals? */

int x, y;
void foo(int a, int b, int c);

int main(void)
{
    int z;

    x = 1;
    y = 2;
    z = 3;
    foo(x, y, z);
}
