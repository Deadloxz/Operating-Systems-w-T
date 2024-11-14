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

class DustemOS {
public:
DustemOS() : loggedIn(false), processCounter(0) {
users.push_back({"admin" , "password"});
srand(time(0));
}

void runOS() {
boot();
while (!loggedIn) {
loggedIn = login();
}
simulate();
}

void simulate() {
string schedulingAlgorithm;
cout << "Enter scheduling algorithm (FCFS or SJF): ";
cin >> schedulingAlgorithm;

int cycle = 0;
int cycleLimit = rand() % MAX_CYCLE_LIMIT + 1;
cout << "Simulation will run for a maximum of " << cycleLimit << " cycles.\n";

queue<Process*> readyQueue;
vector<Process*> waitingQueue;
Process* currentProcess = nullptr;

while (cycle < cycleLimit) {

if (cycle % 5 == 0) {
Process* newProcess = createRandomProcess();
readyQueue.push(newProcess);
cout << "Cycle " << cycle << ": Process " << newProcess->id << "starts running.\n";
}

if (currentProcess == nullptr && !readyQueue.empty()) {
currentProcess = (schedulingAlgorithm == "FCFS") ? selectFCFS(readyQueue) : selectSJF(readyQueue);
currentProcess->state = "running";
cout << "Cycle " << cycle << ": Process " currentProcess->id << " starts running. \n";
}

if (currentProcess != nullptr) {
  currentProcess->remainingTime--;
if (currentProcess->remainingTime == 0) {
    cout << "Cycle " << cycle << ": Process " currentProcess ->id << " completes CPU burst. \n";
    handleBurstCompletion(currentProcess, readyQueue, waitingQueue);
    currentProcess = nullptr;
  }
}

updateWaitingQueue(waitingQueue, readyQueue);

if (readyQueue.empty() && waitingQueue.empty() && currentProcess == nullptr) {
cout << "Cycle " << cycle << ": Simulation complete. \n";
break;
}
    cycle++;
}

if (cycle >=cycleLimit) {
cout << "Simulation reached the random cycle limit of " << cycleLimit << " cycles. \n";
}
}

private:
  vector <user> users;
  boot loggedIn;
  int processCounter;
  //boot sequence w/ animation
void boot()
{
cout << "System is now booting..." << endl;
sleep(2);
cout << "Winds are picking up..." << endl;
sleep(2);
cout << "Tumbleweeds are tumbling..." << endl;
sleep(2);
cout << "System is now booted." << endl;
}

bool login()
{
string username, password;
cout << "Enter Username: " << end;
cin >> username;
cout << "Enter password: " << endl;
cin >> password;

for (const auto& user : users )
{
if (user.username == username && user.password == password)
{
cout << "Welcome DustDevil!" << endl;
return true;
}
}
cout << "Login Failed. Try again... " << endl;
return false;
}

//Randomly create a process with alterning CPU / IO bursts

Process* createRandomProcess()
{
vector <int> bursts;
int numBursts = 2 + (rand() % 4);
for (int i = 0; i < numBursts; ++i)
{
bursts.pushback(1 + rand() % 10);
}
return new Process(processCounter++, bursts, 0);
}

// select process for FCFS scheduling
Process* selectSJF(queue<Process*>& readyQueue)
{
vector <Process*>temp;
while(!readyQueue.empty())
{
  temp.push_back(readyQueue.front());
  readyQueue.pop();
}
auto shortest = min_element(temp.begin(), temp.end(),[](Process* a, Process* b)
{
return a->remainingTime < b->remainingTime;
});
Process* p = *shortest;
temp.erase(shortest);
for (auto proc : temp)
{
readyQueue.push(proc);
}
return p;
}

//update waiting queue for IO bursts
void updateWaitingQueue(vector<Process*>& waitingQueue, queue<Process*>& readyQueue)
{
for (auto it = waitingQueue.begin(); it!= waitingQueue.end();)
{
Process* p = *it;
p->remainingTime--;
if (p->remainingTime == 0)
{
p->currentBurstsIndex++;
p->state = "ready";
p->remainingTime = p->burstsSequence[p->currentBurstsIndex];
readyQueue.push(p);
it = waitingQueue.erase(it);
}
else
{
++it;
}
}
}

//Handle the completion of a cpu bursts
void handleBurstsCompletion(Process* p,queue<Process*>& readyQueue, vector<Process*>& waitingQueue)
{
p->currentBurstsIndex++;
if (p->currentBurstsIndex < p->burstsSequence.size())
{
if (p->currentBurstsIndex % 2 == 1)
{
//IO bursts
p->state = "waiting";
p->remainingTime = p->burstsSequence[p->currentBurstsIndex];
waitingQueue.push_back(p);
}
else
{
//Next CPU bursts
p->state = "ready";
p->remainingTime = p->burstSequence[p->currentBurstIndex];
readyQueue.push(p);
}
}
else
{
p->state = "finished";
cout << "Process " << p->id << " has finished all bursts. " << endl;
}
}
};

int main()
{
DustemOS os;
os.runOS();

return 0;
}

