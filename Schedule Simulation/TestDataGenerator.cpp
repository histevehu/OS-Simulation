#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Header/PCB"

using namespace std;

int RandInt(int min = 0, int max = 1)
{
    return (rand() % (max - min + 1)) + min;
}
string getFileName(int type)
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

//Used to output single schedule algorithm test data
void saveFile(int type, int proNum)
{
    ofstream outFile;
    outFile.open(getFileName(type) + ".txt");
    //output processes total number
    outFile << proNum << endl;
    for (int i = 0; i < proNum; i++)
    {
        //output process id
        outFile << i << endl;
        //if MaxPriority,output process random priority
        if (type == 1)
        {
            outFile << RandInt(0, proNum) << endl;
        }
        //output process arrive time
        outFile << RandInt(0, proNum) << endl;
        //output process cost time
        outFile << RandInt(1, 20) << endl;
        //if not RR,output process start block time & block time
        if (type != 4)
        {
            outFile << RandInt(1, 10) << endl;
            outFile << RandInt(1, 10) << endl;
        }
    }
    outFile.close();
}

//Used to output all schedule algorithms test data
void saveFile(int type, vector<pcb> &processes)
{
    ofstream outFile;
    outFile.open(getFileName(type) + ".txt");
    //output processes total number
    int proNum = processes.size();
    outFile << proNum << endl;
    for (int i = 0; i < proNum; i++)
    {
        //output process id
        outFile << processes[i].id << endl;

        //if MaxPriority,output process random priority
        if (type == 1)
        {
            outFile << processes[i].priority << endl;
        }
        //output process arrive time
        outFile << processes[i].Arrive_time << endl;
        //output process cost time
        outFile << processes[i].Cost_time << endl;
        //if not RR,output process start block time & block time
        if (type != 4)
        {
            outFile << processes[i].Start_block << endl;
            outFile << processes[i].Block_time << endl;
        }
    }
    outFile.close();
}

//Uesed to generate a vector of data for outputing all schedule algorithms test data
void randData(vector<pcb> &processes, int proNum)
{
    for (int i = 0; i < proNum; i++)
    {
        pcb pro;
        //output process id
        pro.id = to_string(i);
        //output process random priority
        pro.priority = RandInt(0, proNum);
        //output process arrive time
        pro.Arrive_time = RandInt(0, proNum);
        //output process cost time
        pro.Cost_time = RandInt(1, 20);
        //output process start block time & block time
        pro.Start_block = RandInt(1, 10);
        pro.Block_time = RandInt(1, 10);
        processes.push_back(pro);
    }
}

void generateData(int type, int proNum)
{
    switch (type)
    {
    case 1:
    case 2:
    case 3:
    case 4:
        saveFile(type, proNum);
        break;

    case 0:
        vector<pcb> processes;
        randData(processes, proNum);
        for (int i = 1; i <= 4; i++)
            saveFile(i, processes);
        break;
    }
}

int main()
{
    int t, n; //t:Number of scheduling algorithm    n:The number of processes
    cout << "Schedule algorithm type of random data generated: (0.All   1.MaxPriority   2.FCFS   3.SJF   4.RR)" << endl;
    cin >> t;
    cout << "The number of processes: " << endl;
    cin >> n;
    generateData(t, n);
    system("pause");
    return 0;
}