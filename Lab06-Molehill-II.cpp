#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

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

class Kretowisko
{
public:
    vector<Komora> komory;

    /*
     * dodaje komorę, ale tylko w przypadku,
     * kiedy nie przecina się z posotałymi
     */
    bool dodaj(const Komora &k);
    /*
     * Buduje kretowisko losując n komór o współrzednych x, y z zakresu [min,max]
     * Wygenerowane komory należy przesunąć o dx=rand()%scatter, dy=rand()%scatter i dz=0
     * Ponieważ komory są pod ziemią - dla współrzednej z losuj z zakresu -abs(depth) do -1
     */
    void buduj(int n, int min, int max, int depth, int scatter);

    /*
     * Zapisz w formacie ( komora1,komora2,komora3,....)
     */
    ostream &wypisz(ostream &os) const;

    /*
     * Wczytaj w takim formacie, jak zapisałeś
     */

    istream &wczytaj(istream &is);

    /*
     * Zwróć całkowitą powierzchnię komór
     */
    int powierzchnia() const;
    /*
     * Zwróć  powierzchnię komór na głębokości depth
     */
    int powierzchnia(int depth) const;

    /*
     * Zwróć całkowitą objętość komór
     */
    int objetosc() const;

    /*
     * Zwróć objętość komór od -inf do głębokości depth
     * Jest to patrząc formalnie całka powierzchnia(d) delta_d
     * Ale powierzchnia(d) to funkcja odcnikami stała...
     */
    int objetosc(int depth) const;

    /*
     * Wszystkie komory są połączone ze sobą. Złośliwy ogrodnik wlał do kretowiska wodę,
     * która rozpłynęła się pomiędzy komoram.
     * Załóż że objętość wlanej wody nie przewyższa sumarycznej objętości komór
     * Oblicz do jakiego poziomu zostaną zalane komory?
     */
    double poziom_wody(double v) const;
};

bool Kretowisko::dodaj(const Komora &k)
{
    for (int i = 0; i != komory.size(); i++)
    {
        if (komory[i].czy_przecina(k))
        {
            return false;
        }
    }
    komory.push_back(k);
    return true;
}

void Kretowisko::buduj(int n, int min, int max, int depth, int scatter)
{
    int i = 0;
    if (depth > 0)
        depth *= -1;
    while (i < n)
    {
        Komora k;

        k.losuj(min, min, -abs(depth), max, max, -1);
        k.przesun(rand() % scatter, rand() % scatter, 0);

        if (dodaj(k))
            i++;
    }
}

ostream &Kretowisko::wypisz(ostream &os) const
{
    os << '(';
    for (int i = 0; i != komory.size(); i++)
    {
        os << komory[i].to_string();
    }

    os << ')';
    cout << endl;
    return os;
}

istream &Kretowisko::wczytaj(istream &is)
{
    komory.clear(); // nie chcemy poprzednio zdefiniowanych komór
    int c;
    do
    {
        c = is.get();
    } while (c != '(' || !is); // znajdź '(' ale przerwij, gdy !is

    for (;;)
    {
        c = is.get();
        // jeżeli koniec pliku lub napotkano ')' to przerwij
        if (c == ')' || !is)
        {
            break;
        }
        if (c == '(')
        {
            // jeżeli napotkano '(', to
            // cofnij znak do strumienia za pomocą unget()
            // wczytaj komorę i dodaj do kretowiska
            is.unget();
            Komora k;
            k.wczytaj(is);
            dodaj(k);
        }
    }
    return is;
}

int Kretowisko::powierzchnia() const
{
    int powierzchnia = 0;
    for (int i = 0; i != komory.size(); i++)
    {

        int y = abs(komory[i].y1 - komory[i].y2);
        int x = abs(komory[i].x2 - komory[i].x1);
        powierzchnia += y * x;
    }
    return powierzchnia;
}

int Kretowisko::powierzchnia(int depth) const
{
    int powierzchnia = 0;
    for (int i = 0; i != komory.size(); i++)
    {
        if (komory[i].z1 >= depth && depth >= komory[i].z2)
        {
            int y = abs(komory[i].y1 - komory[i].y2);
            int x = abs(komory[i].x2 - komory[i].x1);
            powierzchnia += y * x;
        }
    }
    return powierzchnia;
}

