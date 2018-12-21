#include <iostream>
#include <string>
#include <conio.h>
#include "Processor.h"
#include "Debugger.h"
#include "src/Loader.cpp"
#include "src/Converter.cpp"

using namespace std;

void inform()
{
    cout << "\nLaboratory work number 1 on discipline <<System software>>"
         << "\nPerformed by a student of group DIPRb-31 Bondarenko Elizaveta";
}

int main()
{
    Processor processor;
    string fileText, fileBin, fileprotocol;
    inform();
    cout << "\n\nEnter the name of the executable file: ";
    cin >> fileText;
    fileBin=fileText+".bin"; // бинарный файл для загрузки в процессор
    fileprotocol=fileText+".proto";
    try
    {
        convertToBinary(fileText, fileBin);
        loader(fileBin, processor);
        Debugger debugger(processor, fileprotocol);
        debugger.Run();
    }
    catch (Error e)
    {
         cerr << "\nError! ";
         cout << "\n\nTo finish press any key...";
         getch();
    }
    catch (invalid_argument e)
    {
        cerr << "\nError argument!";
        cout << "\n\nTo finish press any key...";
        getch();
    }
}
