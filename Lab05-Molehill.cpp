#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;

class Komora
{
public:
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    Komora(int _x1 = 0, int _y1 = 0, int _z1 = 0, int _x2 = 0, int _y2 = 0, int _z2 = 0)
        : x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2)
    {
        normalizuj();
    }

    /*
     * Znormalizuj tak aby x1<=x2, y1>=y2, z1>=z2
     */
    void normalizuj();

    /*
     * Czy nasza jama przecina się z inną komorą?
     * Jeżeli dwie komory graniczą z zewnątrz ścianami
     * (współrzędne są równe) to nie przecinają się
     */

    bool czy_przecina(const Komora &other) const;

    /*
     * zwróć string sformatowany jako ( x1 y1 z1 , x2 y2 z2 )
     */
    string to_string() const;

    /*
     * wypisz w formacie ( x1 y1 z1 , x2 y2 z2 )
     */
    ostream &wypisz(ostream &os) const;

    /*
     * wczytaj w takim samym formacie ( x1 y1 z1 , x2 y2 z2 )
     *
     */
    istream &wczytaj_ze_spacjami(istream &is);

    /*
     * Docelowa wersja, nawiasy i przecinki mogą łączyć się z liczbami
     * wczytaj w formacie (x1 y1 z1,x2 y2 z2)
     *
     */
    istream &wczytaj(istream &is);

    /*
     * wylosuj wartości z podanych zakresów
     * Zadbaj, aby współrzedne były rózne, np. stosujac do-while
     *
     */
    void losuj(int xmin, int ymin, int zmin, int xmax, int ymax, int zmax);

    /*
     * przesuwa współrzedne komory o wektor (dx,dy,dz)
     */
    void przesun(int dx, int dy, int dz);
};

