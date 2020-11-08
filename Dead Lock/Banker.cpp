#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>

#define OUTPUT_UNIT_W 7
#define OUTPUT_TABLE_W OUTPUT_UNIT_W * 3
#define OUTPUT_W OUTPUT_TABLE_W * 5

using namespace std;

struct resRequst
{
	// subscript(number) of the resource request process
	int reqPro;
	// quantity of each request resource
	vector<int> req;
};

int pro, res;
const string line_d(OUTPUT_W / 2, '=');
const string line_s(OUTPUT_W, '-');
vector<vector<int>>
	allocation, maximum, need,
	allocation_bu, need_bu;
vector<int> available, resource,
	available_bu;
vector<resRequst> requestQueue;

void printResRequest(resRequst &rr)
{
	cout << ">>>Request:{" << rr.reqPro << ",[";
	for (int i = 0; i < res; i++)
		cout << rr.req[i] << " ";
	cout << "]}"
		 << endl;
}

void printRes()
{
	cout << line_s << endl;
	cout.width(OUTPUT_TABLE_W);
	cout << " ";
	cout.width(OUTPUT_TABLE_W);
	cout << "Max";
	cout.width(OUTPUT_TABLE_W);
	cout << "Allocation";
	cout.width(OUTPUT_TABLE_W);
	cout << "Need";
	cout.width(OUTPUT_TABLE_W);
	cout << "Available";
	cout << endl;
	cout.width(OUTPUT_TABLE_W);
	cout << "Process|Resource";
	for (int i = 1; i <= 4; i++)
	{
		cout.width(OUTPUT_UNIT_W);
		cout << "A";
		cout.width(OUTPUT_UNIT_W);
		cout << "B";
		cout.width(OUTPUT_UNIT_W);
		cout << "C";
	}
	cout << endl;
	for (int i = 0; i < pro; i++)
	{
		cout.width(OUTPUT_TABLE_W);
		cout << i;
		for (int j = 0; j < res; j++)
		{
			cout.width(OUTPUT_UNIT_W);
			cout << maximum[i][j];
		}
		for (int j = 0; j < res; j++)
		{
			cout.width(OUTPUT_UNIT_W);
			cout << allocation[i][j];
		}
		for (int j = 0; j < res; j++)
		{
			cout.width(OUTPUT_UNIT_W);
			cout << need[i][j];
		}
		if (i == 0)
		{
			for (int j = 0; j < res; j++)
			{
				cout.width(OUTPUT_UNIT_W);
				cout << available[j];
			}
		}
		cout << endl;
	}
}

void initProcess()
{
	int n; // store input data
	// define number of process and resources' type
	cout << "Input the number of processes:" << endl;
	cin >> pro;
	cout << "Input the number of resources' type:" << endl;
	cin >> res;

	//input matrix Max
	cout << ("Input the maximum matrix: ") << endl;
	for (int i = 0; i < pro; i++)
	{
		vector<int> p_maxRes_t;
		for (int j = 0; j < res; j++)
		{
			cin >> n;
			p_maxRes_t.push_back(n);
		}
		maximum.push_back(p_maxRes_t);
		cout << endl;
	}

	//input matrix allocation
	cout << ("Input the allocation matrix: ") << endl;
	for (int i = 0; i < pro; i++)
	{
		vector<int> p_alloRes_t;
		for (int j = 0; j < res; j++)
		{
			cin >> n;
			p_alloRes_t.push_back(n);
		}
		allocation.push_back(p_alloRes_t);
		cout << ("\n");
	}

	//input matrix available
	cout << ("Input the available matrix: ") << endl;
	for (int i = 0; i < res; i++)
	{
		cin >> n;
		available.push_back(n);
	}

	//calc matrix need
	for (int i = 0; i < pro; i++)
	{
		vector<int> p_needRes_t;
		for (int j = 0; j < res; j++)
		{
			n = maximum[i][j] - allocation[i][j];
			p_needRes_t.push_back(n);
		}
		need.push_back(p_needRes_t);
	}

	//calc matrix resource
	for (int j = 0; j < res; j++)
		resource.push_back(available[j]);
	for (int i = 0; i < pro; i++)
	{
		for (int j = 0; j < res; j++)
			resource[j] += allocation[i][j];
	}
}

