#include <iostream>
#include <vector>
#define OUTPUT_TABLE_WIDTH 15
using namespace std;

struct pcb
{
	//属性值
	string id;		 //ID
	int priority;	 //优先权
	int Arrive_time; //到达时间
	int Cost_time;	 //任务需要的总运行时间 =Running_time+Rest_time
	int Total_time;	 //总耗时 =总运行时间（Cost_time）+总阻塞时间+就绪队列总排队时间
	int Start_block; //运行多少个时间片之后开始阻塞
	int Block_time;	 //阻塞的时间

	//动态记录值
	int Running_time;  //已运行时间
	int Rest_time;	   //还需要的运行时间 =0结束
	int Start_block_t; //自上次阻塞结束恢复运行以来已运行时间片长度,用以计算运行多少个时间片之后再次开始阻塞 =0开始阻塞
	int Block_time_t;  //自阻塞开始，已阻塞时间片长度 =0结束阻塞
	int State;		   // 状态   -1为等待	0为就绪  1为阻塞  2为完成
};
vector<pcb> Waiting_queue; //就绪队列
vector<pcb> Ready_queue;   //就绪队列
vector<pcb> Block_queue;   //阻塞队列
vector<pcb> Finish_queue;  //结束队列
int n;					   //进程数目
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
		cout << "Priority:";
		cin >> process.priority;
		cout << "Arrive time:";
		cin >> process.Arrive_time;
		cout << "Cost time:";
		cin >> process.Cost_time;
		/*cout << "Running_time:";
		cin >> process.Running_time;*/
		cout << "Start block:";
		cin >> process.Start_block;
		cout << "Block time:";
		cin >> process.Block_time;
		/*cout << "State:";
		cin >> process.State;*/
		process.Rest_time = process.Cost_time;
		process.Running_time = 0;
		process.Total_time = 0;
		process.State = -1;
		process.Start_block_t = process.Start_block;
		process.Block_time_t = process.Block_time;
		Waiting_queue.push_back(process);
	}
}

//对就绪队列的PCB根据优先级进行降序排列
void sort()
{
	if (Ready_queue.empty() && Block_queue.empty())
		return;
	else
		for (int i = 0; i < Ready_queue.size() - 1; i++) //就绪队列排序
			for (int j = i + 1; j < Ready_queue.size(); j++)
				if (Ready_queue[i].priority < Ready_queue[j].priority)
				{
					swap(Ready_queue[i], Ready_queue[j]);
				}
}

//输出进程队列信息
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
	if (!Ready_queue.empty()) //就绪队列输出
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
	if (!Block_queue.empty()) //阻塞队列输出
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
			cout << (*fq).Total_time;
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

void run(int order) //执行结果判断
{
	//扫描等待队列，装载可用pcb入就绪队列
	if (!Waiting_queue.empty())
	{
		vector<pcb>::iterator wq = Waiting_queue.begin();
		while (wq != Waiting_queue.end())
		{
			if ((*wq).Arrive_time == order) //阻塞转就绪队列
			{
				(*wq).State = 0;			  //状态改为就绪
				Ready_queue.push_back((*wq)); //加入就绪队列
				wq = Waiting_queue.erase(wq); //在阻塞队列中删除
			}
			else
			{
				wq++;
			}
		}
	}
	//先处理阻塞队列
	if (!Block_queue.empty())
	{
		vector<pcb>::iterator bq = Block_queue.begin();
		while (bq != Block_queue.end())
		{
			(*bq).Block_time_t--;		 //阻塞队列阻塞时间--
			if ((*bq).Block_time_t == 0) //阻塞转就绪队列
			{
				(*bq).State = 0; //状态改为就绪
				(*bq).Start_block_t = (*bq).Start_block;
				Ready_queue.push_back((*bq)); //加入就绪队列
				bq = Block_queue.erase(bq);	  //在阻塞队列中删除
			}
			else
			{
				bq++;
			}
		}
	}
	//如果就绪队列不为空，运行就绪队列
	if (!Ready_queue.empty()) //就绪队列不为空,对就绪队列操作
	{
		//对就绪队列排序
		sort();
		//运行就绪队列
		Ready_queue[0].Rest_time--;
		Ready_queue[0].Running_time++;
		Ready_queue[0].Start_block_t--;
		//进程的已占用CPU时间已达到所需要的运行时间，撤消该进程
		if (Ready_queue[0].Rest_time <= 0)
		{
			Ready_queue[0].State = 2; //完成
			Ready_queue[0].Total_time = order + 1 - Ready_queue[0].Arrive_time;
			Finish_queue.push_back(Ready_queue[0]); //就绪转结束队列
			Ready_queue.erase(Ready_queue.begin()); //删除该进程
		}
		else
		{
			Ready_queue[0].priority -= 3;
			//是否即将阻塞，就绪转阻塞队列？
			if (Ready_queue[0].Start_block_t <= 0)
			{
				Ready_queue[0].State = 1; //阻塞
				Ready_queue[0].Block_time_t = Ready_queue[0].Block_time;
				Block_queue.push_back(Ready_queue[0]); //加入阻塞队列
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
	return 0;
}
