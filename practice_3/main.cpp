import std;
using namespace std;

//задача №5 из практики №2
bool Rook(int x1, int y1, int x2, int y2)
{
    if((x1 == x2 && y1 != y2) || (y1 == y2 && x1 != x2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int invert_digits(unsigned int n)
{
    unsigned int res = 0;
    while(n > 0)
    {
        int digit = n % 10;
        res = res * 10 + digit;
        n /= 10;
    }
    return res;
}

int add_right_digit(int n, int d)
{
    return n * 10 + d;
}

bool is_prime(int a)
{
    if(a == 1)
        return false;
    if(a == 2)
        return true;
    if(a % 2 == 0)
        return false;
    
    for(int i = 3; i <= sqrt(a) + 1; ++i)
    {
        if(a % i == 0)
            return false;
    }
    return true;
}

int fact(int a)
{
    if(a <= 1)
        return 1;
    return a * fact(a - 1);
}

int gcd(int a, int b)
{
    if(b != 0)
        return gcd(b, a % b);
    return a;
}

int digit_sum(int a)
{
    if(a < 0)
        a = -a;
    if(a == 0)
        return 0;
    return a % 10 + digit_sum(a / 10);
}

int main()
{
    int a;
    cin >> a;
    print("{}", digit_sum(a));
}