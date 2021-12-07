#include <iostream>
#include <conio.h>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::wcout;

bool koniecGry;
const int szerokoscPlanszy = 40; //Zmienna ustawiająca szerokosc planszy w tabeli
const int wysokoscPlanszy = 20; //Zmienna ustawiająca wysokosc planszy w tabeli
int x, y; //Zmienne dla pozycji glowy
int owocX, owocY; //Zmienne dla pozycji owoców
int wynik; //Zmienna zapisująca wynik
int ogonX[100], ogonY[100]; //Rysowanie drugiej tablicy wyswietlajacej ogon
int nOgon = 2; // Zmienna określająca długość ogona
enum class eKierunek { STOP = 0, LEWO, PRAWO, GORA, DOL };
eKierunek dir; // zmienna utrzymująca kierunek wężą
void Ustawienia()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    koniecGry = false;
    dir = eKierunek::GORA; // Jeśli ustawimy STOP to waż nie bedzie sie ruszal dopoki nic nie nacisniemy
    x = szerokoscPlanszy / 2;  //Ustawiamy w tym miejscu pozycje węża
    y = wysokoscPlanszy / 2;  //Ustawiamy w tym miejscu pozycje węża
    owocX = rand() % szerokoscPlanszy; // Ustawiamy owoce w losowych miejscach na planszy
    owocY = rand() % wysokoscPlanszy;  // Ustawiamy owoce w losowych miejscach na planszy
    wynik = 0;
}
void Rysowanie()
{
    COORD c;
    c.X = 0;
    c.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    // szerokoscPlanszy ustawiamy + 2, poniewaz mamy wydrukowane 20 spacji wiec szerokosc musi byc szersza o 2
    for (int i = 0; i < szerokoscPlanszy + 2; i++) //Rysowanie gornej lini planszy
        cout << "-";
    cout << endl;

    for (int i = 0; i < wysokoscPlanszy; i++) //Rysowanie boków planszy
    {
        for (int j = 0; j < szerokoscPlanszy; j++)
        {
            if (j == 0)
                cout << "|"; //Rysowanie boków w ostatnich i pierwszych indeksach kolumn
            if (i == y && j == x)
                cout << "@"; // Drukowanie glowy
            else if (i == owocY && j == owocX)
                cout << "O"; // drukowanie owocu
            else
            {
                bool rysowanie = false;
                //pętla wyświetlająca ogon na tablicy
                for (int k = 0; k < nOgon; k++)
                {
                    if (ogonX[k] == j && ogonY[k] == i)
                    {
                        cout << "#";
                        rysowanie = true;
                    }
                }
                if (!rysowanie)
                    cout << " ";
            }


            if (j == szerokoscPlanszy - 1)
                cout << "|";
        }
        cout << endl;
    }

    for (int i = 0; i < szerokoscPlanszy + 2; i++)  //Rysowanie dolnej lini planszy.
        cout << "-";
    cout << endl;
    cout << "        Wynik:" << wynik << endl;
}
void Wejscie()
{
    if (GetAsyncKeyState(VK_LEFT))
    {
        dir = eKierunek::LEWO;
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        dir = eKierunek::PRAWO;
    }

    if (GetAsyncKeyState(VK_DOWN))
    {
        dir = eKierunek::DOL;
    }
    
    if (GetAsyncKeyState(VK_UP))
    {
        dir = eKierunek::GORA;
    }

    if (GetAsyncKeyState(VK_ESCAPE))
    {
        koniecGry = true;
    }
}
void Logika()
{
    int poprzedniX = ogonX[0];
    int poprzedniY = ogonY[0];
    int poprzedni2X, poprzedni2Y;
    ogonX[0] = x;
    ogonY[0] = y;
    for (int i = 1; i < nOgon; i++)
    {
        poprzedni2X = ogonX[i];
        poprzedni2Y = ogonY[i];
        ogonX[i] = poprzedniX;
        ogonY[i] = poprzedniY;
        poprzedniX = poprzedni2X;
        poprzedniY = poprzedni2Y;
    }
    switch (dir)
    {
    case eKierunek::LEWO:
        x--;  //Jeśli idziemy w lewo to zmniejszamy wspolrzedna x
        break;
    case eKierunek::PRAWO:
        x++; //Jeśli idziemy w prawo to zwiekszamy wspolrzedna x
        break;
    case eKierunek::GORA:
        y--;
        break;
    case eKierunek::DOL:  //Jeśli idziemy w góre to zmniejszamy wspolrzedna y
        y++;
        break;
    default: //Jeśli idziemy w dół to zwiekszamy wspolrzedna y
        break;
    }
    if (x > szerokoscPlanszy || x < 0 || y > wysokoscPlanszy || y < 0)
        koniecGry = true;
    //W przypadku jeśli chcemy aby po najechaniu na ściane wąż wychodził z naprzeciwnej ściany należy
   //usunąć powyższe 2 linie kodu i zamiast tego wstawić poniższe instrukcje warunkowe
    /*
    if (x >= szerokoscPlanszy)
    {
        x = 0;
    }
    else if (x < 0)
    {
        x = szerokoscPlanszy - 1;
    }
    if (y >= wysokoscPlanszy)
    {
        y = 0;
    }
    else if (y < 0)
    {
        y = wysokoscPlanszy - 1;
    }
    */
    for (int i = 0; i < nOgon; i++)
        if (ogonX[i] == x && ogonY[i] == y)
            koniecGry = true;

    if (x == owocX && y == owocY)
    {
        wynik += 1;
        //Wstawiamy tu ponownie kod losowo umieszczajacy na planszy owoc, aby po zjedzeniu go pojawil sie nowy
        owocX = rand() % szerokoscPlanszy;
        owocY = rand() % wysokoscPlanszy;
        nOgon++;
    }
}
int main()
{
    Ustawienia();
    while (!koniecGry)
    {
        Rysowanie();
        Wejscie();
        Logika();
        Sleep(33); //sleep(10);
    }
    if (koniecGry = true)
    {
        cout << endl;
        cout << "              GAME OVER" << endl << "           Twoj wynik to: " << wynik << endl;
    }
    return 0;
}
