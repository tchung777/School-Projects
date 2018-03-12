/* can you call functions with different numbers of arguments? */

void none(void), one(int a), two(int a, int b), three(int a, int b, int c);

int main(void)
{
    none();
    one(1);
    two(1, 2);
    three(1, 2, 3);
}
