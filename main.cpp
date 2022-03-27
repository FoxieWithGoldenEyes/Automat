#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <conio.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
fstream file;
string fileName = "history.txt";
string recoveryFileName = "RecoveryFile.txt";
string blockedPasswordFileName = "BlockedPassword.txt";

string miesiace[] =
{ "stycznia","lutego","marca","kwietnia","maja", "czerwieca", "lipca", "sierpienia", "wrzesienia", "pazdziernika", "listopada", "grudnia" };
time_t SekundyCzasu;
tm DataICzas;
string produkty[10] = { "woda mineralna", "woda gazowana", "cola", "ciastka", "paluszki", "guma do zucia", "sok pomaranczowy", "chrupki kukurydziane", "croissant", "snickers" };
int grosze[] = { 50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10, 5, 2, 1 };
int colors[] = { 240, 155, 188, 206 };
float ceny[10] = { 3.0, 3.0, 3.5, 2.8, 2.5, 3.0,  4.0, 4.0, 3.8, 2.5 };
int uruchamianie;
int wybor;
int ilosc;
double rachunek;

int iloscProb = 3;

double input = 0.0;
string haslo = "1234";

void MyBeep(int czas)
{
    Beep(2137, czas);
}

void SetColor(int color)
{
    int kolor;
    kolor = colors[color];
    SetConsoleTextAttribute(hConsole, kolor);
}

void ClearScreen(int color)
{
    SetColor(color);
    system("cls");
}

void GetLocalTime()
{
    //Pobranie obecneego czasu
    time(&SekundyCzasu);
    //Zamiana obecnego czasu na strukturê tm
    localtime_s(&DataICzas, &SekundyCzasu);
}

//Nie wiem czy to jest legalne, ale mo¿e tak?
//U¿ycie dodatkowej aplikacji konsolowej z neta
//https://github.com/nbsp1221/beep-music/

void Alarm()
{
    cout << "Zaraz wlacze alarm. Uwaga!";
    for (int i = 0; i < 10; i++)
    {
        Sleep(500);
        cout << ".";
    }
    cout << endl;
    system("BeepMusic.exe Alarm.txt");
}

void HideFile(string fname)
{
    string command;
    command = "attrib +h " + fname;
    system(command.c_str());
}

bool FileExist(string filename)
{
    ifstream inputFile(filename);

    if (inputFile.good()) {
        return true;
    }
    else
    {
        return false;
    }
}

void CreateSeciurytyFile()
{
    if (!FileExist(blockedPasswordFileName))
    {
        ofstream f(blockedPasswordFileName);
        f << "PROGRAM DZIA£A W TRYBIE AWARYJNYM" << endl;
        f.close();
        HideFile(blockedPasswordFileName);
    }
}

void NaruszenieSystemu(string message)
{
    ClearScreen(1);
    cout << message << endl;
    cout << "Wlaczam procedure awaryjna" << endl;
    HideFile(fileName);
    CreateSeciurytyFile();
    Sleep(1000);
    Alarm();
    exit(0x436f7073);
}

bool SprawdzHaslo(string pass)
{
    if (pass == haslo)
    {
        cout << "Haslo poprawne!" << endl;
        return true;
    }
    else
    {
        cout << "Haslo niepoprawne!" << endl;
        if (iloscProb > 0)
        {
            iloscProb--;
        }
        else
        {
            NaruszenieSystemu("Has³o zosta³o zablokowane!");
        }
        return false;
    }
}

string AskForPassword()
{
    string password;
    cout << "Podaj haslo: ";
    cin >> password;
    return password;
}

//Dodawanie zera przez liczb¹ minut
string minutesToString(int minutes)
{
    string m;
    if (minutes < 10)
    {
        m += "0";
    }
    m += to_string(minutes);
    return m;
}

void reszta(double r)
{
    cout << "------------------------------\n";
    int kwota = 100 * r + 0.5; //zwiekszenie liczby, aby przy przemnozeniu przez 100, czesc ulamkowa, ktora bedzie ucinana nie wplynela na wartosc
    for (int k = 0; kwota > 0; k++)
    {
        int n = kwota / grosze[k];
        if (n > 0)
        {
            if (grosze[k] >= 100)
                cout << setw(3) << grosze[k] / 100 << " zl: ";
            else
                cout << setw(3) << grosze[k] << " gr: ";
            cout << n << endl;
            kwota %= grosze[k];
        }
    }
    cout << "------------------------------\n";
}

void StartUp()
{
    cout << "Automat uruchamia sie, prosze czekac ;)\n";

    for (int i = 100; i > 0; i--)
    {
        switch (i)
        {
        case 100:
            cout << i << endl;
            MyBeep(200);
            Sleep(20);
            system("cls");
            break;
        case 80:
            cout << i << endl;
            MyBeep(200);
            Sleep(20);
            system("cls");
            break;
        case 60:
            cout << i << endl;
            MyBeep(200);
            Sleep(20);
            system("cls");
            break;
        case 40:
            cout << i << endl;
            MyBeep(200);
            Sleep(20);
            system("cls");
            break;
        case 20:
            cout << i << endl;
            MyBeep(200);
            Sleep(20);
            system("cls");
            break;
        default:
            cout << i << endl;
            Sleep(20);
            system("cls");
            break;
        }
    }
    MyBeep(200);
}

double CoinOk(double coin)
{
    int groszy = coin * 100;
    for (int i = 0; i < 15; i++)
    {
        if (groszy == grosze[i])
        {
            return coin;
        }
    }
    return -1;
}

