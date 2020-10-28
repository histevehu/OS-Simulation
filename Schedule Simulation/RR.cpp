#include <iostream>
#include <vector>
#define RR 3
#define OUTPUT_TABLE_WIDTH 15
using namespace std;

struct pcb
{
	//属性值
	string id;		 //ID
	int Arrive_time; //到达时间
	int Cost_time;	 //任务需要的总运行时间 =Running_time+Rest_time
	int Total_time;	 //总耗时 =总运行时间（Cost_time）+ 就绪队列总排队时间

	//动态记录值
	int Running_time; //已运行时间
	int Rest_time;	  //还需要的运行时间 =0结束
	int State;		  // 状态		0为就绪  1为完成
};
vector<pcb> Ready_queue; //就绪队列
vector<pcb>::iterator rq;
vector<pcb> Finish_queue; //结束队列
int n;					  //进程数目
//就绪队列就是执行的含义
void process_info_input()
{
	cout << "The number of processes：";
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
		process.Total_time = 0;
		process.State = 0;
		Ready_queue.push_back(process);
	}
}

//对就绪队列的PCB根据到达时间进行升序排列
void sort()
{
	if (Ready_queue.empty())
		return;
	else
		for (int i = 0; i < Ready_queue.size() - 1; i++) //就绪队列排序
			for (int j = i + 1; j < Ready_queue.size(); j++)
				if (Ready_queue[i].Arrive_time > Ready_queue[j].Arrive_time)
				{
					swap(Ready_queue[i], Ready_queue[j]);
				}
}

//输出进程队列信息
void process_info_print()
{

	cout.width(OUTPUT_TABLE_WIDTH);
	cout << "ID";
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
	cout << "State" << endl;

	if (!Ready_queue.empty()) //就绪队列输出
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
			cout << (*fq).Total_time;
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
	//如当前节点pcb已到达则执行
	if ((*rq).Arrive_time <= order)
	{
		//运行就绪队列
		(*rq).Running_time++;
		(*rq).Rest_time--;
		if ((*rq).Rest_time <= 0)
		{
			(*rq).Total_time = order + 1 - (*rq).Arrive_time;
			(*rq).State = 1;
			Finish_queue.push_back(*rq);
			rq = Ready_queue.erase(rq);
			if (rq == Ready_queue.end() || (*rq).Arrive_time > order + 1)
				rq = Ready_queue.begin();
		}
		else
		{
			if ((*rq).Running_time % RR == 0) //当前任务时间片用尽，切换为下一任务
			{
				if (rq == Ready_queue.end() - 1) //若当前任务为队尾，则下一任务队首重新开始
					rq = Ready_queue.begin();
				//否则判断下一任务是否将到达，若否则仍从队首重新开始
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
	return 0;
}
