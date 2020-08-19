//Szukanie najkrótszej ścieżki w grafie - algorytm Dijkstry

#include <iostream>
#include <limits>
#include <queue>
#include <list>
#include <iomanip>

using namespace std;

struct neighbour{
    int field;
    float weight;
};

static float *path1;
static float *path2;

struct compare1{
    bool operator() (const int& v1, const int& v2){
        if(path1[v1] > path1[v2])
            return true;
        return false;
    }
};

struct compare2{
    bool operator() (const int& v1, const int& v2){
        if(path2[v1] > path2[v2])
            return true;
        return false;
    }
};

int main() {

    ios_base::sync_with_stdio(0);
    int numberOfFields, numberOfPaths, numberOfShops, index, field1, field2, current, time, *shops;
    float distance, expens, result;
    neighbour n1, n2;
    list<neighbour> *neighbours1, *neighbours2;
    priority_queue<int, vector<int>, compare1> firstQueue;
    priority_queue<int, vector<int>, compare2> secondQueue;

    cin >> numberOfFields >> numberOfPaths;

    path1 = new float[numberOfFields];  //dlugość ścieżki z pola 0 do innych pól
    path2 = new float[numberOfFields];  //dlugość ścieżki w drugą stronę
    neighbours1 = new list<neighbour> [numberOfFields];  //wagi krawędzi
    neighbours2 = new list<neighbour> [numberOfFields];  //wagi krawędzi w drugą stronę

    for(int i = 0; i < numberOfPaths; i++){    //wczytanie danych i obliczenie wagi krawedzi
        cin >> field1 >> field2 >> distance >> time;
        expens = distance*49 + time*18;
        n1.field = field2;  //tworzenie sąsiada dla field1
        n1.weight = expens;
        neighbours1[field1].push_back(n1);
        cin >> distance >> time;
        expens = distance*49 + time*18;
        n2.field = field1;  //tworzenie sąsiada dla field2
        n2.weight = expens;
        neighbours1[field2].push_back(n2);
        n2.weight = n1.weight;  //zamieniam wagi sąsiadów
        n1.weight = expens;
        neighbours2[field1].push_back(n1);  //wstawienie do list sąsiedztwa z odwrotnymi drogami
        neighbours2[field2].push_back(n2);
    }

    cin >> numberOfShops;            //wczytanie sklepów, które chce sie odwiedzic
    shops = new int[numberOfShops];
    for(int i = 0; i < numberOfShops; i++){
        cin >> index;
        shops[i] = index;
    }

    path1[0] = 0;
    path2[0] = 0;
    for(int i = 1; i < numberOfFields; i++){      //uzupelnienie tablic z dlugoscia sciezek
        path1[i] = numeric_limits<float>::max();  //przed wykonaniem algorytmu wszedzie jest "nieskonczonosc"
        path2[i] = numeric_limits<float>::max();
    }

    firstQueue.push(0);          //pierwsze przejście do grafie
    while(!firstQueue.empty()){
        current = firstQueue.top();
        firstQueue.pop();
        for(neighbour &n : neighbours1[current])
            if(path1[n.field] > path1[current] + n.weight){
                path1[n.field] = path1[current] + n.weight;
                firstQueue.push(n.field);
            }
    }

    secondQueue.push(0);           //drugie przejście po grafie
    while(!secondQueue.empty()){
        current = secondQueue.top();
        secondQueue.pop();
        for(neighbour &n : neighbours2[current])
            if(path2[n.field] > path2[current] + n.weight){
                path2[n.field] = path2[current] + n.weight;
                secondQueue.push(n.field);
             }
    }

    result = 0;
    for(int i = 0; i < numberOfShops; i++){  //zliczenie kosztów w obie strony do wszystkich sklepów
        result += path1[shops[i]];
        result += path2[shops[i]];
    }

    result = (result/numberOfShops)/100;  //obliczenie sredniej z kosztów

    cout << fixed << setprecision(3) << result << endl;

    return 0;
}
