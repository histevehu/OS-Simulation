//RR schedule
#include <iostream>
#include <vector>
#include "header/PCB"
#include "header/ProcessAnalyze"
#define RR 3 //The length of the running time period allocate to the process each time (unit: time slice)
#define OUTPUT_TABLE_WIDTH 15
using namespace std;

vector<pcb> Ready_queue;
vector<pcb>::iterator rq; //ready queue iterator
vector<pcb> Finish_queue;
int n; //Number of processes

void process_info_input()
{
	cout << "The number of processes:";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		pcb process;
		cout << "================================================================================ Process " << i + 1 << " ================================================================================" << endl;
		cout << "id:";
		cin >> process.id;
		cout << "Arrive time:";
		cin >> process.Arrive_time;
		cout << "Cost time:";
		cin >> process.Cost_time;
		process.Rest_time = process.Cost_time;
		process.Running_time = 0;
		process.TA_time = 0;
		process.WTA_time = 0;
		process.State = 0;
		Ready_queue.push_back(process);
	}
}

//Arrange PCBs in the ready queue in ascending order according to the arrival time
void sort()
{
	if (Ready_queue.empty())
		return;
	else
		for (int i = 0; i < Ready_queue.size() - 1; i++) //Ready queue sorting
			for (int j = i + 1; j < Ready_queue.size(); j++)
				if (Ready_queue[i].Arrive_time > Ready_queue[j].Arrive_time)
				{
					swap(Ready_queue[i], Ready_queue[j]);
				}
}

//Output process queue information
void process_info_print()
{

	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "ID";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "Arrive_time";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "Cost_time";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "TA_time";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "Running_time";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "Rest_time";
	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "State" << endl;

	if (!Ready_queue.empty()) //Ready queue output
	{
		cout << "<Ready Queue>" << endl;
		for (int i = 0; i < Ready_queue.size(); i++)
		{
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].id;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Arrive_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Cost_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Running_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Rest_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].State << endl;
		}
	}

	if (!Finish_queue.empty())
	{
		cout << "<Finish Queue>" << endl;
		for (vector<pcb>::iterator fq = Finish_queue.begin(); fq != Finish_queue.end(); fq++)
		{
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).id;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).Arrive_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).Cost_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).TA_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).Running_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).Rest_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).State << endl;
		}
	}
}

void run(int order)
{
	//run if the current pcb has arrived
	if ((*rq).Arrive_time <= order)
	{
		//Run ready queue
		(*rq).Running_time++;
		(*rq).Rest_time--;
		if ((*rq).Rest_time <= 0)
		{
			(*rq).TA_time = order + 1 - (*rq).Arrive_time;
			(*rq).WTA_time = 1.0 * (*rq).TA_time / (*rq).Cost_time;
			(*rq).State = 1;
			Finish_queue.push_back(*rq);
			rq = Ready_queue.erase(rq);
			if (rq == Ready_queue.end() || (*rq).Arrive_time > order + 1)
				rq = Ready_queue.begin();
		}
		else
		{
			if ((*rq).Running_time % RR == 0) //Current process RR running time is out, switch to the next task
			{
				if (rq == Ready_queue.end() - 1) //If the current process is the end of the queue, the next task will restart at the begin of queue
					rq = Ready_queue.begin();
				//Or judge whether the next process will arrive in next time point, if not, start again from the begin of queue
				else
				{
					rq++;
					if ((*rq).Arrive_time > order + 1)
						rq = Ready_queue.begin();
				}
			}
		}
	}
}
int main()
{
	process_info_input();
	sort();
	rq = Ready_queue.begin();
	int order = 0;
	cout.setf(std::ios::left);
	while (!Ready_queue.empty())
	{
		run(order++);
		cout << endl
			 << endl
			 << "================================================================================ Time Slice: " << (order) << " ================================================================================" << endl;
		process_info_print();
	}
	cout << "Running Finished." << endl
		 << "Process completion sequence:";
	for (vector<pcb>::iterator fq = Finish_queue.begin(); fq != Finish_queue.end(); fq++)
	{
		cout << (*fq).id << ",";
	}
	cout << endl
		 << "Total time slices: " << order << endl;
	analyze(Finish_queue, 3);
	system("pause");
	return 0;
}
