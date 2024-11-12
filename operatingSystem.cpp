#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_CYCLE_LIMIT = 10000

struct User {
string username;
string password;
};

struct Process {
int id;
vector<int> burstSequence;
int arrivalTime;
int currentBurstIndex;
int remainingTime;
string state;
Process(int id, vector<int> bursts, int arrival) : id(id), burstSequence(bursts), arrivalTime(arrival), currentBurstIndex(0), remainingTime(bursts[0]), state("ready") {}
};

int main()
{

cout << "Hello TAMIU!!!" << endl;

return 0;
}

