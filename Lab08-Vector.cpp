#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
using namespace std;

class Vector : public vector<double>
{
public:
    /*
     * Konstruktor. Tworzy wektor n-elementowy (jeżeli n>0)
     */
    Vector(int n = 0);

    /*
     * Mnoży każdy element wektora przez arg (inplace)
     */
    Vector &operator*=(double arg);

    /*
     * Dodaje do każdego elementu wektora arg (inplace)
     */
    Vector &operator+=(double arg);

    /*
     * Zwraca wektor, którego elementy są równe sumie
     * wartości elementów danego wektora i argumentu arg
     */
    Vector operator+(double arg) const;

    /*
     * Zwraca wektor, którego elementy są równe iloczynowi
     * wartości elementów danego wektora i argumentu arg
     */
    Vector operator*(double arg) const;

    /*
     * Jeżeli rozmoary są różne wyrzuca wyjątek
     * Dodaje do każdego i-tego elementu danego wektora i-ty element
     * argumentu arg (inplace)
     */
    Vector &operator+=(const Vector &arg);

    /*
     * Jeżeli rozmoary są różne wyrzuca wyjątek
     * Odejmuje od każdego i-tego elementu danego wektora i-ty element
     * argumentu arg (inplace)
     */
    Vector &operator-=(const Vector &arg);

    /*
     * Jeżeli rozmiary są różne wyrzuca wyjątek
     * Mnoży każdy i-ty element danego wektora przez i-ty element
     * argumentu arg (inplace)
     */
    Vector &operator*=(const Vector &arg);

    /*
     * Zwraca wektor, którego elementy są iloczynami odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator*(const Vector &arg) const;

    /*
     * Zwraca wektor, którego elementy są równe sumie odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator+(const Vector &arg) const;

    /*
     * Zwraca wektor, którego elementy są równe różnicy odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator-(const Vector &arg) const;

    /*
     * Zwraca iloczyn skalarny danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne
     */
    double operator|(const Vector &arg) const;

    /*
     * Oblicza sume elementów wektora
     */
    double sum() const;

    /*
     * Oblicza wartość średnią elementów wektora
     */
    double mean() const;

    /*
     * Oblicza odchylenie standardowe
     */
    double std() const;

    /*
     * Zwraca wektor, którego elementy są wynikiem wywołania funkcji apply
     * na elementach oryginalnego wektora
     */

    Vector apply(double (*f)(double)) const;

    /*
     * Zwraca wektor, którego elementy sa równe elementowi danego wektora
     * podniesionymi do potęgi arg
     */
    Vector operator^(double arg) const;

    /*
     * Zwraca wektor o rozmiarz n wypełnionym zerami
     */
    static Vector zeros(int n);
    /*
     * Zwraca wektor o rozmiarz n wypełnionym losowymi wartościami z zakresu 0-1
     */
    static Vector random(int n);

    /*
     * Zwraca wektor wypełniony wartościami z zakresu start (włącznie) do stop (wyłącznie)
     * Różnica kolejnych wartości wynosi step
     */

    static Vector range(double stop, double start = 0, double step = 1);
};

/*
 * Zapisuje zawartość wektora do strumienia os w formacie
 * [v_0, v_1,     v_last]
 */
ostream &operator<<(ostream &os, const Vector &v);

/*
 * Czyta wektor ze strumienia is zapisany w powyższym formacie
 * [v_0, v_1,     v_last]
 */
istream &operator>>(istream &is, Vector &v);

// Kod metod
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector::Vector(int n) : vector<double>(n > 0 ? n : 0)
{
}

Vector &Vector::operator*=(double arg)
{
    for (auto &e : *this)
        e *= arg;
    return *this;
}

Vector Vector::operator*(double arg) const
{
    Vector ret(*this);
    ret *= arg;
    return ret;
}

Vector &Vector::operator+=(double arg)
{
    for (auto &e : *this)
        e += arg;
    return *this;
}

Vector Vector::operator+(double arg) const
{
    Vector ret(*this);
    ret += arg;
    return ret;
}

Vector &Vector::operator*=(const Vector &arg)
{
    if (size() != arg.size())
        throw "bad size";
    for (int i = 0; i < size(); i++)
        (*this)[i] *= arg[i];
    return *this;
}
Vector &Vector::operator+=(const Vector &arg)
{
    if (size() != arg.size())
        throw "bad size";
    for (int i = 0; i < size(); i++)
    {
        (*this)[i] += arg[i];
    }
    return *this;
}

Vector &Vector::operator-=(const Vector &arg)
{
    if (size() != arg.size())
        throw "bad size";
    for (int i = 0; i < size(); i++)
    {
        (*this)[i] -= arg[i];
    }
    return *this;
}

Vector Vector::operator+(const Vector &arg) const
{
    Vector ret(*this);
    ret += arg;
    return ret;
}

Vector Vector::operator-(const Vector &arg) const
{
    Vector ret(*this);
    ret -= arg;
    return ret;
}

