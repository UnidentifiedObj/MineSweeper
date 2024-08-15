// Some really very exceedingly exceptionally extremely especially genuinely
// tremendously immensly vastly intensely remarkably highly supremely hugely
// truly seriously important things to note :P
// ↘ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↙
// →  DIMENSIONS FOR TERMINAL  ←
// →     COLUMN/Width : 140    ←
// →      ROW/Height : 40      ←
// → Everything else = Default ←
// ↗ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↖

#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#define black "\x1b[30m"
#define white "\x1b[97m"
#define dullwhite "\x1b[37m"
#define red "\x1b[31m"
#define green "\x1b[32m"
#define lightblue "\x1b[36m"
#define blink "\x1b[5m"
#define reset "\x1b[0m"
#define bg "\033[41m"
#define bgblue "\033[44m"
#define resetbg "\033[40m"
#define rightspace 30
#define topspace 2
#define dRIP *(int *)&
#define s 4
#define bomb 249
#define idsize 10

struct Board
{
    char isOpen;
    char val[s];
    char isFlag;
};
struct FF
{
    char r[s];
    char c[s];
};
struct Acc
{
    char id[idsize];
    char besttime[s];
    char games[s];
    char wins[s];
};
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
void makeBoard(char& row, char& col, Board**& box)
{
    box = new Board * [dRIP row];
    char i[s];
    for (dRIP i = 0; dRIP i < dRIP row; (dRIP i)++)
        *(box + dRIP i) = new Board[dRIP col];
}
void printWin()
{
    gotoRowCol(26, 0);
    std::cout << green << R"( $$$$$$\  $$\   $$\ )" << std::endl;
    std::cout << R"($$  __$$\ $$ | $$  |)" << std::endl;
    std::cout << R"($$ /  $$ |$$ |$$  / )" << std::endl;
    std::cout << R"($$ |  $$ |$$$$$  /  )" << std::endl;
    std::cout << R"($$ |  $$ |$$  $$<   )" << std::endl;
    std::cout << R"($$ |  $$ |$$ |\$$\  )" << std::endl;
    std::cout << R"( $$$$$$  |$$ | \$$\ )" << std::endl;
    std::cout << R"( \______/ \__|  \__|)";
}
void printLose()
{
    gotoRowCol(26, 0);
    std::cout << red << " ______   _______  ______  " << std::endl;
    std::cout << "|      | |       ||      | " << std::endl;
    std::cout << "|  _    ||    ___||  _    |" << std::endl;
    std::cout << "| | |   ||   |___ | | |   |" << std::endl;
    std::cout << "| |_|   ||    ___|| |_|   |" << std::endl;
    std::cout << "|       ||   |___ |       |" << std::endl;
    std::cout << "|______| |_______||______| " << std::endl;
}

