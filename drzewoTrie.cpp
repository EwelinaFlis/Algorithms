//Implementacja drzewa Trie
//Użytkownik może wpisać nowy element, ktory znajdzie sie w drzewie
// a także wyszukać wszystkie słowa, które zaczynają się od danej frazy
// i zostaną wyświetlone w kolejności alfabetycznej

#include <iostream>

using namespace std;

struct node {
    char key;
    bool end;
    node **sons;
};

void display(node *root, string prefix){  //funkcja do wyswietlania slow o danym prefiksie
    if(root->end)
        cout << prefix << endl;   //jezeli jest to koniec slowa wyswietlam je
    for(unsigned int i = 0; i < 26; i++)
           if(root->sons[i] != nullptr)    //jezeli istnieja kolejne litera przechodze do syna
               display(root->sons[i], prefix+root->sons[i]->key);
}

void insert(node *&root, string text){
    node *current = root;
    unsigned int i = 0;
    while(i < text.length()){      //przechodze po slowie ktore chce wpisac do drzewa
        int temp = text[i] - 97;   //zapamietuje numer znaku w alfabecie
        if(current->sons[temp] != nullptr)
            current = current->sons[temp];
        else {                              //jezeli pole nie ma syna z ta litera
            node *son = new node;
            son->key = text[i];
            son->sons = new node*[26];
            for(unsigned int j = 0; j < 26; j++)
                son->sons[j] = nullptr;
            son->end = false;
            current->sons[temp] = son;
            current = son;
        }
        i++;
    }
    current->end = true;  //zaznaczam ze ostatnie pole to koniec slowa
}

void search(node *root, string text){
    node *current = root;
    unsigned int i = 0;
    bool found = true;
    string result = "";

    while(i < text.length() && found){  //przechodze po slowie ktore chce znalezc
        int temp = text[i] - 97;
        if(current->sons[temp] != nullptr){
            current = current->sons[temp];
            result += current->key;  //zapamietuje odczytany do tej pory wyraz
        }
        else
            found = false;  //jezeli nie ma kolejnej litery z slowa - przerywam
        i++;
    }

    if(current->end && found)   //jezeli znaleziono slowo
        display(current, result);
    else if(!current->end && found){  //jezeli znaleziono prefiks
        found = false;
        for(unsigned int j = 0; j < 26; j++)
            if(current->sons[j] != nullptr){
                display(current->sons[j], result+current->sons[j]->key);
                found = true;
            }
    }
    if(!found)  //jezeli nie znaleziono slowa lub prefiksu
        cout << "-" << endl;
}

int main() {

    unsigned int number;
    char symbol;
    string text;

    cin >> number;

    node *root = new node;   //utworzenie korzenia
    root->end = false;
    root->key = 'a';
    root->sons = new node*[26];
    for(unsigned int i = 0; i < 26; i++)
            root->sons[i] = nullptr;

    for(unsigned int i = 0; i < number; i++){
        cin >> symbol;
        if(symbol == 'A'){  //symbol A - wczytanie nowe slowa lub prefiksu
            cin >> text;
            insert(root, text);
        }
        else {             //symbol (np S) - wyswietlenie wszystkich slow o danym prefiksie
            cin >> text;
            search(root, text);
        }
    }

    delete root;

    return 0;
}
