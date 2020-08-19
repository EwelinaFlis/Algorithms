//Zadanie jest implementacją własnej kolejki stosu
//Użytkownik może wpisać dowolną ilość wyrażeń, np. 4+6*(5-1)
//Następnie jest sprawdzane czy nawiasy w wyrażeniu są poprawne
//Błędne wyrażenie to np. 4+6*5-1)
//Jeżeli wyrażenie jest pop

#include <iostream>
#include <string>

using namespace std;

class Queue {
    private:
        struct element{
            string value;
            element *next;
        };
        element *first, *last;

    public:
        Queue();
        ~Queue();
        bool empty();
        void push(string);
        void pop();
        string front();
};

Queue::Queue(){
    first = last = nullptr;
}

Queue::~Queue(){
    while(first)
        pop();
}

bool Queue::empty(){
    if(first == nullptr)
        return true;
    return false;
}

string Queue::front(){
    return first->value;
}

void Queue::push(string value){
    element *current = new element;
    current->next = nullptr;
    current->value = value;
    if(last)
        last->next = current;
    else
        first = current;
    last = current;
}

void Queue::pop(){
    element *current  = first;
    first = first->next;
    if(!first)
        last = nullptr;
    delete current;
}

template<typename type>
class Stack{
private:
    struct element {
        type value;
        element *next;
    };
    element *last;
public:
    Stack();
    ~Stack();
    bool empty();
    void push(type);
    void pop();
    type top();
};

template<typename type>
Stack<type>::Stack(){
    last = nullptr;
}

template<typename type>
Stack<type>::~Stack(){
    while(last)
        pop();
}

template<typename type>
bool Stack<type>::empty(){
    if(last == nullptr)
        return true;
    return false;
}

template<typename type>
void Stack<type>::push(type value){
    element *current = new element;
    current->value = value;
    current->next = last;
    last = current;
}

template<typename type>
void Stack<type>::pop(){
    element *current = last;
    last = last->next;
    delete current;
}

template<typename type>
type Stack<type>::top(){
    return last->value;
}

int main() {

    unsigned int numberOfEquations;
    string equation, current, symbol;
    double number1, number2;
    bool error = false;
    Stack<string> brackets;
    Stack<double> numbers;
    Queue symbols;

    cin >> numberOfEquations;
    cin.ignore();

    for(unsigned int i = 0; i < numberOfEquations; i++) {
        cin >> equation;
        error = false;

        while(!brackets.empty())  //czyszczenie kolejki i stosu
            brackets.pop();
        while(!symbols.empty())
            symbols.pop();
        while(!numbers.empty())
            numbers.pop();

        //umieszczenie wyrażenia w kolejce
        for(unsigned int i = 0; i < equation.length(); i++) {
            if(!error) {
                current = equation[i];
                if(current >= "0" && current <= "9") {                     //jezeli symbol to liczba
                    while(equation[i+1] <= '9' && equation[i+1] >= '0'){   //jeżeli liczba jest dwucyfrowa
                        current += equation[i+1];
                        i++;
                    }
                    symbols.push(current);
                }
                else if(current == "+" || current == "-") {                   //operator o niskim priorytecie
                    while((!brackets.empty()) && (brackets.top() == "+" || brackets.top() == "-" || brackets.top() == "*" || brackets.top() == "/")){
                        symbols.push(brackets.top());
                        brackets.pop();
                    }
                    brackets.push(current);
                }
                else if(current == "*" || current == "/"){            //operator o wysokim priorytecie
                    while((!brackets.empty()) && (brackets.top() == "*" || brackets.top() == "/")){
                        symbols.push(brackets.top());
                        brackets.pop();
                    }
                    brackets.push(current);
                }
                else if(current == "[" || current == "{" || current == "("){          //lewy nawias
                    brackets.push(current);
                }
                else if(current == "]" || current == "}" || current == ")"){         //prawy nawias
                    if(brackets.empty()){               //gdy wczytamy prawy nawias a na stosie nie ma nic (lewego nawiasu)
                        cout << "BLAD" << endl;
                        error = true;
                        break;
                    }
                    else if(!brackets.empty() && !error){ //jeśli jest coś na stosie sprawdzamy czy wczytany nawias ma swój odpowiednik
                        while(!brackets.empty() && brackets.top() != "[" && brackets.top() != "{" && brackets.top() != "("){
                            symbols.push(brackets.top());
                            brackets.pop();
                        }
                        if(brackets.empty()){
                            cout << "BLAD" << endl;
                            error = true;
                            break;
                        }
                        else if(!brackets.empty() && !error){   //sprawdzenie czy nawiasy sobie odpowiadają
                            if(brackets.top() == "[" && current == "]"){
                                brackets.pop();
                            }
                            else if(brackets.top() == "{" && current == "}"){
                                brackets.pop();
                            }
                            else if(brackets.top() == "(" && current == ")"){
                                brackets.pop();
                            }
                            else {
                                cout << "BLAD" << endl;
                                error = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if(!error){    //jeżeli nie było błedu w nawiasach
            while(!brackets.empty() && !error){
                symbol = brackets.top();
                if(symbol == "{" || symbol == "(" || symbol == "["){  //jeżeli na stosie został nawias
                    cout << "BLAD" << endl;
                    error = true;
                    break;
                }
                symbols.push(symbol);     //jeśli nie, to wszystko zostaje przepisane ze stosu do kolejki
                brackets.pop();
            }
        }

        if(!error){          //obliczenie wyniku
            while(!symbols.empty()){
                current = symbols.front();
                if(current == "+" || current == "-" || current == "*"  || current == "/"){
                    number2 = numbers.top();
                    numbers.pop();
                    number1 = numbers.top();
                    numbers.pop();
                    if(current == "+")        //wykonujemy odpowiednie działanie
                        number1 += number2;
                    else if(current == "-")
                        number1 -= number2;
                    else if(current == "*")
                        number1 *= number2;
                    else
                        number1 /= number2;
                    numbers.push(number1);
                }
                else {    //gdy wczytany symbol to liczba
                    number1 = stod(current);
                    numbers.push(number1);   //zapisuje się na stosie
                }
                symbols.pop();
            }
            cout  << numbers.top() << endl;  //jeśli nie było nigdzie błędu zostaje wyswietlony wynik

        }
    }

    return 0;
}
