//Program jest implementacją drzewa splay
//Przy wyszukiwaniu elementu drzewo jest modyfikowane tak, ze szukany element jest w korzeniu
//Program pozwala na wprowadzenie miasta z listą priorytetową klientow
// a nastepnie wyszukanie najwazniejszego klienta w danym miescie

#include <iostream>
#include <queue>

using namespace std;

struct client {
    string name;
    float priority;
    unsigned int visits;
};

struct compare{
    bool operator() (const client& c1, const client& c2){
        if(c1.priority < c2.priority)
            return true;
        return false;
    }
};

struct node {
    string city;
    priority_queue<client, vector<client>, compare> clients;
    node *father, *leftSon, *rightSon;
};

void rotationRight(node *&root, node *n){
    if(n->leftSon != nullptr){
        node *left = n->leftSon;         //zapamietuje lewego syna n
        node *father = n->father;
        if(left->rightSon != nullptr){    //jezeli prawy syn istnieje
            n->leftSon = left->rightSon;  //podpinam go jako lewgo syna n
            n->leftSon->father = n;
        }
        else
            n->leftSon = nullptr;
        left->rightSon = n;   //przypisuje n jako prawego syna
        left->father = father;
        n->father = left;     //i zmieniam mu ojca
        if(father != nullptr){   //jezeli n mial ojca to zmieniam mu syna
            if(father->rightSon == n)
                father->rightSon = left;
            else
                father->leftSon = left;
        }
        else              //jezeli nie to znaczy ze byl rootem
            root = left;
    }
}

void rotationLeft(node *&root, node *n){
    if(n->rightSon != nullptr){
        node *right = n->rightSon;
        node *father = n->father;
        if(right->leftSon != nullptr){
            n->rightSon = right->leftSon;
            n->rightSon->father = n;
        }
        else
            n->rightSon = nullptr;
        right->leftSon = n;
        right->father = father;
        n->father = right;
        if(father != nullptr){
            if(father->rightSon == n)
                father->rightSon = right;
            else
                father->leftSon = right;
        }
        else
            root = right;
    }
}

void splay(node *&root, string city){
    node *current = root;
    node *prev = current;
    while(current != nullptr && current->city != city){  //dopoki nie znajde wezla o danym kluczu
        prev = current;            //zapamietuje poprzednika
        if(current->city > city)
            current = current->leftSon;
        else
            current = current->rightSon;
    }
    if(current == nullptr)  //gdy nie ma w drzewie wezla o danym klucz mam poprzednika
        current = prev;

    while(current != root){                      //wykonuje dopoki dany wezel nie jest w korzeniu
        if(current->father == root){      //przypadek gdy ojcem drzewa jest korzen
            if(root->leftSon == current)
                rotationRight(root, root);
            else
                rotationLeft(root, root);
        }
        else if(current->father->leftSon == current && current->father->father->leftSon == current->father){
            rotationRight(root, current->father->father);    //current jest lewym synem i ojciec current jest lewym synem
            rotationRight(root, current->father);
        }
        else if(current->father->rightSon == current && current->father->father->rightSon == current->father){
            rotationLeft(root, current->father->father);    //current jest prawym synem i ojciec current jest prawym synem
            rotationLeft(root, current->father);
        }
        else if(current->father->rightSon == current && current->father->father->leftSon == current->father){
            rotationLeft(root, current->father);     //current jest prawym synem a jego ojciec lewym synem
            rotationRight(root, current->father);
        }
        else if(current->father->leftSon == current && current->father->father->rightSon == current->father){
            rotationRight(root, current->father);    //current jest lewym synem a jego ojciec prawym
            rotationLeft(root, current->father);
        }
    }
}

void insert(node *&root, string city, client cl){
    if(root == nullptr){
        root = new node;
        root->father = root->leftSon = root->rightSon = nullptr;
        root->city = city;
        root->clients.push(cl);
    }
    else {
        splay(root, city);
        if(root->city == city)
            root->clients.push(cl);
        else if(root->city > city){
            node *son = new node;
            son->city = city;
            son->clients.push(cl);
            son->leftSon = son->rightSon = nullptr;
            son->father = root;
            son->leftSon = root->leftSon;
            if(son->leftSon != nullptr)
                son->leftSon->father = son;
            root->leftSon = son;
        }
        else {
            node *son = new node;
            son->city = city;
            son->clients.push(cl);
            son->leftSon = son->rightSon = nullptr;
            son->father = root;
            son->rightSon = root->rightSon;
            if(son->rightSon != nullptr)
                son->rightSon->father = son;
            root->rightSon = son;
        }
    }
}

void deleteNode(node *&root, string city){
    splay(root, city);
    if(root->city == city){           //jezeli w korzeniu znajduje sie usuwany wezel
        node *left = root->leftSon;
        node *right = root->rightSon;
        delete root;
        if(right != nullptr){        //jezeli istnieje prawe poddrzewo
            right->father = nullptr;
            splay(right, city);
            right->leftSon = left;
            if(left != nullptr)
                left->father = right;
            root = right;
            }
        else if(left != nullptr){   //jezeli istnieje lewe poddrzewo
            left->father = nullptr;
            splay(left, city);
            left->rightSon = right;
            root = left;
        }
    }
}

int main() {

    ios_base::sync_with_stdio(0);
    unsigned int numberOfCommands, visits;
    float priority;
    string symbol, city, name;
    client cl;
    node *root = nullptr;

    cin >> numberOfCommands;
    for(unsigned int i = 0; i < numberOfCommands; i++){
        cin >> symbol;
        if(symbol == "A"){                                 //symbol A - użytkownik wpisuje miasto i klienta
            cin >> city >> name >> priority >> visits;
            insert(root, city, {name, priority, visits});
        }
        else {                                        //inny symbol (np F) - uzytkownik wyszukuje klienta w podanym miescie
            cin >> city;
            splay(root, city);
            if(root->city == city){
                cout << root->clients.top().name << endl;
                cl = root->clients.top();
                root->clients.pop();
                cl.visits--;
                if(cl.visits != 0)
                    root->clients.push(cl);
                if(root->clients.empty())
                    deleteNode(root, city);
            }
            else
                cout << "BRAK" << endl;  //jezeli nie ma klientow w danym miescie
        }
    }

    delete root;
    return 0;
}
