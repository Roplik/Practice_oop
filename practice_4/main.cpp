import std;
using namespace std;


int gcd(int a, int b)
{
    if(b != 0)
        return gcd(b, a % b);
    return a;
}

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

template<>
struct std::formatter<Fraction> : std::formatter<std::string_view> 
{
    auto format(const Fraction& f, std::format_context& ctx) const 
    {
        std::string temp = std::format("{}/{}", f.getNumerator(), f.getDenominator());    
        return std::formatter<std::string_view>::format(temp, ctx);
    }
};

int main()
{   
    Fraction a(2, 5);
    Fraction b(3, 4);
    Fraction res = productFractions(a, b);
    
    // 1. Старый метод работает
    res.print(); 
    
    // 2. Теперь работает напрямую в println!
    println("Результат умножения: {}", res); 
    
    // 3. Работает и через std::format, если нужна строка
    string s = format("Дробь: {}", a);
    println("{}", s);

    return 0;
}