int Kretowisko::objetosc() const
{
    int objetosc = 0;
    for (int i = 0; i != komory.size(); i++)
    {
        int y = abs(komory[i].y1 - komory[i].y2);
        int x = abs(komory[i].x2 - komory[i].x1);
        int z = abs(komory[i].z1 - komory[i].z2);
        objetosc += y * x * z;
    }
    return objetosc;
}

int Kretowisko::objetosc(int depth) const
{
    int objetosc = 0;

    for (int i = 0; i != komory.size(); i++)
    {
        int y = abs(komory[i].y1 - komory[i].y2);
        int x = abs(komory[i].x2 - komory[i].x1);

        if (depth >= komory[i].z1)
        {
            int z = abs(komory[i].z1 - komory[i].z2);
            objetosc += x * y * z;
        }
        else if (depth <= komory[i].z2)
        {
            continue;
        }
        else
        {
            int z = abs(depth - komory[i].z2);
            objetosc += y * x * z;
        }
    }

    return objetosc;
}

double Kretowisko::poziom_wody(double v) const
{
    int minZ = 1;
    for (int i = 0; i != komory.size(); i++)
    {
        if (minZ > komory[i].z2)
        {
            minZ = komory[i].z2;
        }
    }
    int i;
    for (i = minZ; i < 0; i++)
    {
        int tempObj = objetosc(i);
        if (tempObj > v)
        {
            // cout<< "poziom_wody" << endl << tempObj << endl << v << endl << i << endl;
            break;
        }
    }

    return (double)(i - 1) + (v - (double)objetosc(i - 1)) / (double)powierzchnia(i);
}

/*
 * Wywołaj metodę buduj i wypisz
 */
void test_kretowisko_buduj()
{
    Kretowisko k;
    k.buduj(3, 0, 10, -8, 2);
    k.wypisz(cout);
}

/*
 * Wczytaj ze stringu i wypisz
 */
void test_kretowisko_wczytaj()
{
    string s = "(( 8 15 -5 , 11 14 -7 ) ( 14 13 -4 , 17 11 -8 ) ( 13 7 -3 , 16 4 -4 ) "
               "( 1 6 -7 , 4 4 -10 ) ( 16 20 -4 , 18 19 -10 ) ( 5 16 -7 , 8 14 -8 ) "
               "( 13 11 -3 , 17 10 -5 ) ( 11 20 -2 , 13 18 -7 ) ( 17 14 -5 , 18 12 -8 ) "
               "( 17 18 -4 , 20 16 -9 ) )";
    istringstream is(s);
    Kretowisko k;
    k.wczytaj(is);
    k.wypisz(cout);
}

/*
 * Zbuduj
 * Zapisz do stringu s
 * Wczytaj ze stringu s
 * Wypisz na cout
 * Zapisz do stringu s2
 * Porównaj stringi
 */

void test_kretowisko_buduj_zapisz_wczytaj_wypisz()
{
    srand(0);
    Kretowisko k;
    k.buduj(3, 0, 10, -8, 2);

    ostringstream os;
    k.wypisz(os);
    string s = os.str();

    istringstream is(s);
    k.wczytaj(is);

    k.wypisz(cout);

    ostringstream os2;
    k.wypisz(os2);
    string s2 = os.str();

    if (s == s2)
    {
        cout << "takie same" << endl;
    }
}

/*
 * Utwórz kretowisko z jedną komorą
 * Przeiteruj od z2 do z1 w pętli i wypisuj powierzchnię i sumaryczną objetość poniżej z
 */

void test_jedna_komora()
{

    Kretowisko k1;
    // k1.buduj(1, 0, 10, -8, 2);
    string s = "(( 8 15 -5 , 11 14 -7))";
    istringstream is(s);
    k1.wczytaj(is);
    k1.wypisz(cout);

    for (int i = k1.komory[0].z2; i <= k1.komory[0].z1; i++)
    {
        cout << "i: " << i << endl;
        cout << "pow: " << k1.powierzchnia(i) << endl;
        cout << "obj: " << k1.objetosc(i) << endl;
        cout << "calkowita obj: " << k1.objetosc() << endl;
    }
}

