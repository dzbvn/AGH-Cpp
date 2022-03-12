#include <vector>
#include <iostream>
#include <cstdlib>
#include <set>
using namespace std;

class Graph
{
    friend class DFSIterator;
    friend class BFSIterator;

    /*
     * Odpowiednik dwuwymiarowej tablicy
     */
    vector<vector<int>> T;
    // flaga - czy graf jest skierowany
    bool is_directed = true;

public:
    /*
     * losuje graf zawierający n wierzchołków.
     * Obecność krawędzi oznaczona wartością 1 w tablicy T
     * density - stosunek liczby krawędzi / N,  gdzie
     *     N=n*n jeżeli allow_selfloop=true
     *     N=n*(n-1) jeżeli allow_selfloop=false
     *     is_directed - czy jest skierowany. Dla !is_directed macierz jest symetryczne
     *     allow_selfloop - czy dopuszczalne są krawędzie (i,i) - niezerowe elementy na przekątnej
     */
    void random(int n, double density, bool is_directed = true, bool allow_selfloop = false);

    /*
     * tworzy pusty graf o liczbie wierzchołków n. justawia flagę is_directed
     */
    void create_empty(int n, bool is_directed = true);

    /*
     * zwraca reprezentację w formacie dot (graphviz): https://dreampuf.github.io/GraphvizOnline
     */
    string to_dot() const;

    /*
     * zwraca zapisaną tekstowo tablicę
     */
    string to_mat() const;

    /*
     * Zwraca zbiór wierzchołków bezpośrednio osiągalnych z danego wierzchołka
     * Zakładamy nawigację wiersz->kolumna, czyli dla i-tego wierzchołka osiąglne będą
     * N = {j:T[i][j]!=0}
     */
    set<int> get_linked_nodes(int i) const;
};

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>

void Graph::create_empty(int n, bool is_directed)
{
    if (n <= 0)
        throw invalid_argument("bad n");
    T.clear();
    // użyj resize
    T.resize(n);
    // cout << T[0].size() << endl;
    for (int i = 0; i < n; i++)
    {
        T[i].resize(n);
    }
    // cout << T[0].size() << endl;
    this->is_directed = is_directed;
}

void Graph::random(int n, double density, bool is_directed, bool allow_selfloop)
{
    create_empty(n, is_directed);
    if (density <= 0 || density > 1)
        throw invalid_argument("bad density");
    /*
     * Określ maksymalną liczbę krawedzi do wylosowania
     * w zależności od allow_selfloop (n*n lub n*(n-1)
     * pomnóż przez density
     * jeżeli skeirowany podziel przez 2 bo macierz jest symetryczna
     * następnie losuj parę indeksów i wypełniaj macierz - stosownie do flag
     */

    int N;
    if (allow_selfloop)
    {
        N = n * n;
        // << N << endl;
    }
    else
    {
        N = n * (n - 1);
    }

    int nEdges = density * N;
    cout << "n: " << n << endl
         << "max n of edges: " << nEdges << endl;
    if (!is_directed)
    {
        nEdges = nEdges / 2;
        for (int i = 0; i < nEdges; i++)
        {
            int v = (rand() % n);
            int u = (rand() % n);
            if (!allow_selfloop && v == u)
                continue;
            T[v][u] = 1;
            T[u][v] = 1;
        }
    }
    else
    {
        for (int i = 0; i < nEdges; i++)
        {
            int v = (rand() % n);
            int u = (rand() % n);
            if (!allow_selfloop && v == u)
                continue;
            T[v][u] = 1;
        }
    }
}

set<int> Graph::get_linked_nodes(int i) const
{
    set<int> r;
    //????
    for (int k = 0; i < T[i].size(); k++)
    {
        if (T[i][k] == 1)
        {
            r.insert(i);
        }
    }
    return r;
}

/*
 * Wizualizacja: https://dreampuf.github.io/GraphvizOnline/
 * Przykładowe formaty

digraph G {
   0;
   1;
   2;
   3;
   4;
   5;
   6;
   7;
   8;
   9;
   0->3;
   0->4;
   1->2;
   2->9;
   3->5;
   3->6;
   3->8;
   4->0;
   4->7;
   5->0;
   5->2;
   5->7;
   6->0;
   6->2;
   7->2;
   7->6;
   8->6;
   9->8;
}

graph G {
   0;
   1;
   2;
   3;
   4;
   5;
   6;
   7;
   8;
   9;
   2--1;
   3--0;
   4--0;
   5--0;
   5--2;
   5--3;
   6--0;
   6--1;
   6--2;
   6--3;
   7--2;
   7--4;
   7--5;
   7--6;
   8--3;
   8--6;
   9--2;
   9--8;
}

 * Wypisujemy także wierzchołki, ponieważ mogą być niepołączone z innymi
 */