void printMsg()
{
    gotoRowCol(0, 0);
    std::cout << white;
    std::cout << " _______ __              " << std::endl;
    std::cout << "|   |   |__|.-----.-----." << std::endl;
    std::cout << "|       |  ||     |  -__|" << std::endl;
    std::cout << "|__|_|__|__||__|__|_____|" << std::endl;
    std::cout << " Sweeper|   Developed By " << std::endl;
    std::cout << "        | Baingan Studios" << std::endl;
}
void printMsgWithID(char* id)
{
    printMsg();
    char i[s];
    std::cout << std::endl
        << green << "Playing As : " << white;
    for (dRIP i = 0; id[dRIP i] != '\0'; (dRIP i)++)
        std::cout << *(id + dRIP i);

    std::cout << std::endl;
}
void printFlag(char& flag, char& col, char& cL)
{
    gotoRowCol(10, (dRIP col * cL) + rightspace + 10);
    std::cout << green;
    std::cout << "|------------------------|" << std::endl;
    gotoRowCol(11, (dRIP col * cL) + rightspace + 10);
    std::cout << "|       |~~~~~~~~.       |" << std::endl;
    gotoRowCol(12, (dRIP col * cL) + rightspace + 10);
    std::cout << "|       | " << white << " FLAG " << green << " |       |" << std::endl;
    gotoRowCol(13, (dRIP col * cL) + rightspace + 10);
    std::cout << "|       |~~~~~~~~'       |" << std::endl;
    gotoRowCol(14, (dRIP col * cL) + rightspace + 10);
    std::cout << "|       |                |" << std::endl;
    gotoRowCol(15, (dRIP col * cL) + rightspace + 10);
    std::cout << "|-------|----------------|" << std::endl;

    if (dRIP flag == 1)
    {
        gotoRowCol(16, (dRIP col * cL) + rightspace + 10);
        std::cout << lightblue << "Active    ";
    }
    else
    {
        gotoRowCol(16, (dRIP col * cL) + rightspace + 10);
        std::cout << red << "Not Active";
    }
}
void printBox(char& row, char& col, char& cellL, char& cellW, Board**& box)
{
    char r[s], c[s], i[s];

    for (dRIP i = 0; dRIP i < dRIP cellW; (dRIP i)++)
    {
        char j[s], sp[s];

        for (dRIP j = 0; (dRIP j) < (dRIP cellL); (dRIP j)++)
        {
            gotoRowCol((dRIP i + dRIP row * dRIP cellW) + topspace, rightspace + (dRIP j + dRIP col * dRIP cellL));
            if ((dRIP j == 0 || (dRIP j) == (dRIP cellL - 1)) || (dRIP i) == (dRIP cellW - 1))
                std::cout << red << char(-37);
            else if ((*(((char*)&*(*(box + dRIP row) + dRIP col)) + 5)) == '1')
                std::cout << green << char(-37);
            else if ((*(((char*)&*(*(box + dRIP row) + dRIP col)) + 0)) == '1')
            {
                if (dRIP(*(((char*)&*(*(box + dRIP row) + dRIP col)) + 1)) == 0)
                    std::cout << dullwhite << blink << dRIP(*(*(box + dRIP row) + dRIP col)).val << reset;
                else if (dRIP(*(((char*)&*(*(box + dRIP row) + dRIP col)) + 1)) != -1)
                    std::cout
                    << white << dRIP(*(((char*)&*(*(box + dRIP row) + dRIP col)) + 1)) << reset;
                else
                    std::cout << white << char(bomb);
            }
            else
                std::cout << white << char(-37);
        }
        std::cout << std::endl;
    }
}
void printBoard(char& row, char& col, char& cellL, char& cellW, Board**& box, char& flag, char* id)
{
    system("cls");
    printMsgWithID(id);
    printFlag(flag, col, cellL);

    char t[s];
    for (dRIP t = 0; dRIP t < (dRIP cellL * dRIP col); (dRIP t)++)
    {
        gotoRowCol(topspace - 1, dRIP t + rightspace);
        std::cout << red << char(-37);
    }
    char i[s];
    for (dRIP i = 0; dRIP i < dRIP row; (dRIP i)++)
    {
        char j[s];
        for (dRIP j = 0; dRIP j < dRIP col; (dRIP j)++)
            printBox(*i, *j, cellL, cellW, box);
    }
}
void placeBombs(char& row, char& col, Board**& box, char& lvl)
{
    char i[s], num[s];
    srand(time(0));
    if (lvl == 'e')
        dRIP num = 3;
    else if (lvl == 'm')
        dRIP num = 7;
    else
        dRIP num = 11;
    for (dRIP i = 0; dRIP i < dRIP num; (dRIP i)++)
    {
        char r[s], c[s];
        dRIP r = rand() % 10;
        dRIP c = rand() % 10;
        if (dRIP r < row && dRIP c < col)
        {
            if (dRIP(*(((char*)&*(*(box + dRIP r) + dRIP c)) + 1)) != -1)
                dRIP(*(((char*)&*(*(box + dRIP r) + dRIP c)) + 1)) = -1;

            else
                (dRIP i)--;
        }
        else
            (dRIP i)--;
    }
}