void initRequest()
{
	int n; // store input data
	cout << "Input request process(-1 to finish):" << endl;
	cin >> n;
	if (n != -1)
	{
		while (n != -1)
		{
			resRequst rr;
			rr.reqPro = n;
			for (int i = 0; i < res; i++)
			{
				int rn;
				cin >> rn;
				rr.req.push_back(rn);
			}
			requestQueue.push_back(rr);
			printResRequest(rr);
			cin >> n;
		}
	}
}

int safeCheck_findAvaiProcess(vector<int> &work, vector<bool> &finish)
{
	for (int i = 0; i < pro; i++)
	{
		if (finish[i])
			continue;
		bool ok = true;
		for (int j = 0; j < res; j++)
		{
			if (need[i][j] > work[j])
			{
				ok = false;
				break;
			}
		}
		if (ok)
			return i;
	}
	return -1;
}

bool safeCheck()
{
	vector<int> work = available;
	vector<int> safeQueue;
	vector<bool> finish(pro, false);
	while (safeCheck_findAvaiProcess(work, finish) != -1)
	{
		int avaiProcess = safeCheck_findAvaiProcess(work, finish);
		for (int i = 0; i < res; i++)
		{
			work[i] += allocation[avaiProcess][i];
		}
		finish[avaiProcess] = true;
		safeQueue.push_back(avaiProcess);
	}
	for (int i = 0; i < pro; i++)
	{
		if (!finish[i])
		{
			cout << "<!>Safe check failed!" << endl;
			return false;
		}
	}
	cout << "  >Safe check passed. "
		 << "Safe Queue: ";
	for (int i = 0; i < pro; i++)
		cout << safeQueue[i] << " ";
	cout << endl;
	return true;
}

void rollBack()
{
	available = available_bu;
	allocation = allocation_bu;
	need = need_bu;
}

bool requestHandle(resRequst resReq)
{
	cout << line_d << " Request Handle " << line_d << endl;
	printResRequest(resReq);
	for (int i = 0; i < res; i++)
	{
		if (resReq.req[i] > need[resReq.reqPro][i])
		{
			cout << "<!>Request check failed: request res(" << resReq.req[i] << ")>need res" << need[resReq.reqPro][i] << endl;
			return false;
		}
	}
	for (int i = 0; i < res; i++)
	{
		if (resReq.req[i] > available[i])
		{
			cout << "<!>Request check failed: request res[" << i << "](" << resReq.req[i] << ")>available res[" << i << "](" << available[i] << ")" << endl;
			return false;
		}
	}
	cout << "  >Request check passed." << endl;
	//pre-resource allocate
	//back up the original resource data for rollback
	available_bu = available;
	allocation_bu = allocation;
	need_bu = need;
	//start to pre-resource allocate
	for (int i = 0; i < res; i++)
	{
		available[i] -= resReq.req[i];
		allocation[resReq.reqPro][i] += resReq.req[i];
		need[resReq.reqPro][i] -= resReq.req[i];
	}
	cout << "  >Request resource pre-allocated." << endl;
	//Run safe check. If it is not safe, rollback the resource data from backup and this allocation will be cancelled.
	//Or the allocation is completed.
	if (safeCheck())
	{
		cout << "  >Resource allocation completed." << endl;
		return true;
	}
	else
	{
		rollBack();
		cout << "<!>Rollback resource data." << endl
			 << "<!>Resource allocation cancelled. " << endl;
		return false;
	}
}

int main()
{
	cout.setf(std::ios::left);
	initProcess();
	// run the first safe check for initial resource matrix. if not safe, exit directly
	if (!safeCheck())
		exit(-1);
	initRequest();
	printRes();
	// handle request of process if user input
	if (requestQueue.size() != 0)
	{
		for (vector<resRequst>::iterator rqi = requestQueue.begin(); rqi != requestQueue.end(); rqi++)
		{
			requestHandle(*rqi);
			printRes();
		}
		cout << line_d << line_d << endl
			 << ">>>All requests handled." << endl;
	}
	system("pause");
}