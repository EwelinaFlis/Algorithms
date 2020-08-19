//Implementacja drzewa BST
//W zadaniu należało wczytać odpowiednią liczbę elementów
// a nastepnie wyszukać i usunąć elementy, jeśli znajdowały się w drzewie

#include <iostream>

using namespace std;

static unsigned int maxN; //maksymalna wartosc w drzewie

struct node{
    unsigned int value;
    string text;
    node *father, *leftSon, *rightSon;
};

void insert(node *root, unsigned int value, string text){
    node *current = root;
    while(!(current->value < value && current->rightSon == nullptr) && !(current->value > value && current->leftSon == nullptr)){
        if(current->value < value)
            current = current->rightSon;
        else
            current = current->leftSon;
    }
    node *son = new node;
    son->value = value;
    son->text = text;
    son->father = current;
    son->leftSon = son->rightSon = nullptr;
    if(current->value < value)
        current->rightSon = son;
    else
        current->leftSon = son;
    if(son->value > maxN)
        maxN = son->value;
}

void inorder(node *root){
    if(root->leftSon != nullptr)
        inorder(root->leftSon);

    cout << root->value << " " << root->text << endl;

    if(root->rightSon != nullptr)
        inorder(root->rightSon);

}

node *maxNode(node *root){
    node *current  = root;
    while(current->rightSon != nullptr)
        current = current->rightSon;
    return current;
}

node *minNode(node *root){
    node *current = root;
    while(current->leftSon != nullptr)
        current = current->leftSon;
    return current;
}

node *search(unsigned int value, node *root){   //funkcja szukania węzła o danej wartości
    node *current = root;
    while(current != nullptr && current->value != value){
        if(current->value < value)
            current = current->rightSon;
        else if(current->value > value)
            current = current->leftSon;
    }
    return current;
}

node *successor(node *root){  //szukanie następnego w kolejności wezła pod względem wielkości
    node *current  = root;
    if(root->value == maxN)
        return nullptr;
    if(current->rightSon != nullptr)
        return minNode(current->rightSon);
    while(current->father->rightSon == current)
        current = current->father;
    return current->father;
}

void deleteNode(unsigned int value, node *root){

    node *current = search(value, root);  //znajduje wezel do usuniecia
    if(current != nullptr){               //jezeli znaleziono wezel
        cout << current->text;
        node *father = current->father;  //zapamietuje ojca
        if(current->value == maxN)      //aktualizuje wartosc maxN
            maxN = maxNode(root)->value;

        if(current->leftSon != nullptr && current->rightSon != nullptr){  //jezeli usuwany ma dwoch synow
            node *suc = successor(current);  //szukam nastepnika
            swap(suc->text, current->text);  //zamieniam wartosci w wezlach
            swap(suc->value, current->value);
            current = suc;
        }

        if(current->leftSon != nullptr){
            if(current != root)
                current->leftSon->father = current->father;
            else {
                current->leftSon->father = nullptr;
                root = current->leftSon;
            }
        }
        if(current->rightSon != nullptr){
            if(current != root)
                current->rightSon->father = current->father;
            else {
                current->rightSon->father = nullptr;
                root = current->rightSon;
            }
        }
        if(current->rightSon == nullptr && current->leftSon == nullptr) {
            if(current == root)
                root = nullptr;
        }

        if(current->father != nullptr && current->father->leftSon == current){
            if(current->leftSon != nullptr)
                current->father->leftSon = current->leftSon;
            else if(current->rightSon != nullptr)
                current->father->leftSon = current->rightSon;
            else
                current->father->leftSon = nullptr;
        }
        if(current->father != nullptr && current->father->rightSon == current){
            if(current->leftSon != nullptr)
                current->father->rightSon = current->leftSon;
            else if(current->rightSon != nullptr)
                current->father->rightSon = current->rightSon;
            else
                current->father->rightSon = nullptr;
        }

        delete current;
        if(father != nullptr)                 //jezeli usuwany ma ojca to usuwam tez jego
            deleteNode(father->value, root);
    }
    else
        cout << "NIE";  //komunikat mowiacy, ze w drzewie nie ma wezla o takiej wartosci
}

int main() {

    ios_base::sync_with_stdio(0);
    unsigned int numberOfNodes, numberOfSearches, value;
    string text;
    node *root;

    cin >> numberOfNodes >> numberOfSearches;
    cin >> value >> text; //wczytanie pierwszego elementu (korzenia)
    root = new node;
    root->value = value;
    root->text = text;
    root->father = root->leftSon = root->rightSon = nullptr;
    maxN = value;

    for(unsigned int i = 1; i < numberOfNodes; i++){  //wczytanie kolejnych elementów
        cin >> value >> text;
        insert(root, value, text);
    }

    for(unsigned int i = 0; i < numberOfSearches; i++){  //usuwanie elementów
        cin >> value;
        deleteNode(value, root);
        cout << endl;
    }

    if(root != nullptr)
        delete root;

    return 0;
}