char checkSurrFlags(char& ri, char& ci, const char& row, const char& col, Board**& box)
{
    if (dRIP(*(((char*)&(*(*(box + dRIP ri) + dRIP ci)) + 1))) == 0)
        return '0';
    char i[s], j[s], flags[s];
    dRIP flags = 0;
    for (dRIP i = -1; dRIP i < 2; (dRIP i)++)
    {
        for (dRIP j = -1; dRIP j < 2; (dRIP j)++)
        {
            if (dRIP ri + dRIP i >= 0 && dRIP ci + dRIP j >= 0 && dRIP ri + dRIP i < row && dRIP ci + dRIP j < col)
            {
                if ((*(((char*)&*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 5)) == '1')
                    (dRIP flags)++;
            }
        }
    }
    if (dRIP flags == dRIP(*(((char*)&(*(*(box + dRIP ri) + dRIP ci)) + 1))))
        return '1';

    else
        return '0';
}
void checkBoundries(char& row, char& col, char& i, char& j, Board**& box)
{
    if (dRIP(*(((char*)&*(*(box + dRIP i) + dRIP j)) + 1)) != -1)
    {
        char k[s], l[s];
        dRIP(*(((char*)&*(*(box + dRIP i) + dRIP j)) + 1)) = 0;
        for (dRIP l = -1; dRIP l < 2; (dRIP l)++)
        {
            for (dRIP k = -1; dRIP k < 2; (dRIP k)++)
            {
                if (dRIP i + dRIP l >= 0 && dRIP j + dRIP k >= 0 && dRIP i + dRIP l < row && dRIP j + dRIP k < col)
                {
                    if (dRIP(*(((char*)&*(*(box + dRIP i + dRIP l) + dRIP j + dRIP k)) + 1)) == -1)
                        (dRIP(*(((char*)&*(*(box + dRIP i) + dRIP j)) + 1)))++;
                }
            }
        }
    }
}
void assignVal(char& row, char& col, Board**& box)
{
    char i[s], j[s];
    for (dRIP i = 0; dRIP i < dRIP row; (dRIP i)++)
    {
        for (dRIP j = 0; dRIP j < dRIP col; (dRIP j)++)
        {
            checkBoundries(row, col, *i, *j, box);
        }
    }
}
void checkFlag(char& ri, char& ci, char& col, char& cL, char& flag)
{

    if ((dRIP ri >= 10 && dRIP ri <= 15) && (dRIP ci >= (dRIP col * cL) + rightspace + 10 && dRIP ci <= (dRIP col * cL) + rightspace + 36))
    {
        if (dRIP flag == 1)
            dRIP flag = 0;
        else
            dRIP flag = 1;
    }
}
void openBox(char& ri, char& ci, char& flag, Board** box)
{
    if (dRIP flag != 1 && (*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 0)) != '1')
        (*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 0)) = '1';
    else if (dRIP flag == 1 && (*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 5)) == '1')
        (*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 5)) = '0';
    else if ((*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 0)) != '1')
        (*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 5)) = '1';
}
void input(char& ri, char& ci, char& cL, char& cW, char& col, Board**& box, char& flag)
{
    getRowColbyLeftClick(dRIP ri, dRIP ci);
    checkFlag(ri, ci, col, cL, flag);
    dRIP ri = (dRIP ri - topspace) / dRIP cW;
    dRIP ci = (dRIP ci - rightspace) / dRIP cL;
}
char validInput(char& ri, char& ci, const char& row, const char& col, Board**& box, char& flag)
{
    if ((*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 0)) == '1')
    {
        if (checkSurrFlags(ri, ci, row, col, box) != '1')
            return '0';
    }
    if (dRIP ri >= dRIP row || dRIP ri < 0 || dRIP ci >= dRIP col || dRIP ci < 0)
        return '0';
    if ((*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 5)) == '1' && dRIP flag == 0)
        return '0';
    if ((*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 0)) == '1')
        return 'b';
    if (dRIP(*(((char*)&*(*(box + dRIP ri) + dRIP ci)) + 1)) == -1 && dRIP flag != 1)
        return 'x';
    return '1';
}
void floodFill(char& row, char& col, char& ri, char& ci, Board**& box)
{
    char i[s], j[s], k[s], size[s];
    dRIP size = 1;
    FF* flood = new FF[dRIP size];
    dRIP(*((char*)&(*(flood + 0)) + 0)) = dRIP ri;
    dRIP(*((char*)&(*(flood + 0)) + 4)) = dRIP ci;
    while (dRIP size > 0)
    {

        if (dRIP(*(((char*)&*(*(box + dRIP(*((char*)&*(flood + 0) + 0))) + dRIP(*((char*)&*(flood + 0) + 4)))) + 1)) == 0)
            for (dRIP i = -1; dRIP i < 2; (dRIP i)++)
            {
                for (dRIP j = -1; dRIP j < 2; (dRIP j)++)
                {
                    if ((dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i >= 0 && dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i < row) && (dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j >= 0 && dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j < col))
                    {
                        if (dRIP(*(((char*)&*(*(box + dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i) + dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j)) + 1)) != -1 && (*(((char*)&*(*(box + dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i) + dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j)) + 4)) != '1' && (*(((char*)&*(*(box + dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i) + dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j)) + 0)) != '1')
                        {
                            (*(((char*)&*(*(box + dRIP(*((char*)&*(flood + 0) + 0)) + dRIP i) + dRIP(*((char*)&*(flood + 0) + 4)) + dRIP j)) + 0)) = '1';
                            (dRIP size)++;
                            FF* floodex = new FF[dRIP size];
                            for (dRIP k = 0; dRIP k < dRIP size - 1; (dRIP k)++)
                                *(floodex + dRIP k) = *(flood + dRIP k);
                            dRIP(*((char*)&*(floodex + dRIP size - 1) + 0)) = dRIP(dRIP(*((char*)&*(flood + 0) + 0))) + dRIP i;
                            dRIP(*((char*)&*(floodex + dRIP size - 1) + 4)) = dRIP(dRIP(*((char*)&*(flood + 0) + 4))) + dRIP j;
                            delete[] flood;
                            flood = floodex;
                        }
                    }
                }
            }

        (dRIP size)--;
        FF* floodsh = new FF[dRIP size];
        for (dRIP k = 0; dRIP k < dRIP size; (dRIP k)++)
            *(floodsh + dRIP k) = *(flood + dRIP k + 1);
        delete[] flood;
        flood = floodsh;
    }
    delete[] flood;
}
void openMines(char& row, char& col, Board**& box)
{
    char i[s], j[s];
    for (dRIP i = 0; dRIP i < dRIP row; (dRIP i)++)
    {
        for (dRIP j = 0; dRIP j < dRIP col; (dRIP j)++)
        {
            if (dRIP(*(((char*)&*(*(box + dRIP i) + dRIP j)) + 1)) == -1)
                *((char*)&(*(*(box + dRIP i) + dRIP j)) + 0) = '1';
        }
    }
}
char checkWin(char& row, char& col, Board**& box)
{
    char i[s], j[s];
    for (dRIP i = 0; dRIP i < dRIP row; (dRIP i)++)
    {
        for (dRIP j = 0; dRIP j < dRIP col; (dRIP j)++)
        {
            if (*((char*)&(*(*(box + dRIP i) + dRIP j)) + 0) != '1' && dRIP(*((char*)&(*(*(box + dRIP i) + dRIP j)) + 1)) != -1)
                return '0';
        }
    }
    return '1';
}
char selectLvl(char* id)
{
    system("cls");
    printMsgWithID(id);

    std::cout << "\n\n" green << R"(
                                                         ______     ______     ______     __  __ 
                                                        /\  ___\   /\  __ \   /\  ___\   /\ \_\ \
                                                        \ \  __\   \ \  __ \  \ \___  \  \ \____ \
                                                         \ \_____\  \ \_\ \_\  \/\_____\  \/\_____\
                                                          \/_____/   \/_/\/_/   \/_____/   \/_____/
                                          
)" << std::endl;

    std::cout << lightblue << R"(
                                                         __    __     ______     _____     __
                                                        /\ "-./  \   /\  ___\   /\  __-.  /\ \
                                                        \ \ \-./\ \  \ \  __\   \ \ \/\ \ \ \ \
                                                         \ \_\ \ \_\  \ \_____\  \ \____-  \ \_\
                                                          \/_/  \/_/   \/_____/   \/____/   \/_/
                                         
)" << std::endl;
    std::cout << red << R"(
                                                         __  __     ______     ______     _____    
                                                        /\ \_\ \   /\  __ \   /\  == \   /\  __-.  
                                                        \ \  __ \  \ \  __ \  \ \  __<   \ \ \/\ \
                                                         \ \_\ \_\  \ \_\ \_\  \ \_\ \_\  \ \____- 
                                                          \/_/\/_/   \/_/\/_/   \/_/ /_/   \/____/
                                           
)" << std::endl;
    gotoRowCol(15, 0);
    std::cout << white << R"(
        ____ ____ _    ____ ____ ___ 
        [__  |___ |    |___ |     |  
        ___] |___ |___ |___ |___  |  
                             
        _    ____ _  _ ____ _        
        |    |___ |  | |___ |        
        |___ |___  \/  |___ |___     
                             
)";
    char ri[s], ci[s];
    while (true)
    {
        getRowColbyLeftClick(dRIP ri, dRIP ci);
        if (dRIP ri >= 10 && dRIP ri <= 15 && dRIP ci >= 56 && dRIP ci <= 98)
            return 'e';
        else if (dRIP ri >= 18 && dRIP ri <= 22 && dRIP ci >= 56 && dRIP ci <= 98)
            return 'm';
        else if (dRIP ri >= 26 && dRIP ri <= 30 && dRIP ci >= 56 && dRIP ci <= 98)
            return 'h';
    }
}

