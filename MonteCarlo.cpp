//Zadanie rozwiązane metodą Monte Carlo
//Na wejściu użytkownik podaje dwie godziny w formacie HH:mm i numer (ktory jest przypisany do odpowiedniego pokoju)
//Program podaje jakie jest prawdopodobienstwo, ze znajdziemy w danym pokoju Grażynę w podanym przedziale czasu

#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

// przykladowa funkcja
int Grazyna(int HH, int mm){
    if(HH >= 8 && HH < 15) return 0;         // 0 - kuchnia
    else if(HH >= 15 && HH < 22) return 1;   // 1 - ogrod
    return 2;                                // 2 - sypialnia
}

int main(){

    int h1, m1, h2, m2, room, counter=0, hour, minutes;
    double result;
    char colon;

    cin >> h1 >> colon >> m1;
    cin >> h2 >> colon >> m2;
    cin >> room;

    for(int i = 0; i < 100000; i++){
        if(m2 == 0)
            hour = rand()%(h2-h1) + h1;
        else
            hour = rand()%(h2-h1+1) + h1;
        if(hour == h2)
            minutes = rand()%m2;
        else if(hour == h1)
            minutes = rand()%(60-m1)+m1;
        else
            minutes = rand()%60;
        if(Grazyna(hour, minutes) == room)
            counter++;
    }

    result = counter/100000.0;
    cout << fixed << setprecision(2) << result << endl;

    return 0;
}
