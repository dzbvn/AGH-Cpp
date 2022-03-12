// PP_Lab10.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <io.h>
#include <sstream>
#include <vector>
#include <fstream>
//#include <bits/stdc++.h>

using namespace std;

//*******************
// wersja Visual Studio

string mode_to_string(unsigned int mode)
{
    const char *chrmode = "xwr";
    string result;
    for (int i = 8; i >= 0; i--)
    {
        result += mode & (1 << i) ? chrmode[i % 3] : '-';
    }
    return result;
}

void print_file_info(const char *path)
{
    struct stat fstat;
    stat(path, &fstat);
    cout << "size:" << fstat.st_size << " ";
    char buf[20];
    struct tm newtime;
    localtime_s(&newtime, &fstat.st_mtime);
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", &newtime);
    cout << "Modified at:" << buf << " ";
    cout << "mode: " << mode_to_string(fstat.st_mode) << " " << oct << fstat.st_mode;
}

static void test_dir1()
{
    string dir_name = "c:/";
    string find_dir_name = dir_name + "*";

    struct _finddata_t fileinfo;
    long handle;

    handle = _findfirst(find_dir_name.c_str(), &fileinfo);
    if (handle < 0)
        return;
    printf((fileinfo.attrib & _A_SUBDIR ? "%s <DIR> " : "%s "),
           fileinfo.name);
    auto fpath = dir_name + "/" + fileinfo.name;
    print_file_info(fpath.c_str());
    cout << endl;

    while (_findnext(handle, &fileinfo) == 0)
    {
        printf((fileinfo.attrib & _A_SUBDIR ? "%s <DIR> " : "%s "),
               fileinfo.name);
        auto fpath = dir_name + "/" + fileinfo.name;
        print_file_info(fpath.c_str());
        cout << endl;
    }
    _findclose(handle);
}

class Dirent
{
protected:
    // nazwa
    string name;
    // wskaźnik na jednostkę  nadrzędną (katalog)
    const Dirent *parent;
    // stuktura z atrybutami pliku
    struct stat fstat;

    // funkcja, która ma odczytać atrybuty pliku i zapisać w fstat
    void fill_info();

public:
    Dirent(const char *_name, const Dirent *_parent) : name(_name), parent(_parent)
    {
        fill_info();
    }

    virtual ~Dirent() {}

    string get_name() const { return name; }

    /*
     * Zwraca ścieżkę, która jest konkatenacją ścieżki rodzica, path_separtor i name
     */
    string get_path() const;

    /*
     * manualna implementacja RTTI
     */
    virtual bool is_file() const { return false; }

    virtual bool is_dir() const { return false; }

    /*
     * Formatuje opis uprawnień, kod w przykładach
     */
    string get_mode_string() const;

    /*
     * formatuje opis pliku (nazwa, czy katalog, rozmiar, data modyfikacji uprawnienie dostępu)
     */
    virtual string to_string() const;

    /*
     * może się przydać ale Windows też akceptuje /
     */
    static char path_separator;

    /*
     *
     */
    virtual void list(ostream &os, int indent = 0) const;
};

class File : public Dirent
{
public:
    File(const char *_name, const Dirent *_parent = nullptr)
        : Dirent(_name, _parent) {}

    virtual bool is_file() const { return true; };

    string to_string() const;
};

class Directory : public Dirent
{
public:
    vector<Dirent *> entries;

    Directory(const char *_name, const Dirent *_parent = nullptr)
        : Dirent(_name, _parent) {}

    Directory(const Directory &other) : Dirent(other) { copy(other); }

    /*
     * Uwaga entries zawierają wskaźniki, trzeba te pliki usunąć
     */
    ~Directory();

    virtual bool is_dir() const { return true; };

    string to_string() const;

    /*
     *
     */
    void scan(int maxdepth = INT_MAX);

    /*
     *
     */
    void list(ostream &os, int indent = 0) const;

    void copy(const Directory &other);

    Directory &operator=(const Directory &other);

    Dirent *find(const char *name);
};

char Dirent::path_separator = '/';

string Dirent::get_path() const
{
    ostringstream os;
    if (parent)
    {
        os << parent->get_path();
    }
    os << path_separator;
    os << name;
    return os.str();
}

string Dirent::get_mode_string() const
{
    const char *chrmode = "xwr";
    //... przykład był wcześniej
    unsigned int mode = fstat.st_mode;
    string result;
    for (int i = 8; i >= 0; i--)
    {
        result += mode & (1 << i) ? chrmode[i % 3] : '-';
    }
    return result;
}

string Dirent::to_string() const
{
    return name;
}