void Komora::normalizuj()
{
    if (x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 < y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if (z1 < z2)
    {
        int temp = z1;
        z1 = z2;
        z2 = temp;
    }
}

bool Komora::czy_przecina(const Komora &other) const
{
    if (other.x1 > x1 and other.x1 < x2 and other.y1 < y1 and other.y1 > y2 and other.z1 < z1 and other.z1 > z2)
    {
        return true;
    }
    if (other.x2 > x1 and other.x2 < x2 and other.y1 < y1 and other.y1 > y2 and other.z1 < z1 and other.z1 > z2)
    {
        return true;
    }
    if (other.x1 > x1 and other.x1 < x2 and other.y1 < y1 and other.y1 > y2 and other.z2 < z1 and other.z2 > z2)
    {
        return true;
    }
    if (other.x2 > x1 and other.x2 < x2 and other.y1 < y1 and other.y1 > y2 and other.z2 < z1 and other.z2 > z2)
    {
        return true;
    }
    if (other.x1 > x1 and other.x1 < x2 and other.y2 < y1 and other.y2 > y2 and other.z1 < z1 and other.z1 > z2)
    {
        return true;
    }
    if (other.x2 > x1 and other.x2 < x2 and other.y2 < y1 and other.y2 > y2 and other.z1 < z1 and other.z1 > z2)
    {
        return true;
    }
    if (other.x1 > x1 and other.x1 < x2 and other.y2 < y1 and other.y2 > y2 and other.z2 < z1 and other.z2 > z2)
    {
        return true;
    }
    if (other.x2 > x1 and other.x2 < x2 and other.y2 < y1 and other.y2 > y2 and other.z2 < z1 and other.z2 > z2)
    {
        return true;
    }
    if (other.x1 > x1 and other.x2 < x2 and other.z2 < z1 and other.z2 > z2)
    {
        return true;
    }
    return false;
}

string Komora::to_string() const
{
    ostringstream os;
    // użyj innej metody do wypisania na strumień ostringstream - zapisu do stringu
    os << x1 << " " << y1 << " " << z1 << " , " << x2 << " " << y2 << " " << z2 << endl;
    return os.str();
}

ostream &Komora::wypisz(ostream &os) const
{
    // wypisz w ustalonym formacie
    os << x1 << " " << y1 << " " << z1 << " , " << x2 << " " << y2 << " " << z2 << endl;
    return os;
}

istream &Komora::wczytaj_ze_spacjami(istream &is)
{
    string s;
    is >> s;
    if (s != "(")
        is.setstate(ios::failbit);
    // wczytaj współrzedne
    is >> s;
    x1 = atoi(s.c_str());
    is >> s;
    y1 = atoi(s.c_str());
    is >> s;
    z1 = atoi(s.c_str());
    // wczytaj przecinek
    is >> s;
    // znowu współrzędne
    is >> s;
    x2 = atoi(s.c_str());
    is >> s;
    y2 = atoi(s.c_str());
    is >> s;
    z2 = atoi(s.c_str());
    // nawias
    is >> s;
    normalizuj();
    return is;
}

// to można zrealizować zgrabniej wyrażeniami regularnymi
istream &Komora::wczytaj(istream &is)
{
    string s;

    getline(is, s, ',');   // wczytaj do przecinka
    int idx = s.find('('); // znajdź nawias
    if (idx == string::npos)
    {
        is.setstate(ios::failbit);
        return is;
    }

    s = s.substr(idx + 1, s.length()); // wydziel fragment po nawiasie
    istringstream iss(s);              // utwórz wejściowy strumień z obiektu string
    iss >> x1;                         // wczytaj współrzędne
    iss >> y1;
    iss >> z1;

    getline(is, s, ')'); // wczytaj do nawiasu zamykajacego
    iss.clear();
    iss.str(s); // przypnij strumień do nowego stringu
    iss >> x2;
    iss >> y2;
    iss >> z2; // następne współzedne
    if (!iss)
    {
        is.setstate(ios::failbit);
        return is;
    }
    normalizuj();
    return is;
}

void Komora::losuj(int xmin, int ymin, int zmin, int xmax, int ymax, int zmax)
{

    x1 = rand() % (xmax - xmin) + xmin;
    y1 = rand() % (ymax - ymin) + ymin;
    z1 = rand() % (zmax - zmin) + zmin;
    x2 = rand() % (xmax - xmin) + xmin;
    y2 = rand() % (ymax - ymin) + ymin;
    z2 = rand() % (zmax - zmin) + zmin;
}
void Komora::przesun(int dx, int dy, int dz)
{
    x1 = x1 + dx;
    y1 = y1 + dy;
    z1 = z1 + dz;
    x2 = x2 + dx;
    y2 = y2 + dy;
    z2 = z2 + dz;
}

void test_komora_wypisz()
{
    Komora k(1, 2, 3, 4, 5, 6);
    k.wypisz(cout);
}

void test_komora_odczyt_spacje()
{
    istringstream is("( 1 2 3 , 4 5 -6 )");
    Komora k;
    k.wczytaj_ze_spacjami(is);
    k.wypisz(cout);
}

void test_komora_odczyt()
{
    istringstream is("(1 2 3,4 5 6)(23 24 25,26 27 28)");
    Komora k;
    k.wczytaj(is);
    k.wypisz(cout);
    k.wczytaj(is);
    k.wypisz(cout);
}

void test_czy_przecina()
{
    istringstream is("(3 7 8 , 7 2 2)(5 3 9 , 8 0 6)");
    Komora k;
    k.wczytaj(is);
    k.wypisz(cout);
    Komora k2;
    k2.wczytaj(is);
    k2.wypisz(cout);
    cout << "Przecina:" << k.czy_przecina(k2) << endl;
}

// sprawdź wynik na stronie https://github.com/pszwed-ai/cpp/blob/main/Intersections3D.ipynb
void test_czy_losowe_przecina()
{
    Komora k(0, 3, 9, 3, 2, 5);
    k.wypisz(cout);
    cout << endl;
    srand(0);
    for (int i = 0; i < 10; i++)
    {
        Komora k2;

        k2.losuj(0, 0, 0, 5, 6, 8);
        k2.wypisz(cout);
        cout << " " << k.czy_przecina(k2) << endl;
    }
}

int main()
{
    Komora k;
    // cout<< k.to_string();
    k.losuj(2, 3, 4, 6, 7, 8);
    k.wypisz(cout);
    cout << "--testy--" << endl;
    test_komora_wypisz();
    test_komora_odczyt_spacje();
    test_komora_odczyt();
    test_czy_przecina();
    test_czy_losowe_przecina();
    return 0;
}