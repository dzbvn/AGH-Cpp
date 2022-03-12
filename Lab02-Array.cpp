#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
using namespace std;

class Array
{
    double *data;
    int n;

public:
    Array(int size = 20) : data(0)
    {
        n = (size >= 0) ? size : 0;
        if (n > 0)
            data = new double[n];
    }
    ~Array()
    {
        if (data)
            delete data;
    }

    /*
     * wypełnij tablice równomiernie rozłożonymi elementami od start to end
     * Funkcja zwraca false, jeżeli end<start
     * */
    bool linspace(double start = 0, double end = 1);

    /*
     * wypełnij tablice losowymi elementami z zakresu 0-1
     *
     * Wywołanie srand()
     * jeżeli seed == -1 zainicjuj generator liczb pseudolosowych time(0)
     * w przeciwnym wypadku zainicjuj wartością seed
     * */
    void random(int seed = -1);

    /*
     * zapis do strumienia wyjściowego w formacie:
     * n, ciąg n liczb typu double
     */

    void write_formatted(ostream &os) const;

    /*
     * Zwolnij pamięć
     * Przeczytaj n
     * Przydziel pamięć
     * Przeczytaj liczby
     * */
    void read_formatted(istream &is);

    /*
     * Dodaj do każdego elementu tablicy wartość v
     */
    void add(double v);

    /*
     * Pomnóż każdy element tablicy przez wartość v
     */
    void mul(double v);
};

bool Array::linspace(double start, double end)
{
    if (start > end)
    {
        return false;
    }
    else
    {
        double p = (end - abs(start)) / (n - 1);
        for (int i = 0; i < n; i++)
        {
            data[i] = p * i;
        }
        return true;
    }
}

void Array::random(int seed)
{
    if (seed == -1)

    {
        srand(time(0));
        for (int i = 0; i < n; i++)
        {
            double rVal = ((double)rand() / (RAND_MAX));
            data[i] = rVal;
        }
    }
    else
    {
        srand(seed);
        for (int i = 0; i < n; i++)
        {
            double rVal = ((double)rand() / (RAND_MAX));
            data[i] = rVal;
        }
    }
}

void Array::write_formatted(ostream &os) const
{
    os << n << "\n";
    for (int i = 0; i < n; i++)
    {
        os << data[i] << "\n";
    }
}

void Array::read_formatted(istream &is)
{
    if (this->data)
        delete[] this->data;
    is >> this->n;
    this->data = new double[this->n];
    for (int i = 0; i < this->n; i++)
        is >> this->data[i];
}

void Array::add(double v)
{
    for (int i = 0; i < n; i++)
    {
        data[i] += v;
    }
}

void Array::mul(double v)
{
    for (int i = 0; i < n; i++)
    {
        data[i] *= v;
    }
}

/*
 * Przetestuj działanie dla różnych start i end kombinacji start i end
 * Wypisuj zawartość tablicy
 */

void test_linspace()
{
    Array arrL1(6);
    arrL1.linspace(0, 10);
    arrL1.write_formatted(cout);

    Array arrL2(11);
    arrL2.linspace(0, 20);
    arrL2.write_formatted(cout);

    Array arrL3(3);
    arrL3.linspace(0, 10);
    arrL3.write_formatted(cout);
}

/*
 * Przetestuj działanie
 * Czy tablica zawiera liczby z właściwego przedziału?
 */

void test_random()
{
    Array arrR1(6);
    arrR1.random();
    arrR1.write_formatted(cout);

    Array arrR2(10);
    arrR2.random();
    arrR2.write_formatted(cout);

    Array arrR3(12);
    arrR3.random();
    arrR3.write_formatted(cout);
}

/*
 * Przetestuj mul i add, tak aby wygenerować
 * - tablicę losowych wartości z zakresu [4,5]
 * - tablicę losowych wartości z zakresu [-10,10]
 * - tablicę kolejnych, równomiernie rozłozonych wartości od 0 do 100
 * - tablicę wartości od 0 do -10
 */

void test_mul_add()
{

    Array arrMA(5);
    arrMA.random();
    arrMA.add(4);
    arrMA.write_formatted(cout);

    Array arrMA2(6);
    arrMA2.random();
    arrMA2.mul(20);
    arrMA2.add(-10);
    arrMA2.write_formatted(cout);

    Array arrMA3(21);
    arrMA3.linspace(0, 100);
    arrMA3.write_formatted(cout);

    Array arrMA4(5);
    arrMA4.linspace(-10, 0);
    arrMA4.write_formatted(cout);
}

/*
 * Przetestuj zapis do pliku
 */
void test_file_write(const char *nazwa_pliku)
{
    Array array(100);
    // wypełnij wartościami
    array.random();
    ofstream os(nazwa_pliku);
    if (!os)
    {
        cerr << "Błąd otwarcia pliku: " << nazwa_pliku << endl;
        return;
    }
    // ... zapisz array do strumienia os
    array.write_formatted(os);
}

/*
 * Przetestuj odczyt z pliku
 * Przeczytaj i wypisz
 */

void test_file_read(const char *nazwa_pliku)
{
    Array array(0);
    ifstream is(nazwa_pliku);
    array.read_formatted(is);
    array.write_formatted(cout);
}

int main()
{

    // test_random();
    // test_linspace();
    test_mul_add();
    // test_file_write("plik1");
    // test_file_read("plik1");
    return 0;
}
