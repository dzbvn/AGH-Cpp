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

    /*
     * Zmień kolejność elementów
     * Seed analogicznie, jak dla random
     * powtórz n/2 razy:
     *      wylosuj indeks i
     *      wylosuj indeks j
     *      zamień miejscami data[i] i data[j]
     * */
    void shuffle(int seed = -1);

    /*
     * Posortuj - sortowanie bąbelkowe
     * */

    void bubble_sort();

    /*
     * Posortuj za pomocą bibliotecznej funkcji qsort
     */
    void qsort();

    /*
     * porównuje elementy
     * zwraca false jeżeli
     *    - długości tablic są różne
     *    - dla któregoś z elementów zachodzi |data[i] - other.data[i]|>tol
     */
    bool equals(const Array &other, double tol = 1e-10);
};
int compare(const void *a, const void *b);

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

void Array::shuffle(int seed)
{
    // wywołaj srand() jak dla random
    if (seed == -1)

    {
        srand(time(0));
        for (int k = 0; k < n / 2; k++)
        {
            int i = (rand() % n);
            int j = (rand() % n);
            swap(data[i], data[j]);
        }
    }
    else
    {
        srand(seed);
        for (int k = 0; k < n / 2; k++)
        {
            int i = (rand() % n);
            int j = (rand() % n);
            swap(data[i], data[j]);
        }
    }
}

void Array::bubble_sort()
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (data[j] > data[j + 1])
                swap(data[j], data[j + 1]);
        }
    }
}

void Array::qsort()
{
    std::qsort(this->data, this->n, sizeof(double), compare);
}

bool Array::equals(const Array &other, double tol)
{
    if (n != other.n)
    {
        return false;
    }
    else
    {
        for (int i; i < n; i++)
        {
            if (abs(data[i] - other.data[i]) > tol)
            {
                return false;
            }
        }
    }
    return true;
}

int compare(const void *a, const void *b)
{
    if (*(double *)a > *(double *)b)
        return 1;
    else if (*(double *)a < *(double *)b)
        return -1;
    else
        return 0;
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

/*
 * Utwórz tablicę 10 obiektów typu array
 * Wypełnij je wartościami losowymi z zakresu [i,i+1], gdzie i = 0,...,10
 * Zapisz do pliku liczbę 10
 * W kolejnych wierszach zapisz tab[i]
 */

void test_save_array_2D(const char *file_name)
{
    ofstream os;
    os.open(file_name);

    Array *tab = new Array[10];
    os << 10 << endl;
    for (int i = 0; i < 10; i++)
    {
        tab[i].random();
        tab[i].add(i);

        tab[i].write_formatted(cout);
        tab[i].write_formatted(os);
    }

    os.close();
    delete[] tab;
}

/*
 * Przeczytaj z pliku liczbę tablic
 * Utwórz dynamicznie tablicę obiektów typu Array
 * Przeczytaj z pliku zawartość kolejnych tablic
 */

void test_load_array_2D(const char *file_name)
{
    ifstream is(file_name);
    int ilosc;
    is >> ilosc;
    cout << ilosc << endl;
    Array *tab = new Array[ilosc];
    for (int i = 0; i < ilosc; i++)
    {
        tab[i].read_formatted(is);
        tab[i].write_formatted(cout);
    }
    is.close();
}

/*
 * Otwórz plik
 * Sprawdz, czy się udało
 * wczytaj kolejne znaki i dodawaj do obiektu klasy string
 * w petli nieskończonej wykonaj
 *     int c = is.get();
 *     if(c<0)break;
 *     dodaj c do stringu
 * zwróć string
 */
string load_file(const char *name)
{
    ifstream is(name);
    if (!is)
    {
        cerr << "Bład otwarcia pliku:" << name << endl;
        return "";
    }
    string r;
    for (;;)
    {
        int c = is.get();
        if (c < 0)
            break;
        r += c;
    }
    return r;
}

/*
 * wykonaj test_save_array_2D, test_load_array_2D i test_save_array_2D - zapisując do dwóch różnych plików
 * porównaj zawartość plików
 */

void test_load_save()
{
    string s1 = load_file("plik1.txt");
    string s2 = load_file("plik2.txt");
    cout << (s1 == s2);
}
/*
 * Zmień kolejność elementów za pomocą shuffle
 * Wypisz
 * Posortuj metodą bubble sort
 * Wypisz
 */
void test_shuffle_bubble_sort()
{
    Array B;
    B.linspace(1, 20);
    B.shuffle();
    B.write_formatted(cout);
    B.bubble_sort();
    B.write_formatted(cout);
}

/*
 * Zmień kolejność elementów za pomocą shuffle
 * Wypisz
 * Posortuj metodą qsort
 * Wypisz
 */

void test_shuffle_qsort()
{
    Array Q;
    Q.linspace(1, 20);
    Q.shuffle();
    Q.write_formatted(cout);
    Q.qsort();
    Q.write_formatted(cout);
}

int main()
{

    // test_random();
    // test_linspace();
    // test_mul_add();
    // test_file_write("plik1");
    // test_file_read("plik1");

    // test_shuffle_bubble_sort();
    // test_shuffle_qsort();
    // test_save_array_2D("plik3.txt");
    test_load_array_2D("plik3.txt");
    // cout << load_file("plik2.txt")<<endl;
    return 0;
}
