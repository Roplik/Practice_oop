import std;
using namespace std;


int gcd(int a, int b)
{
    if(b != 0)
        return gcd(b, a % b);
    return a;
}
// int lcm(int a, int b)
// {
//     if(a == 0 || b == 0)
//         return 0;
//     return (abs(a)/ gcd(a,b)) * abs(b);
// }




class Fraction
{
    int numerator   = 0;
    int denominator = 0;
    public:
        Fraction(int number)
        {
            numerator   = number;
            denominator = 1;
        }
        Fraction(int num, int den)
        {
            numerator   = num;
            denominator = den;
            reduction();
        }
        int getNumerator() const
        {
            return numerator;
        }
        int getDenominator() const
        {
            return denominator;
        }
        void print() const
        {
            println("{}/{}", numerator, denominator);
        }
    private:
        void reduction()
        {
            if(numerator == 0)
            {
                denominator = 1;
                return;
            }

            if(denominator < 0)
            {
                numerator   = -numerator;
                denominator = -denominator;
            }
            int divisor  = gcd(numerator, denominator);
            numerator   /= divisor;
            denominator /= divisor;
            return;
        }
};

Fraction summFractions(const Fraction& a, const Fraction& b)
{
    Fraction res(a.getNumerator() * b.getDenominator() + b.getNumerator() * a.getDenominator(), a.getDenominator() * b.getDenominator());
    return res;
}

Fraction productFractions(const Fraction& a, const Fraction& b)
{
    Fraction res(a.getNumerator() * b.getNumerator(), a.getDenominator() * b.getDenominator());
    return res;
}

int main()
{   
    Fraction a(2, 5);
    Fraction b(3, 4);
    Fraction res = productFractions(a, b);
    res.print();
    return 0;
}