void Dirent::fill_info()
{
    auto full_path = get_path();
    stat(full_path.c_str(), &fstat);
}

void Dirent::list(ostream &os, int indent) const
{
    for (int i = 0; i < indent; i++)
    {
        os << "  ";
    } // wołamy przedefiniowaną polimorficzną funkcję klas potomnych...
    os << to_string() << endl;
}

string File::to_string() const
{
    ostringstream os;
    os << name << " [";
    os << fstat.st_size << ", ";
    char buf[20];
    struct tm newtime;
    localtime_s(&newtime, &fstat.st_mtime);
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", &newtime);
    os << buf << ", ";
    os << mode_to_string(fstat.st_mode) << ", " << oct << fstat.st_mode;
    // rozmiar
    // data modyfikajci
    // prawa dostępu i wartość ósemkowa praw dostępu
    os << "]";
    return os.str();
}

Directory::~Directory()
{
    //... czy coś trzeba usunąć?
    for (std::vector<int>::size_type i = 0; i != entries.size(); i++)
    {
        delete entries[i];
    }
    entries.clear();
}

string Directory::to_string() const
{
    ostringstream os;
    os << name << " <DIR> [";
    // czas modyfikacji
    //  prawa dostępu (tekst i ósemkowo)
    char buf[20];
    struct tm newtime;
    localtime_s(&newtime, &fstat.st_mtime);
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", &newtime);
    os << buf << ", ";
    os << mode_to_string(fstat.st_mode) << ", " << oct << fstat.st_mode;
    os << "]";
    return os.str();
}

void Directory::scan(int max_depth)
{
    if (max_depth == 0)
        return;
    //.... opendir();
    struct _finddata_t fileinfo;
    long handle;
    string find_dir_name = get_path() + path_separator + "*";

    handle = _findfirst(find_dir_name.c_str(), &fileinfo);
    if (handle < 0)
        return;
    if (!(strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, ".") == 0))
    {

        if (fileinfo.attrib & _A_NORMAL)
        {
            entries.push_back(new File(fileinfo.name, this));
        }

        if (fileinfo.attrib & _A_SUBDIR)
        {
            entries.push_back(new Directory(fileinfo.name, this));
        }
    }

    while (_findnext(handle, &fileinfo) == 0)
    {
        if (strcmp(fileinfo.name, ".") == 0)
        {
            continue;
        }
        if (strcmp(fileinfo.name, "..") == 0)
        {
            continue;
        }
        if (fileinfo.attrib & _A_NORMAL)
        {
            entries.push_back(new File(fileinfo.name, this));
        }

        if (fileinfo.attrib & _A_SUBDIR)
        {
            entries.push_back(new Directory(fileinfo.name, this));
        }
    }
    _findclose(handle);

    if (max_depth > 1)
    {
        for (auto e : entries)
        {
            // wywołaj scan(max_depth - 1) ale tylko dla obiektów typu directory
            if (e->is_dir())
            {
                dynamic_cast<Directory *>(e)->scan(max_depth - 1);
            }
        }
    }
}

void Directory::list(ostream &os, int indent) const
{
    // wypisz informacje o kataloguDirent::list(os,indent);
    // dodaj wcięcie i wypisz informacje o elementach podrzędnych
    Dirent::list(os, indent);
    for (auto e : entries)
    {

        if (e->is_file())
        {
            // e->File::list(os, indent);
            dynamic_cast<File *>(e)->list(os, indent);
        }
        else if (e->is_dir())
        {
            // e->Directory::list(os, indent);
            dynamic_cast<Directory *>(e)->list(os, indent);
        }
    }
}

void Directory::copy(const Directory &other)
{

    // entries = other.entries;

    for (auto a : entries)
    {
        if (a->is_dir())
        {
            entries.push_back(new Directory(*(Directory *)a));
        }
        if (a->is_file())
        {
            entries.push_back(new File(*(File *)a));
        }
    }
}

Directory &Directory::operator=(const Directory &other)
{

    // TODO: tu wstawić instrukcję return
    copy(other);
    return *this;
}

Dirent *Directory::find(const char *name)
{
    for (auto a : entries)
    {
        if (a->get_name() == name)
        {
            return a;
        }
    }
    return this;
}

static void test_fast()
{

    Directory d("c:/");

    d.scan(1);

    d.list(cout);
}

static void test_long()
{
    Directory d("c:/");
    d.scan(4); // 2,3 na początek
    ofstream of("dir.txt");
    d.list(of);
}
int main()
{
    test_dir1();
    // cout << mode_to_string(0751) << endl;
    test_fast();
    // test_long();
}