string Graph::to_dot() const
{
    ostringstream os;
    if (is_directed)
        os << "digraph G {" << endl;
    else
        os << "graph G {" << endl;

    for (int i = 0; i < T.size(); i++)
    {
        os << "\t" << i << ";" << endl;
    }
    if (is_directed)
    {
        int j = 0;
        for (const auto &row : T)
        {
            int k = 0;
            for (auto x : row)
            {
                if (x == 1)
                {
                    os << "\t" << j << "->" << k << ";" << endl;
                }
                k++;
            }
            j++;
        }
    }
    else
    {
        int j = 0;
        for (const auto &row : T)
        {
            int k = 0;
            for (auto x : row)
            {
                if (x == 1)
                {
                    os << "\t" << j << "--" << k << ";" << endl;
                }
                k++;
            }
            j++;
        }
    }
    os << "}" << endl;

    //????
    return os.str();
}

string Graph::to_mat() const
{
    int n = T.size();
    ostringstream os;
    for (const auto &row : T)
    {
        for (auto x : row)
        {
            os << x << " ";
        }
        os << std::endl;
    }
    //????
    return os.str();
}

//########DFS_iterator
#include <stack>

/*
 * Klasa pozwala na iterację po grafie (odwiedzanie kolejnych wierzchołków) w porzadku depth first
 * Klasa ma uproszczony interfejs iteratora, który moza podsumowac następująco
   DFSIterator it(g,0);
   while(it.has_next()){
        int node = it.next();
        cout<<node<<" ";
   }
 * Harmonogram odwiedzin wierzchołków określa stos LIFO
 *
 */

class DFSIterator
{
    // referencja wskazująca graf
    const Graph &g;
    // stos wierzchołków do odwziedzenia
    stack<int> to_visit;
    // zbiór odwiedzonych wierzchołków
    set<int> visited;
    /*
     * W przypadku grafu stos moze zawierać powtórzenia. Wierzchołek zostaje odwiedzony wcześniej i dalej
     * być przechowywany na stosie
     * Jeżeli flaga visit_once==true, takie odwiedozne wierzchołki będziemy usuwać ze stosu
     */
    bool visit_once;

public:
    DFSIterator(const Graph &_g, int start_node, bool visit_once = false);
    /*
     * Czy możliwe jest przjeście do następnego elementu - czyli czy stos nie jest pusty
     */
    bool has_next() const;
    /*
     * Przechodzi na następny wierzchołek i zwraca go
     */
    int next();
};

DFSIterator::DFSIterator(const Graph &_g, int start_node, bool visit_once) : g(_g)
{
    //??? od czego zaczynamy?
    this->visit_once = visit_once;

    for (int i = 0; i < g.T.size(); i++)
    {
        if (i != start_node)
        {
            to_visit.push(i);
        }
    }
    this->visited.clear();
}

bool DFSIterator::has_next() const
{
    // return ???
    return !to_visit.empty();
}

int DFSIterator::next()
{

    // pobierz pierwszy element ze stosu - current
    // dodaj current do visited
    // pobierz zbiór N(current) wierzchołków bezpośrednio osiągalnych z current
    // dla każego wierzchołka v in N(current)
    // jeżeli nie był odwiedzony - umieść go na stosie
    //
    // jeżeli flaga visit_once jest ustawiona
    // w pętli usuń wszystkie elementy ze szczytu stosu, które były odwiedzone
    // zwróć current
    int current = to_visit.top();
    to_visit.pop();
    visited.insert(current);
    set<int> N;
    N = g.get_linked_nodes(current);

    for (auto v : N)
    {
        if (visited.find(v) == visited.end())
        {
            to_visit.push(v);
        }
    }
    // cout << "d111" <<endl;
    if (visit_once)
    {
        while (!to_visit.empty())
        {
            if (visited.find(to_visit.top()) != visited.end())
            {
                to_visit.pop();
            }
            else
            {
                break;
            }
        }
    }
    return current;
}

