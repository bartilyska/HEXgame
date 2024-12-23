#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <string.h>
#include <fstream>
#define DLUGOSC 100
#define SZEROKOSC 40
#define PLANSZA 11
#define PLANSZASKOS 30
using namespace std;
int liczPionki(char tab[][DLUGOSC], int rozmiar)
{
    int licz = 0;
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; tab[i][j] != '\0'; j++)
        {
            if (tab[i][j] == 'b' || tab[i][j] == 'r')
                licz++;
        }
    }
    return licz;
}
bool porownajPionki(char tab[][DLUGOSC], int rozmiar)
{
    int liczb = 0, liczr = 0;
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; tab[i][j] != '\0'; j++)
        {
            if (tab[i][j] == 'b')
                liczb++;
            else if (tab[i][j] == 'r')
                liczr++;
        }
    }
    if (liczb == liczr || liczb == liczr - 1)
        return true;
    else return false;
}
void utworzPlansze(char plansza[PLANSZA][PLANSZA], char tablica[SZEROKOSC][DLUGOSC], int indeks)
{
    char pionki[PLANSZASKOS][PLANSZASKOS];
    for (int i = 0; i < PLANSZASKOS; i++)
    {
        for (int j = 0; j < PLANSZASKOS; j++)
        {
            pionki[i][j] = '0'; //wyzeruj
        }
    }
    int pionkix = 0, pionkiy = 0;
    for (int i = 1; i < indeks - 1; i++) //pierwsza i ostatnia linia to są --- bez pionkow
    {
        for (int j = 0; tablica[i][j] != '\0'; j++)
        {
            if (tablica[i][j] == '<')
            {
                if (tablica[i][j + 2] == ' ') //zamiana spacji na kropki bylo latwiej debugowac
                    tablica[i][j + 2] = '.';
                pionki[pionkiy][pionkix] = tablica[i][j + 2]; // po < jest litera z pionkiem
                pionkix++;
            }
        }
        pionkix = 0;
        pionkiy++;
    }
    int rozmiar = (indeks - 2) / 2 + 1;
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            plansza[i][j] = '0'; //zerowanie planszy
        }
    }
    for (int i = 0; i <= pionkiy / 2; i++)
    {
        int wys = 0, xowa = rozmiar - 1 - i;
        for (int j = 0; pionki[i][j] != '0'; j++)
        {
            plansza[wys][xowa] = pionki[i][j]; //wypelnianie po ukosie dwiema petlami for
            xowa++;
            wys++;
        }
    }
    for (int i = pionkiy / 2 + 1; i < pionkiy; i++)
    {
        int wys = 1 + i - (pionkiy / 2 + 1), xowa = 0;
        for (int j = 0; pionki[i][j] != '0'; j++)
        {
            plansza[wys][xowa] = pionki[i][j];
            xowa++;
            wys++;
        }
    }
}
void KopiaPlansza(char kopia[PLANSZA][PLANSZA], char plansza[PLANSZA][PLANSZA], int rozmiar)
{
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            kopia[i][j] = plansza[i][j];
        }
    }
}
bool mozliwy(int y, int x, int rozmiar)
{
    if (y < rozmiar && y >= 0 && x >= 0 && x < rozmiar)
        return true;
    else return false;
}
//oddzielne dfsy zeby przyspieszyc kolejnosc sprawdzania
bool dfs_r(int y, int x, char plansza[PLANSZA][PLANSZA], bool odwiedzone[PLANSZA][PLANSZA], int rozmiar)
{ //ZWRACA TRUE GDY ZNAJDZIE POLACZENIE DO KONCOWKI
    odwiedzone[y][x] = true;
    if (y == rozmiar - 1)
        return true;
    int ruch_x[6] = { 0,-1,1,-1,1,0 };
    int ruch_y[6] = { 1,1,0,0,-1,-1 };
    for (int i = 0; i < 6; i++)
    {
        int nowe_y = y + ruch_y[i];
        int nowe_x = x + ruch_x[i];
        if (mozliwy(nowe_y, nowe_x, rozmiar) && plansza[nowe_y][nowe_x] == 'r' && !odwiedzone[nowe_y][nowe_x])
            if (dfs_r(nowe_y, nowe_x, plansza, odwiedzone, rozmiar))
                return true;
    }
    return false;
}
bool dfs_b(int y, int x, char plansza[PLANSZA][PLANSZA], bool odwiedzone[PLANSZA][PLANSZA], int rozmiar)
{
    odwiedzone[y][x] = true;
    if (x == rozmiar - 1)
        return true;
    int ruch_x[6] = { 1,1,0,0,-1,-1 };
    int ruch_y[6] = { 0,-1,1,-1,0,1 };
    for (int i = 0; i < 6; i++)
    {
        int nowe_y = y + ruch_y[i];
        int nowe_x = x + ruch_x[i];
        if (mozliwy(nowe_y, nowe_x, rozmiar) && plansza[nowe_y][nowe_x] == 'b' && !odwiedzone[nowe_y][nowe_x])
            if (dfs_b(nowe_y, nowe_x, plansza, odwiedzone, rozmiar))
                return true;
    }
    return false;
}
bool CzyKoniec_b(char plansza[PLANSZA][PLANSZA], int rozmiar)
{ //puszcza dfs od jednej krawedzi do drugiej
    bool odwiedzone[PLANSZA][PLANSZA] = { false };
    bool odp = 0;
    for (int i = 0; i < rozmiar; i++)
    {
        if (plansza[i][0] == 'b' && !odwiedzone[i][0])
        {
            if (dfs_b(i, 0, plansza, odwiedzone, rozmiar))
            {
                odp = true;
                break;
            }
        }
    }
    return odp;
}
bool CzyKoniec_r(char plansza[PLANSZA][PLANSZA], int rozmiar)
{
    bool odwiedzone[PLANSZA][PLANSZA] = { false };
    bool odp = 0;
    for (int i = 0; i < rozmiar; i++)
    {
        if (plansza[0][i] == 'r' && !odwiedzone[0][i])
        {
            if (dfs_r(0, i, plansza, odwiedzone, rozmiar))
            {
                odp = true;
                break;
            }
        }
    }
    return odp;
}
int liczTypPionka(char plansza[PLANSZA][PLANSZA], int rozmiar, char znak)
{
    int pionek = 0;
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            if (plansza[i][j] == znak)
                pionek++;
        }
    }
    return pionek;
}
void Is_Possible(char plansza[PLANSZA][PLANSZA], int rozmiar)
{
    int czerwone = liczTypPionka(plansza, rozmiar, 'r');
    int niebieskie = liczTypPionka(plansza, rozmiar, 'b');
    if (czerwone == niebieskie) //ostatni ruch - blue
    {
        if (CzyKoniec_r(plansza, rozmiar) == true) //czerwoni nie moga miec wygranej
            printf("NO\n");
        else if (CzyKoniec_b(plansza, rozmiar) == true) //jezeli blue wygral to jest szansa ze plansza niepoprawna
        {
            bool czyok = 0;
            for (int i = 0; i < rozmiar; i++)
            {
                for (int j = 0; j < rozmiar; j++)
                {
                    if (plansza[i][j] == 'b')
                    {
                        plansza[i][j] = '.';
                        if (!CzyKoniec_b(plansza, rozmiar))
                        {//jezeli chociaz w jednej pozycji wczesniejszej nie bylo wina to jest ok
                            czyok = 1;//(istnieje ruch ktory bedzie tym wygrywajacym)
                            break;
                        }
                        plansza[i][j] = 'b';
                    }
                }
                if (czyok == 1)
                    break;
            }
            if (czyok == 0) //chociaz w jednej sytuacji musialo ruch wczesniej nie byc wygranej
                printf("NO\n");
            else printf("YES\n");
        }
        else
            printf("YES\n");
    }
    else if (czerwone == niebieskie + 1) //red analogicznie
    {
        if (CzyKoniec_b(plansza, rozmiar) == true)
            printf("NO\n");
        else if (CzyKoniec_r(plansza, rozmiar) == true)
        {
            bool czyok = 0;
            for (int i = 0; i < rozmiar; i++)
            {
                for (int j = 0; j < rozmiar; j++)
                {
                    if (plansza[i][j] == 'r')
                    {
                        plansza[i][j] = '.';
                        if (!CzyKoniec_r(plansza, rozmiar) == true)
                        {
                            czyok = 1;
                            break;
                        }
                        plansza[i][j] = 'r';
                    }
                }
            }
            if (czyok == 0)
                printf("NO\n");
            else printf("YES\n");
        }
        else
            printf("YES\n");
    }
    else
        printf("NO\n");
}
bool ObronaNaiwna(char plansza[PLANSZA][PLANSZA], int rozmiar, char znak, int ruchyblok)
{
    if (ruchyblok == 0)
        return false;
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            if (plansza[i][j] == '.')
            {
                plansza[i][j] = znak; //dostaw obronce na dowolne pole
                if (ruchyblok == 1)
                {
                    if (znak == 'r' && !CzyKoniec_r(plansza, rozmiar)) //jezeli niespowodowalo to wygranej przeciwnika to nie udalo sie obronic
                        return false;
                    else if (znak == 'b' && !CzyKoniec_b(plansza, rozmiar))
                        return false;
                }
                else
                {
                    if (!ObronaNaiwna(plansza, rozmiar, znak, ruchyblok - 1)) //generuj drugiego obronce
                        return false;
                }
                plansza[i][j] = '.';
            }
        }
    }
    return true;
}
bool SprawdzCzyWinNaiwny(char plansza[PLANSZA][PLANSZA], int rozmiar, char znak, int ruchydowin, int ruchyblok)
{
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            if (plansza[i][j] == '.') //jak puste pole to wstaw
            {
                plansza[i][j] = znak;
                if (ruchydowin == 1) //jezeli nie mozna dostawic wiecej pionkow
                {
                    if (znak == 'r' && CzyKoniec_r(plansza, rozmiar)) //to sprawdz czy udalo sie wygrac
                    {
                        if (!ObronaNaiwna(plansza, rozmiar, 'b', ruchyblok)) //i obrona zawiodla, czyli dalo sie jakos upchac obroncow by nie przeszkadzali
                            return true;
                    }
                    else if (znak == 'b' && CzyKoniec_b(plansza, rozmiar))
                    {
                        if (!ObronaNaiwna(plansza, rozmiar, 'r', ruchyblok))
                            return true;
                    }
                }
                else
                {
                    if (znak == 'b' && !CzyKoniec_b(plansza, rozmiar) && SprawdzCzyWinNaiwny(plansza, rozmiar, 'b', ruchydowin - 1, ruchyblok)) //generuj drugiego atakujacego jezeli jeszcze nie wygral
                        return true;
                    else if (znak == 'r' && !CzyKoniec_r(plansza, rozmiar) && SprawdzCzyWinNaiwny(plansza, rozmiar, 'r', ruchydowin - 1, ruchyblok))
                        return true;
                }
                plansza[i][j] = '.'; //wroc do poprzedniego stanu planszy
            }
        }
    }
    return false;
}
void Can_Win_Naive(char plansza[PLANSZA][PLANSZA], int rozmiar)
{
    int czerwone = liczTypPionka(plansza, rozmiar, 'r');
    int niebieskie = liczTypPionka(plansza, rozmiar, 'b');
    if ((czerwone < rozmiar - 2 && niebieskie < rozmiar - 2) || CzyKoniec_b(plansza, rozmiar) || CzyKoniec_r(plansza, rozmiar))
        printf("NO\nNO\nNO\nNO\n\n"); //jak juz koniec gry lub zla liczba pionkow to nie sprawdzaj
    else
    {
        char kopia[PLANSZA][PLANSZA];
        KopiaPlansza(kopia, plansza, rozmiar); //kopia planszy zeby nie zgubic w trakcie edytowania
        if (czerwone == niebieskie)
        {
            int t[4][2] = { {1,0},{1,1},{2,1},{2,2} }; //tablica ile pionkow jakich bedzie dostawianych
            for (int i = 0; i < 4; i++)
            {
                char atak;
                if (i == 0 || i == 2)
                    atak = 'r';
                else atak = 'b';
                if (SprawdzCzyWinNaiwny(plansza, rozmiar, atak, t[i][0], t[i][1]))
                    printf("YES\n");
                else printf("NO\n");
                KopiaPlansza(plansza, kopia, rozmiar); //skopiuj po edycji z powrotem
            }
        }
        else if (czerwone == niebieskie + 1)
        {
            int t[4][2] = { {1,1},{1,0},{2,2},{2,1} };
            for (int i = 0; i < 4; i++)
            {
                char atak;
                if (i == 0 || i == 2)
                    atak = 'r';
                else atak = 'b';
                if (SprawdzCzyWinNaiwny(plansza, rozmiar, atak, t[i][0], t[i][1]))
                    printf("YES\n");
                else printf("NO\n");
                KopiaPlansza(plansza, kopia, rozmiar);
            }
        }
        else
            printf("NO\nNO\nNO\nNO\n");
    }
}
bool SprawdzCzyWinPerfect(char plansza[PLANSZA][PLANSZA], int rozmiar, char znak, int ruchydowin, int ruchyblok)
{
    for (int i = 0; i < rozmiar; i++)
    {
        for (int j = 0; j < rozmiar; j++)
        {
            if (plansza[i][j] == '.')
            {
                plansza[i][j] = znak;
                if (znak == 'r' && CzyKoniec_r(plansza, rozmiar) && ruchydowin == 1) //jezeli dana pozycja powoduje wygrana i nie ma wiecej atakujacych
                {
                    if (ruchyblok == 0) //jak nie ma obroncy to juz to win
                        return true;
                    plansza[i][j] = 'b'; //to wstaw tam obronce
                    if (SprawdzCzyWinPerfect(plansza, rozmiar, 'r', ruchydowin, ruchyblok - 1)) //sprawdz czy mimo bloku obroncy da sie gdzies indziej postawic i wygrac
                        return true;
                }
                else if (znak == 'b' && CzyKoniec_b(plansza, rozmiar) && ruchydowin == 1)
                {
                    if (ruchyblok == 0)
                        return true;
                    plansza[i][j] = 'r';
                    if (SprawdzCzyWinPerfect(plansza, rozmiar, 'b', ruchydowin, ruchyblok - 1))
                        return true;
                }
                else
                {
                    if (znak == 'b' && !CzyKoniec_b(plansza, rozmiar) && ruchydowin > 1 && SprawdzCzyWinPerfect(plansza, rozmiar, 'b', ruchydowin - 1, ruchyblok)) //generuj drugiego atakujacego
                        return true;
                    else if (znak == 'r' && !CzyKoniec_r(plansza, rozmiar) && ruchydowin > 1 && SprawdzCzyWinPerfect(plansza, rozmiar, 'r', ruchydowin - 1, ruchyblok))
                        return true;
                }
                plansza[i][j] = '.';
            }
        }
    }
    return false;
}
void Can_Win_Perfect(char plansza[PLANSZA][PLANSZA], int rozmiar)
{
    int czerwone = liczTypPionka(plansza, rozmiar, 'r');
    int niebieskie = liczTypPionka(plansza, rozmiar, 'b');
    if ((czerwone < rozmiar - 2 && niebieskie < rozmiar - 2) || CzyKoniec_b(plansza, rozmiar) || CzyKoniec_r(plansza, rozmiar))
        printf("NO\nNO\nNO\nNO\n\n");
    else
    {
        char kopia[PLANSZA][PLANSZA];
        KopiaPlansza(kopia, plansza, rozmiar);
        if (czerwone == niebieskie) //ruch czerwonych
        {
            bool winred = 0, winblue = 0;//flagi by nie sprawdzac bez sensu( np jak czerwony wygral to juz niebieski nie wygra)
            if (SprawdzCzyWinPerfect(plansza, rozmiar, 'r', 1, 0))
            {
                printf("YES\n");
                winred = 1;
            }
            else printf("NO\n");
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winred == 0 && SprawdzCzyWinPerfect(plansza, rozmiar, 'b', 1, 1))
            {
                printf("YES\n");
                winblue = 1;
            }
            else printf("NO\n");
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winblue == 0 && SprawdzCzyWinPerfect(plansza, rozmiar, 'r', 2, 1))
            {
                printf("YES\n");
                winred = 1;
            }
            else printf("NO\n");
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winred == 0)
            {
                bool odp = 0;
                for (int i = 0; i < rozmiar; i++)
                {
                    for (int j = 0; j < rozmiar; j++) //wstaw pierwszego obronce na dowolne
                    {
                        if (plansza[i][j] == '.')
                        {
                            plansza[i][j] = 'r';
                            if (!SprawdzCzyWinPerfect(plansza, rozmiar, 'b', 2, 1))
                            {
                                printf("NO\n");
                                odp = 1;
                                break;
                            }
                            plansza[i][j] = '.';
                            KopiaPlansza(plansza, kopia, rozmiar);
                        }
                    }
                    if (odp == 1)
                        break;
                }
                if (odp == 0)
                    printf("YES\n");
            }
            else
                printf("NO\n");
        }
        else if (czerwone == niebieskie + 1)
        {
            bool winred = 0, winblue = 0; //inna kolejnosc bo niebieski zaczyna a zapytanie o czerwonego
            char odp[4];
            if (SprawdzCzyWinPerfect(plansza, rozmiar, 'b', 1, 0)) //bez obrony to jak z naiwnym
            {
                odp[1] = 'y';
                winblue = 1;
            }
            else odp[1] = 'n';
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winblue == 0 && SprawdzCzyWinPerfect(plansza, rozmiar, 'r', 1, 1))
            {
                odp[0] = 'y';
                winred = 1;
            }
            else odp[0] = 'n';
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winred == 0 && SprawdzCzyWinPerfect(plansza, rozmiar, 'b', 2, 1))
            {
                odp[3] = 'y';
                winblue = 1;
            }
            else odp[3] = 'n';
            KopiaPlansza(plansza, kopia, rozmiar);
            if (winblue == 0)
            {
                bool ok = 0;
                for (int i = 0; i < rozmiar; i++)
                {
                    for (int j = 0; j < rozmiar; j++)
                    {
                        if (plansza[i][j] == '.')
                        {
                            plansza[i][j] = 'b';
                            if (!SprawdzCzyWinPerfect(plansza, rozmiar, 'r', 2, 1))
                            {
                                odp[2] = 'n';
                                ok = 1;
                                break;
                            }
                            plansza[i][j] = '.';
                            KopiaPlansza(plansza, kopia, rozmiar);
                        }
                    }
                    if (ok == 1)
                        break;
                }
                if (ok == 0)
                    odp[2] = 'y';
            }
            else odp[2] = 'n';
            for (int i = 0; i < 4; i++)
            {
                if (odp[i] == 'y')
                    printf("YES\n");
                else printf("NO\n");
            }
        }
        else
            printf("NO\nNO\nNO\nNO\n");
        printf("\n");
    }
}
int main()
{
    char linia[DLUGOSC];
    char tablica[SZEROKOSC][DLUGOSC]; //wchodzaca plansza
    int indeks = 0; //indeks do ktorego wiersza tablicy wstawic kolejna linie
    while (cin.getline(linia, DLUGOSC))
    {
        if (linia[0] >= 'A' && linia[0] <= 'Z') //Polecenie w inpucie
        {
            if (strcmp(linia, "BOARD_SIZE") == 0)
            {
                int odp = (indeks - 2) / 2 + 1;
                printf("%d\n", odp);
            }
            else if (strcmp(linia, "PAWNS_NUMBER") == 0)
            {
                int odp = liczPionki(tablica, indeks);
                printf("%d\n", odp);
            }
            else if (strcmp(linia, "IS_BOARD_CORRECT") == 0)
            {
                if (porownajPionki(tablica, indeks))
                    printf("YES\n");
                else printf("NO\n");
            }
            else //trzeba stworzyc plansze nie da sie od razu odpowiedziec
            {
                char plansza[PLANSZA][PLANSZA];
                utworzPlansze(plansza, tablica, indeks);
                int rozmiar = (indeks - 2) / 2 + 1;

                if (strcmp(linia, "IS_GAME_OVER") == 0)
                {
                    if (porownajPionki(tablica, indeks))
                    {
                        if (CzyKoniec_r(plansza, rozmiar))
                            printf("YES RED\n");
                        else if (CzyKoniec_b(plansza, rozmiar))
                            printf("YES BLUE\n");
                        else
                            printf("NO\n");
                    }
                    else printf("NO\n");
                }
                else if (strcmp(linia, "IS_BOARD_POSSIBLE") == 0)
                {
                    Is_Possible(plansza, rozmiar);
                }
                else if (strcmp(linia, "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT") == 0)
                {
                    cin.getline(linia, DLUGOSC);
                    cin.getline(linia, DLUGOSC);
                    cin.getline(linia, DLUGOSC);
                    Can_Win_Naive(plansza, rozmiar);
                }
                else if (strcmp(linia, "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT") == 0)
                {
                    cin.getline(linia, DLUGOSC);
                    cin.getline(linia, DLUGOSC);
                    cin.getline(linia, DLUGOSC);
                    Can_Win_Perfect(plansza, rozmiar);
                }
            }
            indeks = 0; //po poleceniu wczytuj nowa plansze
        }
        else
        {//wczyt linii
            if (linia[0] != '\0') //zeby nie wczytywac pustych linii ktore sa w testach
            {
                strcpy(tablica[indeks], linia);
                indeks++;
            }
        }
    }
    return 0;
}
