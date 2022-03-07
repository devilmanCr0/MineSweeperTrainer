// dllmain.cpp : Defines the entry point for the DLL application.
#include "hTools.h"


typedef int(_stdcall* revealBombs)(char a1);
typedef int(_stdcall* GridClick)(int column, int row);
typedef void(_stdcall* CheckIfBomb)(int a1, int a2); 
typedef int(_stdcall* PositionValue)(int column, int row);

GridClick clickOnMap;
revealBombs bombReveal;
CheckIfBomb flagClaim;
PositionValue tileChecker;

uintptr_t moduleBase;

char bombTile(int column, int row)
{
    int number = (int)(tileChecker(column, row) & 0xBF);

    return number;
}

bool ExtractTile(int a, HANDLE& consoleHandle)
{
    int* Rows{ (int*)((uintptr_t)moduleBase + 0x56A8) };
    int* Columns{ (int*)((uintptr_t)moduleBase + 0x56AC) };
    uint8_t* GridArray{ (uint8_t*)((uintptr_t)moduleBase + 0x5361) };
    static int eggnugget{ 0 };
    
    std::cout << std::endl;
    int number{ 0 };
    
    if (a == 1)
    {
        if (eggnugget)
        {
            
            SetConsoleCursorPosition(consoleHandle, { 0, 8 });
            
        }
        eggnugget++;
    }
    

    for (int i{ 0 };i < (int)*Rows; i++)
    {
        for (int j{ 0 }; j < (int)*Columns; j++)
        {
           

            if ((*GridArray & 0xE0) == 0x80)
            {
                if (a == 1)
                {
                    SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
                    std::cout << " X ";
                }
               
            }
            else {
                if (a == 1)
                {
                   
                    int lol = bombTile(j + 1, i + 1);
                    HANDLE hHand{};
                    

                    //fix this
                    switch (lol)
                    {
                    case 0:
                        SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
                        std::cout << " " << '@' << " ";
                        break;
                    case 1:
                        SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
                        std::cout << " " << lol << " ";
                        break;
                    case 2:
                        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
                        std::cout << " " << lol << " ";
                        break;
                    case 3:
                        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
                        std::cout << " " << lol << " ";
                        break;
                    case 4:
                        SetConsoleTextAttribute(consoleHandle, BACKGROUND_BLUE);
                        std::cout << " " << lol << " ";
                        break;
                    case 5:
                        SetConsoleTextAttribute(consoleHandle, BACKGROUND_GREEN);
                        std::cout << " " << lol << " ";
                        break;
                    case 6:
                        SetConsoleTextAttribute(consoleHandle, BACKGROUND_RED);
                        std::cout << " " << lol << " ";
                        break;
                    case 7:
                        SetConsoleTextAttribute(consoleHandle, BACKGROUND_INTENSITY);
                        std::cout << " " << lol << " ";
                        break;
                    }

                }
                else if (a == 2) {
                    clickOnMap(j+1, i+1);
                   
                }
               
            }
 
            

                
                GridArray += 0x1;
        }
        GridArray += ((32 - (*Columns)));
        if (a == 1)
        {
            std::cout << std::endl;
        }
        
    }
    //to create our win
    if (a == 2)
    {
        GridArray = (uint8_t*)((uintptr_t)moduleBase + 0x5361);
        for (int i{ 0 }; i < (int)*Rows; i++)
        {
            for (int j{ 0 }; j < (int)*Columns; j++)
            {
                if ((*GridArray & 0xE0) == 0x80)
                {
                    flagClaim(j + 1, i + 1);
                    return true;
                }
                GridArray += 0x1;
            }
            GridArray += ((32 - (*Columns)));
        }
    }


    
    return true;
}

DWORD WINAPI MineFuck(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    HANDLE consoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) };

    std::cout << "Welcome to the deliciousness " << std::endl;

    bool bVisible{ false };
    bool bFreezeTime{ false };
    bool bMineGod{ false };
    bool bAutoWin{ false };
    bool bActivateOnce{ true };
    bool oneTimeThing{ true };


    moduleBase = (uintptr_t)GetModuleHandle(L"winmine.exe");

    bombReveal = (revealBombs)((BYTE*)moduleBase + 0x2F80);
    clickOnMap = (GridClick)((BYTE*)moduleBase + 0x3008);
    flagClaim = (CheckIfBomb)((BYTE*)moduleBase + 0x374F);
    tileChecker = (PositionValue)((BYTE*)moduleBase + 0x2F3B);


    uintptr_t TimeInit{ (uintptr_t)moduleBase + 0x5164 };

    std::cout << "CONTROLS " << std::endl;

    std::cout << "P FOR LAYOUT EXTRACTION - ONE BUTTON PRESS" << std::endl;
    std::cout << "O FOR MINE VISIBILITY - TOGGLE" << std::endl;
    std::cout << "I FOR TIME FREEZE - TOGGLE" << std::endl;
    std::cout << "L FOR MINE IMMUNITY - TOGGLE" << std::endl;
    std::cout << "K FOR AUTO WIN - TOGGLE" << std::endl;



    while (1)
    {
        if (*(uintptr_t*)((uintptr_t)moduleBase + 0x5164) == 0)
        {
            oneTimeThing = 1;
        }
        
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        
        
        if (GetAsyncKeyState(80))
        {
            if (oneTimeThing)
            {
               
                    std::cout << "ACTIVATED EXTRACTION";
                    ExtractTile(1, consoleHandle);
                    Sleep(1000);
                    oneTimeThing = false;
               
            }
           
        }

        if (GetAsyncKeyState(79) & 1)
        {
            bVisible = !bVisible;
           
        }

        if (GetAsyncKeyState(73) & 1)
        {
            bFreezeTime = !bFreezeTime;
        }

        if (GetAsyncKeyState(76) & 1)
        {
            bMineGod = !bMineGod;
            
        }

        if (GetAsyncKeyState(75))
        {
            
            std::cout << "ACTIVATED AUTOWIN - ENJOY :)";
            ExtractTile(2, consoleHandle);
            Sleep(1000);
        }

     

        if (bVisible)
        {
            if (bombReveal)
            {
                bombReveal(10);
            }
            
            bVisible = false;
        }
        if (bFreezeTime)
        {
            if (bActivateOnce)
            {
                *(uintptr_t*)TimeInit = 0;
                bActivateOnce = false;
            }
        }
        else
        {
            if (!bActivateOnce)
            {
                *(uintptr_t*)TimeInit = 1;
                bActivateOnce = true;
            }
        }

        if (bMineGod)
        {
            memoryPatch(2, (BYTE*)moduleBase + 0x3536);
        }
        else
        {
            memoryAllocate(2, (BYTE*)moduleBase + 0x3536, (BYTE*)"\x75\x50");
        }
        if (bAutoWin)
        {
            
        }
    }


    
    memoryAllocate(2, (BYTE*)moduleBase + 0x3536, (BYTE*)"\x75\x50");
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = nullptr;
        hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MineFuck, hModule, 0, nullptr);
        if (hThread)
        {
            CloseHandle(hThread);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

