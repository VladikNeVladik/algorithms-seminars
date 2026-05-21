int float_test(void)
{
    float fl32_a = 1.0;
    float fl32_b = 1.41421;

    fl32_a = fl32_b + 1.0;

    double fl64_a = 1.0;
    double fl64_b = 1.41421;

    fl64_a = fl64_b + 1.0;

    if (fl32_a < fl32_b)
    {
        return 0;
    }

    return -1;
}

int g(int a)
{
    return (int) (2.0 * a);
}

float f(float a, int b)
{
    return a * g(b);
}