//Zapis do pliku
void WriteToFile(tm date, string zamowienie)
{
    file.open(fileName, ios::out | ios::app);

    file << "Data:    " << date.tm_mday << " " << miesiace[date.tm_mon] << " " << 1900 + date.tm_year << "  ";     //Bez endl - w jedej lini
    file << "[" << date.tm_hour << ":" << minutesToString(date.tm_min) << "]" << endl;
    file << "Ilosc:   " << ilosc << endl;
    file << "Produkt: " << zamowienie << endl << endl;

    file.close();
}

//Kasowanie pliku
void ClearFile()
{

    if (SprawdzHaslo(AskForPassword()))
    {
        file.open(fileName, ios::out | ios::trunc);
        file.close();
    }
}

//Kupowanie produktów
void Kupowanie()
{   
    cout << "Wybierz przekaske :D\n";
    cout << "nr     cena        produkt\n";
    for (int i = 0; i < 10; i++)
    {
        cout << i << "   |   " << ceny[i] << "zl      " << produkty[i] << endl;
    }

    do {
        cout << "Wpisz nr produktu: ";
        cin >> wybor;

        cout << "Wypisz ilosc sztuk produktu: ";
        cin >> ilosc;

        rachunek = ilosc * ceny[wybor];

        GetLocalTime();
        WriteToFile(DataICzas, produkty[wybor]);

    } while (wybor < 0 || wybor>9);
    do {
        cout << "Obecna kwota: " << input << endl;
        cout << "Do zaplaty jeszcze: " << rachunek - input << "zl" << endl;
        cout << "Wrzuc monete\n(format: ZZ.GG, np. 2.00 jako 2 zlote lub 0.50 jako 50 groszy): ";

        double coin;
        cin >> coin;
        
        double function_result = CoinOk(coin);
        if (function_result == -1)
        {
            NaruszenieSystemu("Nie ma takiej monety. Oj dorwa cie gliny za falszerstwo!");
        }
        else
        {
            input += function_result;
        }

    } while (input < rachunek);


    cout << "------------------------------\n";
    cout << "Wydano " << ilosc << " sztuk " << produkty[wybor] << endl;
    cout << "------------------------------\n";
    if (input == rachunek)
    {
        cout << "Bez reszty\n";
        cout << "------------------------------\n";
    }
    else
    {
        cout << "Reszta:" << endl;
        float roznica = input - rachunek;
        reszta(roznica);
    }
}

//Przegl¹danie pliku w konsoli
void ShowFile()
{
    if (SprawdzHaslo(AskForPassword()))
    {
        file.open(fileName, ios::in | ios::app);

        string str;

        int lineCounter = 0;
        while (getline(file, str))
        {
            cout << lineCounter << ") " << str << endl;
            lineCounter++;
        }

        file.close();
    }
}

void WeitForUserReaction()
{
    cout << "\nKliknij enter by kontynuowac" << endl;
    //Je¿eeli wciœniêto (w kodzie - dowolny) kawisz to przejdŸ do kodu dalej
    //_kbhit() - zwraca true je¿eeli wciœniêto klawisz - tutaj: !_kbhit() - je¿eli nie wciœniêto klawisza
    while (!_kbhit())
    {
        Sleep(50);
    }
}

void NormalRun()
{
    ClearScreen(0);
    cout << "------------------------------------------------\n";
    cout << " AUTOMAT Z PRZEKASKAMI\n";
    cout << "------------------------------------------------\n";
    do
    {
        cout << "Aby uruchomic automat nacisnij 1" << endl;
        cin >> uruchamianie;
    } while (uruchamianie != 1);

    StartUp();
    ClearScreen(1);

    // Lista opcji
    while (true)
    {
        cout << "0. Wyjscie" << endl;
        cout << "1. Kupowanie produktu" << endl;
        cout << "2. Histoia kupowania" << endl;
        cout << "3. Czyszczenie historii" << endl;
        short input;
        cin >> input;
        if (input == 1)
        {
            ClearScreen(3);
            Kupowanie();
        }
        else if (input == 2)
        {
            ClearScreen(3);
            ShowFile();
        }
        else if (input == 3)
        {
            ClearScreen(3);
            ClearFile();
        }
        else exit(0);
        WeitForUserReaction();
        ClearScreen(1);
    }
}

int main()
{
    //NormalRun();
    if (FileExist(recoveryFileName))
    {
        string pass;
        ifstream recoveryFile(recoveryFileName);
        getline(recoveryFile, pass);
        recoveryFile.close();

        if (SprawdzHaslo(pass))
        {
            cout << "Uff. Fajnie, ze admin przyjechal..." << endl;
            cout << "Odblokowuje dostep!" << endl;
            iloscProb = 3;
            remove(recoveryFileName.c_str());
            remove(blockedPasswordFileName.c_str());
            WeitForUserReaction();
            NormalRun();
        }
        else
        {
            cout << "O nie. Trafil sie jakis haker! Uciekac!" << endl;
            remove(recoveryFileName.c_str());
            remove(fileName.c_str());
            exit(0);
        }
    }
    if (FileExist(blockedPasswordFileName))
    {
        NaruszenieSystemu("Znowu ty? Poczekaj jeeszcze chwile, a zobaczysz swiat zza krat!");
        exit(0);
    }
    else
    {
        NormalRun();
    }


    return 0;
}




