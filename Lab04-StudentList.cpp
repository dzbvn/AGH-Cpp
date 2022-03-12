#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;

class Student
{
public:
    int indeks;
    string imie;
    string nazwisko;
    bool skreslony = false;
    string grupa;
    /*
     * Czytaj dane z istream. Załóż, że pola są oddzielone znakiem tabulacji
     * */
    void read(istream &is);
    /*
     * Zapisz dane do istream. Oddzielaj pola znakiem tabulacji. Nie dodawaj endline
     * */
    void write(ostream &os) const;
};

void Student::read(istream &is)
{
    is >> indeks;
    is >> imie;
    is >> nazwisko;
    is >> skreslony;
    is >> grupa;
}

void Student::write(ostream &os) const
{
    os << indeks << "\t" << imie << "\t" << nazwisko << "\t" << grupa << "\t";
}

void test_read_write()
{
    ifstream is("studenci_isi-2021.csv");
    if (!is)
    {
        cerr << "Błąd otwarcia pliku" << endl;
        return;
    }
    string header;
    // czytaj nagłówek za pomocą getline()
    getline(is, header);
    cout << header << endl;
    while (is)
    {
        Student st;
        st.read(is);
        st.write(cout);
        cout << endl;
        // wczytaj
        // zapisz na cout
    }
}

class StudentList
{
    Student *tablica = 0;
    int count = 0;
    int capacity = 0;

public:
    StudentList(int cap = 100) : capacity(0), tablica(0)
    {
        if (cap > 0)
        {
            capacity = cap;
            tablica = new Student[cap];
        }
    }
    ~StudentList()
    {
        if (tablica)
            delete[] tablica;
    }

    int getCount() const
    {
        return count;
    }
    /*
     * Dodaj studenta do tablicy. Zwróć wartość false, jeżeli zabrakło miejsca
     */
    bool add(const Student &st);
    /*
     * Czytaj ze strumienia i dodawaj.
     * Strumień nie ma nagłówka
     * Przerwij, gdy nie uda się odczyt lub zabraknie miejsca
     */
    void read(istream &is);
    /*
     *Zapisuj do strumienia (bez nagłówka)
     */
    void write(ostream &os) const;
    /*
     * skreśl studenta o dnaym numerze indeksu - ustaw pole skreślony
     */
    bool skresl(int index)
    {
        for (int i = 0; i < count; i++)
        {
            if (tablica[i].indeks = index)
            {
                tablica[i].skreslony = true;
                return true;
            }
        }
        return false;
    }
    /*
     * reaktywuj studenta o dnaym numerze indeksu - ustaw pole skreślony
     */
    bool reaktywuj(int index)
    {
        for (int i = 0; i < count; i++)
        {
            if (tablica[i].indeks = index)
            {
                tablica[i].skreslony = false;
                return true;
            }
        }
        return false;
    }
    /*
     * usuń studenta o danym indeksie z tablicy
     * fizycznie usuń obiekt
     */

    bool usun(int index);

    /*
     * Utwórz listęi umieść w niej wszystkich studentów
     * których pole grupa jest równe group
     */
    StudentList selectGroup(const char *group) const;

    /*
     * Utwórz listę i umieść w niej wszystkich studentów
     * którzy spełniają predykat.
     * Predykat jest wskaźnikiem do funkcji boolowskiej, której parametrem jest student
     */

    StudentList select(bool (*pred)(const Student &s)) const;
};

bool StudentList::add(const Student &st)
{
    if (count == capacity)
        return false;

    tablica[count] = st;
    count++;
    return true;
}

void StudentList::write(ostream &os) const
{
    for (int i = 0; i < count; i++)
    {
        tablica[i].write(os);
        os << endl;
    }
}

void StudentList::read(istream &is)
{
    while (is)
    {
        Student st;
        st.read(is);
        if (!is)
        {
            break;
        }
        if (!(add(st)))
        {
            break;
        }
        if (count == capacity)
        {
            break;
        }
    }
}

bool StudentList::usun(int index)
{
    //???
    for (int i = 0; i < count; i++)
    {
        if (tablica[i].indeks = index)
        {
            for (int j = i; j < count; j++)
            {
                if (j != capacity - 1)
                {
                    tablica[j] = tablica[j + 1];
                    count--;
                    return true;
                }
            }
        }
    }
    return false;
}

StudentList StudentList::selectGroup(const char *group) const
{
    StudentList ret;
    for (int i = 0; i < count; i++)
    {
        if (tablica[i].grupa == group)
        {
            ret.add(tablica[i]);
        }
    }
    return ret;
}

StudentList StudentList::select(bool (*pred)(const Student &s)) const
{
    StudentList ret;
    for (int i = 0; i < count; i++)
    {
        if (pred(tablica[i]))
        {
            ret.add(tablica[i]);
        }
    }
    return ret;
}

void test_list_read()
{
    ifstream is("studenci_isi-2021.csv");
    if (!is)
    {
        cerr << "Błąd otwarcia pliku" << endl;
        return;
    }
    string header;
    getline(is, header);
    // Utwórz obiekt StudentList, wczytaj resztę pliku do listy, wypisz na cout
    StudentList lista1;
    lista1.read(is);
    lista1.write(cout);
}
/*
 * Wczytaj plik do obiektu StudentList
 * Wywołaj funkcję select z parametrem "CWL3"
 * Wypisz zwróconą listę na cout
 */
void test_list_read_select()
{
    ifstream is("studenci_isi-2021.csv");
    if (!is)
    {
        cerr << "Błąd otwarcia pliku" << endl;
        return;
    }
    string header;
    getline(is, header);
    StudentList lista1;
    lista1.read(is);
    lista1.selectGroup("CWL3").write(cout);
}

/*
 * Przykład wywołania z predykatem
 */

bool student_na_b(const Student &st)
{
    return st.nazwisko[0] == 'B';
}
bool wszystkie_Panie(const Student &st)
{
    return st.imie[st.imie.length() - 1] == 'a';
}

void test_list_read_select_pred()
{
    // wczytaj całą listę do slist
    StudentList slist;
    ifstream is("studenci_isi-2021.csv");
    if (!is)
    {
        cerr << "Błąd otwarcia pliku" << endl;
        return;
    }
    string header;
    getline(is, header);
    slist.read(is);
    StudentList slist2 = slist.select([](auto &st)
                                      { return st.nazwisko[0] == 'B'; });
    // StudentList slist2 = slist.select(student_na_b);
    // StudentList slist2 = slist.select(wszystkie_Panie);
    // slist2.usun(409450);
    slist2.write(cout);
}

int main()
{
    // test_read_write();
    // test_list_read();
    test_list_read_select();
    // test_list_read_select_pred();
    return 0;
}
