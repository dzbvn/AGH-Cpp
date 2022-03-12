#include <iostream>
#include <string>
using namespace std;

//-----------------Zadanie 1
/*int main(){
    int x=7,y=8;
    std::cout<<x<<std::endl;
    cout<<x<<"+"<<y<<"="<<x+y <<endl;
    cin>>x>>y;
    string imie;
    string nazwisko;
    cin>>imie>>nazwisko;
    cout<<imie<<" "<<nazwisko;
    return 0;
}*/

//----------------Zadanie 2
#define N 10
void fibo1()
{
    int tab[N];
    if (N == 1)
    {
        tab[0] = 1;
        cout << tab[0];
    }

    else if (N > 1)
    {
        tab[0] = 1;
        tab[1] = 1;
        for (int i = 2; i < N; i++)
        {
            tab[i] = tab[i - 1] + tab[i - 2];
        }
        for (int i = 0; i < N; i++)
        {
            cout << tab[i];
            cout << endl;
        }
    }
    else
    {
        cout << "Niepoprawny rozmiar tablicy";
    }
}

/*int main(){
fibo1();
}*/

//----------------Zadanie 3-------
void fibo2(int n)
{
    if (n <= 0)
    {
        cout << "Niepoprawny rozmiar tablicy";
    }
    else
    {
        int *tab = new int[n];
        if (n == 1)
        {
            tab[0] = 1;
            cout << tab[0];
        }
        else if (n > 1)
        {
            tab[0] = 1;
            tab[1] = 1;
            for (int i = 2; i < n; i++)
            {
                tab[i] = tab[i - 1] + tab[i - 2];
            }
            for (int i = 0; i < n; i++)
            {
                cout << tab[i];
                cout << endl;
            }
        }
        delete[] tab;
    }
}
/*int main(){
    fibo2(5);
}*/
//-----------------Zadanie 4

class Fibo1
{
    int *tab;
    int n;

public:
    void init(int _n);
    void destroy();
    void fill();
    void print();
};
void Fibo1::init(int _n)
{
    if (_n <= 0)
    {
        n = 0;
        tab = 0;
        return;
    }
    n = _n;
    tab = new int[n];
}

void Fibo1::destroy()
{
    if (tab)
    {
        delete[] tab;
    }
}

void Fibo1::fill()
{
    if (tab)
    {
        if (n == 1)
        {
            tab[0] = 1;
        }
        else if (n > 1)
        {
            tab[0] = 1;
            tab[1] = 1;
            for (int i = 2; i < n; i++)
            {
                tab[i] = tab[i - 1] + tab[i - 2];
            }
        }
    }
}

void Fibo1::print()
{
    if (tab)
    {
        for (int i = 0; i < n; i++)
        {
            cout << tab[i];
            cout << endl;
        }
    }
}
void test_fibo1()
{
    Fibo1 f;
    f.init(1);
    f.fill();
    f.print();
    f.destroy();
}
/*
int main(){
    test_fibo1();
    return 0;
}*/

//---------------------Zadanie 5
class Fibo
{
    int *tab;
    int n;

public:
    Fibo(int _n);
    ~Fibo();
    void fill();
    void print();
};

Fibo::Fibo(int _n)
{

    if (_n <= 0)
    {
        n = 0;
        tab = 0;
        return;
    }
    n = _n;
    tab = new int[n];
}
Fibo::~Fibo()
{
    if (tab)
    {
        delete[] tab;
    }
}
void Fibo::fill()
{
    if (tab)
    {
        if (n == 1)
        {
            tab[0] = 1;
        }
        else if (n > 1)
        {
            tab[0] = 1;
            tab[1] = 1;
            for (int i = 2; i < n; i++)
            {
                tab[i] = tab[i - 1] + tab[i - 2];
            }
        }
    }
}

void Fibo::print()
{
    if (tab)
    {
        for (int i = 0; i < n; i++)
        {
            cout << tab[i];
            cout << endl;
        }
    }
}

void test_fibo()
{
    Fibo ff(3);
    ff.fill();
    ff.print();
}
/*int main(){
    test_fibo();
    return 0;
}*/

//-----------------Zadanie 6
class GeometricSequence
{
    int *tab;
    int n;
    double first;
    int q;

public:
    GeometricSequence(int _n, int _q, double _first);
    ~GeometricSequence();
    void print();
    void fill();
};
GeometricSequence::GeometricSequence(int _n, int _q, double _first)
{
    if (n <= 0)
        tab = 0;
    n = _n;
    q = _q;
    first = _first;
    tab = new int[n];
}

GeometricSequence::~GeometricSequence()
{
    if (tab)
    {
        delete[] tab;
    }
}
void GeometricSequence::fill()
{
    if (tab)
    {
        if (n == 1)
        {
            tab[0] = first;
        }
        else if (n > 1)
        {
            tab[0] = first;
            for (int i = 1; i < n; i++)
            {
                tab[i] = tab[i - 1] * q;
            }
        }
    }
}
void GeometricSequence::print()
{
    if (tab)
    {
        for (int i = 0; i < n; i++)
        {
            cout << tab[i];
            cout << endl;
        }
    }
}

/*int main(){
    GeometricSequence gg(3,2,1);
    gg.fill();
    gg.print();
}*/
