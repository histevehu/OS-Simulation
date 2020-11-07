#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

int n, type;
string fileName;

int RandInt(int min = 0, int max = 1)
{
    return (rand() % (max - min + 1)) + min;
}
string getFileName()
{
    switch (type)
    {
    case 1:
        return "MaxPriority";
        break;

    case 2:
        return "FCFS";
        break;

    case 3:
        return "SJF";
        break;

    case 4:
        return "RR";
        break;
    }
}

void saveFile()
{
    ofstream outFile;
    outFile.open(getFileName() + ".txt");
    outFile << n << endl;
    for (int i = 1; i <= n; i++)
    {
        outFile << i << endl;
        if (type == 1)
        {
            outFile << RandInt(0, n) << endl;
        }
        outFile << RandInt(0, n) << endl;
        outFile << RandInt(1, 20) << endl;
        if (type != 4)
        {
            outFile << RandInt(0, 10) << endl;
            outFile << RandInt(0, 10) << endl;
        }
    }
    outFile.close();
}

int main()
{
    cout << "The type of schedule: (1.MaxPriority   2.FCFS   3.SJF   4.RR)" << endl;
    cin >> type;
    cout << "The number of processes: " << endl;
    cin >> n;
    cin.get();
    saveFile();
    return 0;
}