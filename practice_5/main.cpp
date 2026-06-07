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

        Fraction& operator+=(const Fraction& second)
        {
            numerator   = numerator * second.getDenominator() + second.getNumerator() * denominator;
            denominator = denominator * second.getDenominator();
            reduction();
            return *this;

        }
        Fraction& operator-=(const Fraction& second)
        {
            numerator   = numerator * second.getDenominator() - second.getNumerator() * denominator;
            denominator = denominator * second.getDenominator();
            reduction();
            return *this;
        }
        Fraction& operator*=(const Fraction& second)
        {
            numerator   *= second.getNumerator();
            denominator *= second.getDenominator();
            reduction();
            return *this;
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

Fraction operator+(const Fraction& first, const Fraction& second)
{
    Fraction res = first;
    res += second;
    return res; // Или можно return Fraction(first) += second; 
}
Fraction operator-(const Fraction& first, const Fraction& second)
{
    Fraction res = first;
    res -= second;
    return res;
}
Fraction operator*(const Fraction& first, const Fraction& second)
{
    Fraction res = first;
    res *= second;
    return res; 
}


Fraction& operator++(Fraction& first)
{
    first += Fraction(1);
    return first; 
}

Fraction operator++(Fraction& first, int)
{
    Fraction old(first);
    first += Fraction(1);
    return old;
}

std::ostream& operator<<(std::ostream& os,const Fraction& f)
{
    os << f.getNumerator() << "/" << f.getDenominator();
    return os;
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
    Fraction a(1);
    Fraction b(2);
    Fraction c(3);

    a += 10;
    println("a: {}", a);

    b -= 20;
    println("b: {}", b);

    c *= 3;
    println("c: {}", c);

    Fraction d = a + b;
    println("d: {}", d);

    Fraction e(1);
    e++;
    println("e1: {}", e);
    println("e2: {}", ++e);

    Fraction f(8);
    cout << "f: " << f << endl;
    
    Fraction g(10);
    auto demonstrationSS = [](const Fraction& f)
    {
        stringstream ss;
        ss << f;
        return ss.str();
    };
    println("g: {}", demonstrationSS(g));

    ofstream outFile("test.txt");
    if(outFile.is_open())
    {
        outFile << "Дроби: "  << endl;
        outFile << "a: " << a << endl;
        outFile << "b: " << b << endl;
        outFile << "a + b: " << a + b << endl;
        outFile.close();
    }

    ifstream inFile("test.txt");
    if(inFile.is_open())
    {
        string line;
        while(getline(inFile,line))
        {
            println("{}",line);
        }
        inFile.close();
    }
    return 0;
}