//Dynamic max priority number schedule
#include <iostream>
#include <vector>
#include "header/PCB"
#include "header/ProcessAnalyze"
#define OUTPUT_TABLE_WIDTH 15
using namespace std;

vector<pcb> Waiting_queue;
vector<pcb> Ready_queue;
vector<pcb> Block_queue;
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
		cout << "Priority:";
		cin >> process.priority;
		cout << "Arrive time:";
		cin >> process.Arrive_time;
		cout << "Cost time:";
		cin >> process.Cost_time;
		cout << "Start block:";
		cin >> process.Start_block;
		cout << "Block time:";
		cin >> process.Block_time;
		process.Rest_time = process.Cost_time;
		process.Running_time = 0;
		process.TA_time = 0;
		process.WTA_time = 0;
		process.State = -1;
		process.Start_block_t = process.Start_block;
		process.Block_time_t = process.Block_time;
		Waiting_queue.push_back(process);
	}
}

//Arrange PCBs in the ready queue in descending order according to priority
void sort()
{
	if (Ready_queue.empty() && Block_queue.empty())
		return;
	else
		for (int i = 0; i < Ready_queue.size() - 1; i++) //Ready queue sorting
			for (int j = i + 1; j < Ready_queue.size(); j++)
				if (Ready_queue[i].priority < Ready_queue[j].priority)
				{
					swap(Ready_queue[i], Ready_queue[j]);
				}
}

//Output process queue information
void process_info_print()
{
	if (!Waiting_queue.empty() || !Ready_queue.empty() || !Block_queue.empty() || !Finish_queue.empty())
	{
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "ID";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Priority";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Arrive_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Cost_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Total_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Running_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Rest_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Start_block";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Block_time";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Start_block_t";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "Block_time_t";
		cout.width(OUTPUT_TABLE_WIDTH);
		cout << "State" << endl;
	}
	if (!Waiting_queue.empty())
	{
		cout << "<Waiting Queue>" << endl;
		for (vector<pcb>::iterator wq = Waiting_queue.begin(); wq != Waiting_queue.end(); wq++)
		{
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).id;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).priority;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).Arrive_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).Cost_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).Start_block;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).Block_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*wq).State << endl;
		}
	}
	if (!Ready_queue.empty()) //Ready queue output
	{
		cout << "<Ready Queue>" << endl;
		for (int i = 0; i < Ready_queue.size(); i++)
		{
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].id;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].priority;
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
			cout << Ready_queue[i].Start_block;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Block_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].Start_block_t;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Ready_queue[i].State << endl;
		}
	}
	if (!Block_queue.empty()) //Blocking queue output
	{
		cout << "<Block Queue>" << endl;
		for (int i = 0; i < Block_queue.size(); i++)
		{
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].id;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].priority;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Arrive_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Cost_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Running_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Rest_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Start_block;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Block_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].Block_time_t;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << Block_queue[i].State << endl;
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
			cout << (*fq).priority;
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
			cout << (*fq).Start_block;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).Block_time;
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << "N/A";
			cout.width(OUTPUT_TABLE_WIDTH);
			cout << (*fq).State << endl;
		}
	}
}

void run(int order)
{
	//Scan the waiting queue, load the available pcb into the ready queue
	if (!Waiting_queue.empty())
	{
		vector<pcb>::iterator wq = Waiting_queue.begin();
		while (wq != Waiting_queue.end())
		{
			if ((*wq).Arrive_time == order) //Time to arrive
			{
				(*wq).State = 0;			  //Status changed to ready
				Ready_queue.push_back((*wq)); //Put into the ready queue
				wq = Waiting_queue.erase(wq); //Delete in waiting queue
			}
			else
			{
				wq++;
			}
		}
	}
	//Process the blocking queue first
	if (!Block_queue.empty())
	{
		vector<pcb>::iterator bq = Block_queue.begin();
		while (bq != Block_queue.end())
		{
			(*bq).Block_time_t--;		 //Blocking queue blocking time--
			if ((*bq).Block_time_t == 0) //End blocking
			{
				(*bq).State = 0; //Status changed to ready
				(*bq).Start_block_t = (*bq).Start_block;
				Ready_queue.push_back((*bq)); //Put into ready queue
				bq = Block_queue.erase(bq);	  //Delete in the blocking queue
			}
			else
			{
				bq++;
			}
		}
	}
	//If the ready queue is not empty, run the ready queue
	if (!Ready_queue.empty()) //The ready queue is not empty, operate on the ready queue
	{
		//Sort the ready queue
		sort();
		//Run ready queue
		Ready_queue[0].Rest_time--;
		Ready_queue[0].Running_time++;
		Ready_queue[0].Start_block_t--;
		//if the occupied CPU time of the process has reached the required running time, cancel the process
		if (Ready_queue[0].Rest_time <= 0)
		{
			Ready_queue[0].State = 2; // Change state to finish
			Ready_queue[0].TA_time = order + 1 - Ready_queue[0].Arrive_time;
			Ready_queue[0].WTA_time = 1.0 * Ready_queue[0].TA_time / Ready_queue[0].Cost_time;
			Finish_queue.push_back(Ready_queue[0]); //Push to finish queue
			Ready_queue.erase(Ready_queue.begin()); //Delete the process
		}
		else
		{
			Ready_queue[0].priority -= 3;
			//Is it about to be blocked and should turn to the blocking queue?
			if (Ready_queue[0].Start_block_t <= 0)
			{
				Ready_queue[0].State = 1; //block
				Ready_queue[0].Block_time_t = Ready_queue[0].Block_time;
				Block_queue.push_back(Ready_queue[0]); //Push to block queue
				Ready_queue.erase(Ready_queue.begin());
			}
		}
	}
}
int main()
{
	process_info_input();
	int order = 0;
	cout.setf(std::ios::left);
	while (!Waiting_queue.empty() || !Ready_queue.empty() || !Block_queue.empty())
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
	analyze(Finish_queue, 0);
	system("pause");
	return 0;
}