void printLogin(char& val)
{
    system("cls");
    printMsg();
    std::cout << R"(
                                                                                                                 
             
              _____                ______    ____              _____           _____         ______  _______        ______
             |\    \   _____   ___|\     \  |    |         ___|\    \     ____|\    \       |      \/       \   ___|\     \
             | |    | /    /| |     \     \ |    |        /    /\    \   /     /\    \     /          /\     \ |     \     \
             \/     / |    || |     ,_____/||    |       |    |  |    | /     /  \    \   /     /\   / /\     ||     ,_____/|
             /     /_  \   \/ |     \--'\_|/|    |  ____ |    |  |____||     |    |    | /     /\ \_/ / /    /||     \--'\_|/
            |     // \  \   \ |     /___/|  |    | |    ||    |   ____ |     |    |    ||     |  \|_|/ /    / ||     /___/| 
            |    |/   \ |    ||     \____|\ |    | |    ||    |  |    ||\     \  /    /||     |       |    |  ||     \____|\
            |\ ___/\   \|   /||____ '     /||____|/____/||\ ___\/    /|| \_____\/____/ ||\____\       |____|  /|____ '     /|
            | |   | \______/ ||    /_____/ ||    |     ||| |   /____/ | \ |    ||    | /| |    |      |    | / |    /_____/ |
             \|___|/\ |    | ||____|     | /|____|_____|/ \|___|    | /  \|____||____|/  \|____|      |____|/  |____|     | /
                \(   \|____|/   \( |_____|/   \(    )/      \( |____|/      \(    )/        \(          )/       \( |_____|/
                 '      )/       '    )/       '    '        '   )/          '    '          '          '         '    )/
                        '             '                          '                                                     '
)" << std::endl;
    if (val == '0')
    {
        gotoRowCol(26, 55);
        std::cout << "         " << bg << blink << "              " << resetbg << std::endl;
        gotoRowCol(27, 55);
        std::cout << "---------" << bg << white << "    LOG IN    " << resetbg << "---------" << std::endl;
        gotoRowCol(28, 55);
        std::cout << "         " << bg << "              " << reset << std::endl;
        gotoRowCol(31, 55);
        std::cout << "         " << bgblue << blink << "              " << resetbg << std::endl;
        gotoRowCol(32, 55);
        std::cout << "---------" << bgblue << white << "   SIGN UP?   " << resetbg << "---------" << std::endl;
        gotoRowCol(33, 55);
        std::cout << "         " << bgblue << "              " << reset << std::endl;
        val = '1';
    }
    if (val == '2')
    {
        gotoRowCol(35, 0);
        std::cout << "Error ID Not Found" << std::endl;
    }
    if (val == '3')
    {
        gotoRowCol(35, 0);
        std::cout << "Already Taken" << std::endl;
    }
}
void inputLog(char& val, char* id)
{
    char i[s], oldid[idsize] = {};
    for (dRIP i = 0; dRIP i < idsize; (dRIP i)++)
        *(id + dRIP i) = '\0';
    printLogin(val);
    gotoRowCol(27, 57);
    std::cout << red << "Enter Old ID -> " << reset;
    std::cin >> oldid;
    for (dRIP i = 0; oldid[dRIP i] != '\0'; (dRIP i)++)
        *(id + dRIP i) = *(oldid + dRIP i);
    if (val == '1')
        val = '2';
}

void inputSign(char& val, char* id)
{
    char i[s];
    gotoRowCol(32, 54);
    std::cout << lightblue << "Enter New ID (Max 10) -> " << reset;
    std::cin >> id;
}
void checkLogAndSign(char& sel)
{
    char r[s], c[s];
    getRowColbyLeftClick(dRIP r, dRIP c);
    while (!(dRIP r >= 26 && dRIP r <= 33 && dRIP c >= 63 && dRIP c <= 77))
        getRowColbyLeftClick(dRIP r, dRIP c);

    if (dRIP r >= 26 && dRIP r <= 28 && dRIP c >= 63 && dRIP c <= 77)
        sel = 'l';

    else if (dRIP r >= 31 && dRIP r <= 33 && dRIP c >= 63 && dRIP c <= 77)
        sel = 's';
}
void writeOut(Acc& profile)
{
    std::ofstream sign;
    sign.open("data.bin", std::ios::binary | std::ios::app);
    if (sign)
        sign.write((char*)&profile, sizeof(Acc));
    sign.close();
}
char compareIDs(const char* id1, const char* id2)
{
    char i[s];
    for (dRIP i = 0; dRIP i < idsize; (dRIP i)++)
    {
        if (*(id1 + dRIP i) != *(id2 + dRIP i))
            return '0';
        if (*(id1 + dRIP i) == '\0')
            break;
    }
    return '1';
}
char checkSign(char* id)
{

    char i[s];
    dRIP i = 0;
    Acc check;
    std::ifstream reader("data.bin", std::ios::binary);
    while (reader.read((char*)&check, sizeof(Acc)))
    {
        if (compareIDs((((char*)&check) + 0), id) == '1')
        {
            std::cout << "Already Taken" << std::endl;
            return '0';
        }
        (dRIP i)++;
    }
    return '1';
}
char logIn(const char* id, Acc& profile)
{
    std::ifstream log("data.bin", std::ios::binary);
    while (log.read((char*)&profile, sizeof(Acc)))
    {
        if (compareIDs((((char*)&profile) + 0), id) == '1')
            return '1';
    }
    std::cout << "Error ID Not Found" << std::endl;
    return '0';
}
void signUp(const char* id, Acc& profile)
{
    char i[s];
    dRIP(*(((char*)&profile) + 10)) = 0,
        dRIP(*(((char*)&profile) + 14)) = 0, dRIP(*(((char*)&profile) + 18));
    for (dRIP i = 0; *(id + dRIP i) != '\0'; (dRIP i)++)
        *((((char*)&profile) + 0) + dRIP i) = *(id + dRIP i);
    writeOut(profile);
}
void changeData(Acc& profile)
{
    std::fstream readchange("data.bin", std::ios::binary | std::ios::in | std::ios::out);

    if (!readchange)
        std::cout << "Error opening file." << std::endl;

    Acc newdata = {};
    char position[s];
    dRIP position = 0;
    while (readchange.read((char*)&newdata, sizeof(Acc)))
    {
        if (compareIDs((((char*)&profile) + 0), (((char*)&newdata) + 0)) == '1')
            break;
    }
    dRIP position = readchange.tellg();
    dRIP position -= sizeof(Acc);

    readchange.seekp(dRIP position);
    readchange.write((char*)&profile, sizeof(Acc));
    readchange.close();
}
void displayData(const Acc& profile)
{
    system("cls");
    gotoRowCol(5, 0);
    std::cout << green << blink << R"(   __     __                 _____ _        _       
					 \ \   / /                / ____| |      | |      
 					  \ \_/ /__  _   _ _ __  | (___ | |_ __ _| |_ ___ 
  					   \   / _ \| | | | '__|  \___ \| __/ _` | __/ __|
   					    | | (_) | |_| | |     ____) | || (_| | |_\__ \
   					    |_|\___/ \__,_|_|    |_____/ \__\__,_|\__|___/
                                                  
                                                  

)" << reset << white
<< std::endl;
    gotoRowCol(17, 58);
    std::cout << white << "Your ID     : ";
    char i[s];
    dRIP i = 0;
    while (true)
    {
        if (*(((char*)&(profile)+0) + dRIP i) == '\0')
            break;
        std::cout << *(((char*)&(profile)+0) + dRIP i);
        (dRIP i)++;
    }

    gotoRowCol(22, 58);
    if (dRIP(*(((char*)&(profile)+10))) != 0)
        std::cout << white << "Best Time   : " << dRIP(*(((char*)&(profile)+10))) << "s" << reset << std::endl;
    else
        std::cout << white << "Best Time   : None" << reset << std::endl;
    gotoRowCol(27, 58);
    std::cout << white << "Total Games : " << dRIP(*(((char*)&(profile)+14))) << reset << std::endl;

    gotoRowCol(32, 58);
    if (dRIP(*(((char*)&(profile)+18))) != 0)
        std::cout << white << "Total Wins  : " << dRIP(*(((char*)&(profile)+18))) << reset << std::endl;
    else
        std::cout << white << "Total Wins  : None :p" << reset << std::endl;
}
char openBoundries(char& ri, char& ci, char& row, char& col, Board**& box)
{
    char i[s], j[s], result = '0';
    for (dRIP i = -1; dRIP i < 2; (dRIP i)++)
    {
        for (dRIP j = -1; dRIP j < 2; (dRIP j)++)
        {
            if (dRIP ri + dRIP i >= 0 && dRIP ri + dRIP i < dRIP row && dRIP ci + dRIP j >= 0 && dRIP ci + dRIP j < dRIP col)
            {
                if (dRIP(*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 1)) == -1 && (*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 5)) != '1')
                    result = 'x';

                else if (dRIP(*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 1)) == 0 && (*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 0)) != '1' && (*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 5)) != '1')
                {
                    (*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 0)) = '1';
                    char currRow[s], currCol[s];
                    dRIP currRow = dRIP ri + dRIP i;
                    dRIP currCol = dRIP ci + dRIP j;
                    floodFill(row, col, *currRow, *currCol, box);
                }
                else if ((*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 5)) != '1')
                    (*((char*)&(*(*(box + dRIP ri + dRIP i) + dRIP ci + dRIP j)) + 0)) = '1';
            }
        }
    }
    return result;
}