Vector Vector::operator*(const Vector &arg) const
{
    Vector ret(*this);
    ret *= arg;
    return ret;
}
double Vector::operator|(const Vector &arg) const
{
    double result = 0;
    if (size() != arg.size())
        throw "bad size";
    for (int i = 0; i < size(); i++)
    {
        result += (*this)[i] * arg[i];
    }
    return result;
}

Vector Vector::zeros(int n)
{
    Vector ret(n);
    for (int i = 0; i < n; i++)
    {
        ret[i] = 0;
    }
    return ret;
}

Vector Vector::random(int n)
{
    Vector ret(n);
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        ret[i] = ((double)rand() / (RAND_MAX));
    }
    return ret;
}

Vector Vector::range(double stop, double start, double step)
{
    double n = (stop - start) / step;
    Vector ret(n);
    if (start > stop)
    {
        return ret;
    }
    else
    {

        for (int i = 0; i < n; i++)
        {
            ret[i] = start + step * i;
        }
        return ret;
    }
}

double Vector::sum() const
{
    double sumResult = 0;
    for (auto &e : *this)
        sumResult += e;
    return sumResult;
}

double Vector::mean() const
{
    double s = (*this).sum();
    double size = (*this).size();
    double result = s / size;
    // cout << s << endl;
    return result;
}

/*
 * Oblicza odchylenie standardowe
 */
double Vector::std() const
{
    double sumStd = 0;

    for (int i = 0; i < (*this).size(); i++)
    {
        sumStd += ((*this)[i] - (*this).mean()) * ((*this)[i] - (*this).mean());
    }
    double result = sqrt(sumStd / (*this).size());
    return result;
}

/*
 * Zwraca wektor, którego elementy są wynikiem wywołania funkcji apply
 * na elementach oryginalnego wektora
 */

Vector Vector::apply(double (*f)(double)) const
{
    Vector ret(*this);
    for (int i = 0; i < ret.size(); i++)
    {
        ret[i] = f(ret[i]);
    }
    return ret;
}

/*
 * Zwraca wektor, którego elementy sa równe elementowi danego wektora
 * podniesionymi do potęgi arg
 */
Vector Vector::operator^(double arg) const
{
    Vector ret(*this);

    for (int i = 0; i < ret.size(); i++)
    {
        ret[i] = pow(ret[i], arg);
    }
    return ret;
}

////////////////////////////////////////////////

ostream &operator<<(ostream &os, const Vector &v)
{
    os << "[";
    for (int i = 0; i < v.size(); i++)
    {
        if (i != 0)
            os << ", ";
        os << v[i];
    }
    os << "]";
    return os;
}

istream &operator>>(istream &is, Vector &v)
{
    v.clear();
    string s;
    getline(is, s); // wczytaj do znaku ]
    // zamień (przecinki i nawiasy) , ] na spacje
    // s.replace(s.begin(), s.end(),',')
    istringstream iss(s);
    for (;;)
    {

        if (!iss)
        {
            break;
        }
        // wczytaj liczby double i dodaj do wektora
    }
    return is;
}
// Testy
static void test_read_write()
{
    Vector r = Vector::range(10);
    ostringstream oss;
    oss << r;
    string repr = oss.str();
    cout << repr << endl;
    istringstream iss(repr);
    Vector v;
    iss >> v;
    v *= 2;
    cout << v;
}

static void test_add()
{
    int n = 10;
    Vector a = Vector::range(10);
    Vector b = Vector::range(10) * 5 + 2;
    Vector c = a + b;
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << c[1] << endl;
}

// iloczyn skalarny dwóch wektorów
void test_dot()
{
    Vector a = Vector::range(9, 0, 3);
    Vector b = Vector::range(8, 2, 2);
    double s = a.operator|(b);
    cout << a << endl;
    cout << b << endl;
    cout << s << endl;
}

// podnoszenie elementów wektora do potęgi
void test_pow()
{
    Vector a = Vector::range(8, 2, 2);
    Vector c = a.operator^(2);
    cout << a << endl;
    cout << c << endl;
}

// wypisuje wektor po zastosowaniu laogarytmu, a następnie exp(log(x))
void test_apply()
{
    Vector a = Vector::range(8, 2, 2);
    // cout <<"s:" << a.std() << endl;
    cout << a << endl;
    Vector b = a.apply(log);
    cout << b << endl;
    Vector c = b.apply(exp);
    cout << c << endl;
}

// wypełnia wektor wartościami losowymi z przedziału 0,1
// dodaje -0.5 i mnoży przez 20
// kazdy element przekształca według przepisu exp(-(x^2)/8)
// wyznacza średnią i odchylenie standardowe

void test_stats()
{
    Vector a = Vector::random(4);
    cout << a << endl;
    a = a.operator+(-0.5);
    cout << a << endl;
    a = a.operator*(20);
    cout << a << endl;
    a = a.operator^(2);
    cout << a << endl;
    a = a.operator*(-1. / 8);
    a = a.apply(exp);
    cout << a << endl;
    double m = a.mean();
    double std = a.std();
    cout << m << endl
         << std << endl;
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    // test_add();
    // test_dot();
    // test_pow();
    // test_apply();
    test_stats();
    return 0;
}