/*
 * Analogicznie przetestuj dwie komory
 */

void test_dwie_komory()
{
    string s = "(( 8 15 -5 , 11 14 -7 ) ( 14 13 -4 , 17 11 -8 ))";
    Kretowisko k2;
    istringstream is(s);
    k2.wczytaj(is);
    k2.wypisz(cout);
    int maxZ = -99999;
    int minZ = 1;
    for (int i = 0; i != k2.komory.size(); i++)
    {
        if (minZ > k2.komory[i].z2)
        {
            minZ = k2.komory[i].z2;
        }
        if (maxZ < k2.komory[i].z1)
        {
            maxZ = k2.komory[i].z1;
        }
    }
    // cout << maxZ << minZ << endl;

    for (int i = minZ; i <= maxZ; i++)
    {

        cout << "i: " << i << endl;
        cout << "pow: " << k2.powierzchnia(i) << endl;
        cout << "obj: " << k2.objetosc(i) << endl;
        cout << "calkowita obj: " << k2.objetosc() << endl;
    }
}

/*
 * Znajdź z_min i z_max
 * W pętli z od z_min do z_max dla 100 punktów
 * Wypisz powierzchnię(z), przybliżoną objętość (całkowanie metodą trapezów)
 */

void test_objetosc_odd()
{
    Kretowisko k3;
    k3.buduj(4, 0, 10, -10, 2);

    int maxZ = -99999;
    int minZ = 1;
    for (int i = 0; i != k3.komory.size(); i++)
    {
        if (minZ > k3.komory[i].z2)
        {
            minZ = k3.komory[i].z2;
        }
        if (maxZ < k3.komory[i].z1)
        {
            maxZ = k3.komory[i].z1;
        }
    }
    // cout << minZ << maxZ << endl;

    for (double i = minZ; i < maxZ; i += (double)((abs(minZ - maxZ)) / 100.0))
    {
        cout << i << endl;
        cout << "Powierzchnia: " << k3.powierzchnia((int)i) << endl;
        cout << "Objetosc: " << k3.objetosc((int)i) << endl;
    }
}

/*
 * Oblicz całkowitą objętość komór
 * W pętli od 0 do V oblicz głębokość dla v
 * Wypisz v, d, i powierzchnię(d)
 */
void test_poziom_wody_odv()
{
    Kretowisko k4;
    k4.buduj(4, 0, 10, -10, 2);
    int V = k4.objetosc();
    for (int v = 0; v < V; v++)
    {
        int d = (int)k4.poziom_wody((double)v);
        cout << "v: " << v << endl
             << "d: " << d << endl
             << "powierzchnia: " << k4.powierzchnia(d) << endl;
    }
}

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
    if (x1 < other.x2 && x2 > other.x1 && y1 > other.y2 && y2 < other.y1 && z1 > other.z2 && z2 < other.z1)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    while (x1 == x2)
    {
        x2 = rand() % (xmax - xmin) + xmin;
    }
    while (y1 == y2)
    {
        y2 = rand() % (ymax - ymin) + ymin;
    }
    while (z1 == z2)
    {
        z2 = rand() % (zmax - zmin) + zmin;
    }
    normalizuj();
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

        k2.losuj(0, 0, 0, 8, 8, 8);
        k2.normalizuj();
        k2.wypisz(cout);
        cout << " " << k.czy_przecina(k2) << endl;
    }
}

int main()
{
    /*Komora k;
    cout<< k.to_string();
    k.losuj(2, 3, 4, 6, 7, 8);
    k.wypisz(cout);
    cout << "--testy--" << endl;
    test_komora_wypisz();
    test_komora_odczyt_spacje();
    test_komora_odczyt();
    test_czy_przecina();
    test_czy_losowe_przecina();*/
    // test_kretowisko_buduj();
    // test_kretowisko_wczytaj();
    // test_kretowisko_buduj_zapisz_wczytaj_wypisz();
    // test_jedna_komora();
    // test_dwie_komory();
    // test_objetosc_odd();
    test_poziom_wody_odv();
    return 0;
}