void printLeaderBoard(Acc*& leadp, const char& size, Acc& you)
{
    system("cls");
    std::cout << red << blink << R"(
         __         ______     ______     _____     ______     ______        ______     ______     ______     ______     _____
        /\ \       /\  ___\   /\  __ \   /\  __-.  /\  ___\   /\  == \      /\  == \   /\  __ \   /\  __ \   /\  == \   /\  __-.
        \ \ \____  \ \  __\   \ \  __ \  \ \ \/\ \ \ \  __\   \ \  __<      \ \  __<   \ \ \/\ \  \ \  __ \  \ \  __<   \ \ \/\ \
         \ \_____\  \ \_____\  \ \_\ \_\  \ \____-  \ \_____\  \ \_\ \_\     \ \_____\  \ \_____\  \ \_\ \_\  \ \_\ \_\  \ \____-
          \/_____/   \/_____/   \/_/\/_/   \/____/   \/_____/   \/_/ /_/      \/_____/   \/_____/   \/_/\/_/   \/_/ /_/   \/____/
                                                                                                                       
)" << reset << std::endl;
    char i[s];
    gotoRowCol(8, 5);
    std::cout << green << "|" << reset;
    for (dRIP i = 0; dRIP i < 130; (dRIP i)++)
        std::cout << green << "-" << reset;
    std::cout << green << "|" << reset;
    gotoRowCol(8, 68);
    std::cout << green << "|" << reset;
    gotoRowCol(9, 25);
    std::cout << "IDs";
    gotoRowCol(9, 68);
    std::cout << green << "|" << reset;
    gotoRowCol(9, 5);
    std::cout << green << "|" << reset;
    gotoRowCol(9, 136);
    std::cout << green << "|" << reset;
    gotoRowCol(9, 105);
    std::cout << "Times";
    gotoRowCol(10, 5);
    std::cout << green << "|" << reset;
    for (dRIP i = 0; dRIP i < 130; (dRIP i)++)
        std::cout << green << "-" << reset;
    std::cout << green << "|" << reset;
    gotoRowCol(10, 68);
    std::cout << green << "|" << reset;
    for (dRIP i = 0; dRIP i < dRIP size; (dRIP i)++)
    {
        char j[s];
        dRIP j = 0;
        gotoRowCol(11 + dRIP i, 25);
        if (compareIDs((((char*)&you) + 0), ((((char*)&(*(leadp + dRIP i)) + 0)))) == '1')
        {
            while (true)
            {
                if (*((((char*)&(*(leadp + dRIP i)) + 0)) + dRIP j) == '\0')
                    break;
                std::cout << *((((char*)&(*(leadp + dRIP i)) + 0)) + dRIP j);
                (dRIP j)++;
            }
            std::cout << " (you)";
        }
        else
        {
            while (true)
            {
                if (*((((char*)&(*(leadp + dRIP i)) + 0)) + dRIP j) == '\0')
                    break;
                std::cout << *((((char*)&(*(leadp + dRIP i)) + 0)) + dRIP j);
                (dRIP j)++;
            }
        }
        gotoRowCol(11 + dRIP i, 68);
        std::cout << green << "|" << reset;
        gotoRowCol(11 + dRIP i, 5);
        std::cout << green << "|" << reset;
        gotoRowCol(11 + dRIP i, 136);
        std::cout << green << "|" << reset;
        gotoRowCol(11 + dRIP i, 105);
        if (dRIP(*((char*)&(*(leadp + dRIP i)) + 10)) != 0)
            std::cout << dRIP(*((char*)&(*(leadp + dRIP i)) + 10)) << "s";
        else
            std::cout << "None";
    }
    std::cout << std::endl;
    std::cout << green << "     |" << reset;
    for (dRIP i = 0; dRIP i < 130; (dRIP i)++)
    {
        if (dRIP i == 62)
            std::cout << green << "|" << reset;

        else
            std::cout << green << "-" << reset;
    }
    std::cout << green << "|" << reset;
}
void swapLead(Acc& lead1, Acc& lead2)
{
    Acc lead3 = lead1;
    lead1 = lead2;
    lead2 = lead3;
}
void sortLeader(Acc*& leadp, const char& size)
{
    char i[s], j[s];
    for (dRIP i = 0; dRIP i < dRIP size - 1; (dRIP i)++)
    {
        for (dRIP j = 0; dRIP j + 1 < dRIP size - dRIP i; (dRIP j)++)
        {
            if ((dRIP(*((char*)&(*(leadp + dRIP j)) + 10)) > (*((char*)&(*(leadp + dRIP j + 1)) + 10)) && dRIP(*((char*)&(*(leadp + dRIP j + 1)) + 10)) != 0) || dRIP(*((char*)&(*(leadp + dRIP j)) + 10)) == 0)
                swapLead(*(leadp + dRIP j), *(leadp + dRIP j + 1));
        }
    }
}
void leaderBoard(Acc*& leadp, char& size, Acc& you)
{
    char i[s];
    dRIP size = 0;
    std::ifstream templead("data.bin", std::ios::binary);
    Acc temp;
    while (templead.read((char*)&temp, sizeof(Acc)))
        (dRIP size)++;

    std::ifstream leader("data.bin", std::ios::binary);
    leadp = new Acc[dRIP size];
    dRIP i = 0;
    while (leader.read((char*)&*(leadp + dRIP i), sizeof(Acc)))
        (dRIP i)++;
    sortLeader(leadp, size);
    printLeaderBoard(leadp, size, you);
}
int main()
{
    char selection, val = '0', id[idsize] = {}, leadersize[s];
    Acc profile = {}, * leader = nullptr;

    printLogin(val);
    checkLogAndSign(selection);
    if (selection == 's')
    {
        printLogin(val);
        while (true)
        {
            inputSign(val, id);
            if (checkSign(id) == '1')
                break;
            char taken = '3';
            printLogin(taken);
        }
        signUp(id, profile);
    }
    else
    {
        while (true)
        {
            inputLog(val, id);
            if (logIn(id, profile) == '1')
                break;
        }
    }
    char playagain = 'y', runner = '1';
    while (playagain == 'y')
    {
        char* IDp = (((char*)&profile) + 0);
        (dRIP(*(((char*)&profile) + 14)))++;
        Board** box;
        char lvl;
        lvl = selectLvl(IDp);
        char r[s], c[s], cL[s], cW[s], ri[s], ci[s], flag[s], startingtime[s], lost = '0';
        dRIP cL = 7, dRIP cW = 4, dRIP flag = 0;
        if (lvl == 'e')
            dRIP r = 5, dRIP c = 5;
        else if (lvl == 'm')
            dRIP r = 7, dRIP c = 7;
        else
            dRIP r = 9, dRIP c = 9;
        makeBoard(*r, *c, box);
        placeBombs(*r, *c, box, lvl);
        assignVal(*r, *c, box);
        printBoard(*r, *c, *cL, *cW, box, *flag, IDp);
        dRIP startingtime = time(0);
        while (true)
        {
            input(*ri, *ci, *cL, *cW, *c, box, *flag);

            if (validInput(*ri, *ci, *r, *c, box, *flag) == 'x' || lost == '1')
            {
                if (lost != '1')
                    openBox(*ri, *ci, *flag, box);
                openMines(*r, *c, box);
                printBoard(*r, *c, *cL, *cW, box, *flag, IDp);
                printLose();
                std::cout << " t = " << time(0) - dRIP startingtime << "s" << std::endl;
                std::cout << "Play Again? (y/n)";
                std::cin >> playagain;
                if (playagain != 'y')
                    runner = '0';
                break;
            }
            else if (validInput(*ri, *ci, *r, *c, box, *flag) == 'b')
            {
                if (openBoundries(*ri, *ci, *r, *c, box) == 'x')
                    lost = '1';
                else
                    printBoard(*r, *c, *cL, *cW, box, *flag, IDp);
            }
            else if (validInput(*ri, *ci, *r, *c, box, *flag) == '1')
            {
                openBox(*ri, *ci, *flag, box);
                if (dRIP(*((char*)&(*(*(box + dRIP ri) + dRIP ci)) + 1)) == 0 && (*((char*)&(*(*(box + dRIP ri) + dRIP ci)) + 5)) != '1' && dRIP flag == 0)
                    floodFill(*r, *c, *ri, *ci, box);
                printBoard(*r, *c, *cL, *cW, box, *flag, IDp);
            }
            printFlag(*flag, *c, *cL);
            if (checkWin(*r, *c, box) == '1')
            {
                char currenttime[s];
                dRIP currenttime = time(0);
                (dRIP(*(((char*)&profile) + 18)))++;
                printWin();
                if (dRIP(*(((char*)&profile) + 18)) == 1)
                    dRIP(*(((char*)&profile) + 10)) = dRIP currenttime - dRIP startingtime;
                else if (dRIP currenttime - dRIP startingtime > dRIP(*(((char*)&profile) + 10)))
                    dRIP(*(((char*)&profile) + 10)) = dRIP currenttime - dRIP startingtime;
                std::cout << " t = " << dRIP currenttime - dRIP startingtime << "s" << std::endl;
                std::cout << "Play Again? (y/n)";
                std::cin >> playagain;
                if (playagain != 'y')
                    runner = '0';
                break;
            }
        }

        delete[] box;
        if (runner == '0')
            break;
    }
    changeData(profile);
    displayData(profile);
    char pause;
    std::cout << "Pause before LeaderBoard" << std::endl;
    system("pause");
    leaderBoard(leader, *leadersize, profile);

    return 0;
}