#include <queue>
/*
 * Klasa pozwala na iterację po grafie (odwiedzanie kolejnych wierzchołków) w porzadku breadth first
 * Klasa ma uproszczony interfejs iteratora, który moza podsumowac następująco
   BFSIterator it(g,0);
   while(it.has_next()){
        int node = it.next();
        cout<<node<<" ";
   }
 * Harmonogram odwiedzin wierzchołków określa kolejka FIFO
 */

class BFSIterator
{
    // referencja wskazująca graf
    const Graph &g;
    // kolejka wierzchołków
    queue<int> to_visit;
    // zbiór odwiedzonych wierzchołków
    set<int> visited;
    /*
     * W przypadku grafu stos moze zawierać powtórzenia. Wierzchołek zostaje odwiedzony wcześniej i dalej
     * być przechowywany na stosie
     * Jeżeli flaga visit_once==true, takie odwiedozne wierzchołki będziemy usuwać ze stosu
     */
    bool visit_once;

public:
    BFSIterator(const Graph &_g, int start_node, bool visit_once = false);
    /*
     * Czy możliwe jest przjeście do następnego elementu - czyli czy stos nie jest pusty
     */
    bool has_next() const;
    /*
     * Przechodzi na następny wierzchołek i zwraca go
     */
    int next();
};

BFSIterator::BFSIterator(const Graph &_g, int start_node, bool visit_once) : g(_g)
{
    //???
    this->visit_once = visit_once;
    this->visited.clear();
    for (int i = 0; i < g.T.size(); i++)
    {
        if (i != start_node)
        {
            to_visit.push(i);
        }
    }
}

bool BFSIterator::has_next() const
{
    return !to_visit.empty();
}

int BFSIterator::next()
{
    //???
    int current = to_visit.front();
    to_visit.pop();
    visited.insert(current);
    set<int> N;
    N = g.get_linked_nodes(current);

    for (auto v : N)
    {
        if (visited.find(v) == visited.end())
        {
            to_visit.push(v);
        }
    }
    // cout << "d111" <<endl;
    if (visit_once)
    {
        while (to_visit.empty() == false)
        {
            if (visited.find(to_visit.front()) != visited.end())
            {
                to_visit.pop();
            }
            else
            {
                break;
            }
        }
    }
    return current;
}

void test_random_dot_mat()
{
    Graph g;

    g.random(10, .2, true, false);
    // cout << "dddd";
    cout << g.to_mat() << endl;
    cout << g.to_dot() << endl;
}

void test_dfs_iterator1()
{
    srand(123);
    Graph g;

    g.random(10, .2, true, false);
    cout << g.to_mat() << endl;
    cout << g.to_dot() << endl;
    DFSIterator it(g, 0);
    // cout << "dd" << endl;
    while (it.has_next())
    {
        int node = it.next();
        cout << node << " ";
    }
    cout << endl;
}

void test_dfs_iterator2()
{
    srand(123);
    Graph g;
    g.random(10, .2, false, false);
    cout << g.to_mat() << endl;
    cout << g.to_dot() << endl;

    DFSIterator it(g, 0);
    while (it.has_next())
    {
        int node = it.next();
        cout << node << " ";
    }
    cout << endl;

    cout << "visit once: ";
    DFSIterator it2(g, 0, true);
    while (it2.has_next())
    {
        int node = it2.next();
        cout << node << " ";
    }
    cout << endl;
}

void test_bfs_iterator()
{
    srand(123);
    Graph g;
    g.random(10, .4, true, false);
    cout << g.to_mat() << endl;
    cout << g.to_dot() << endl;

    BFSIterator it(g, 0);
    while (it.has_next())
    {
        int node = it.next();
        cout << node << " ";
    }
    cout << endl;

    cout << "visit once: ";
    BFSIterator it2(g, 0, true);
    while (it2.has_next())
    {
        int node = it2.next();
        cout << node << " ";
    }
    cout << endl;
}

void test_bfs_iterator2()
{
    srand(123);
    Graph g;
    g.random(10, .2, false, false);
    cout << g.to_mat() << endl;
    cout << g.to_dot() << endl;

    BFSIterator it(g, 0);
    while (it.has_next())
    {
        int node = it.next();
        cout << node << " ";
    }
    cout << endl;

    cout << "visit once: ";
    BFSIterator it2(g, 0, true);
    while (it2.has_next())
    {
        int node = it2.next();
        cout << node << " ";
    }
    cout << endl;
}

int main()
{
    std::srand(std::time(nullptr));
    test_random_dot_mat();
    // test_dfs_iterator1();
    // test_dfs_iterator2();
    // test_bfs_iterator();
    // test_bfs_iterator2();
}
