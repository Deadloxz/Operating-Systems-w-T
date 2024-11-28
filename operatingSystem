#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_CYCLE_LIMIT = 10000;

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

while (cycle < cycleLimit) { // Use random cycle limit
            // Create new process every 5 cycles
            if (cycle % 5 == 0) {
                Process* newProcess = createRandomProcess();
                readyQueue.push(newProcess);
                cout << "Cycle " << cycle << ": Process " << newProcess->id << " created.\n";
            }

            // CPU scheduling
            if (currentProcess == nullptr && !readyQueue.empty()) {
                currentProcess = (schedulingAlgorithm == "FCFS") ? selectFCFS(readyQueue) : selectSJF(readyQueue);
                currentProcess->state = "running";
                cout << "Cycle " << cycle << ": Process " << currentProcess->id << " starts running.\n";
            }

            // Process current CPU burst
            if (currentProcess != nullptr) {
                currentProcess->remainingTime--;
                if (currentProcess->remainingTime == 0) {
                    cout << "Cycle " << cycle << ": Process " << currentProcess->id << " completes CPU burst.\n";
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
    vector<User> users;
    bool loggedIn;
    int processCounter;

    // Boot sequence with animation
    void boot() {
        cout << "System is now booting...\n";
        sleep(2);
        cout << "Winds are picking up...\n";
        sleep(2);
        cout << "Tumbleweeds are tumbling...\n";
        sleep(2);
        cout << "System is now booted.\n";
    }

    bool login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                cout << "Welcome Dust Devil!\n";
                return true;
            }
        }
        cout << "Login failed. Try again.\n";
        return false;
    }

    // Randomly create a process with alternating CPU/IO bursts
    Process* createRandomProcess() {
        vector<int> bursts;
        int numBursts = 2 + (rand() % 4);
        for (int i = 0; i < numBursts; ++i) {
            bursts.push_back(1 + rand() % 10);
        }
        return new Process(processCounter++, bursts, 0);
    }

    // Select process for FCFS scheduling
    Process* selectFCFS(queue<Process*>& readyQueue) {
        Process* p = readyQueue.front();
        readyQueue.pop();
        return p;
    }

    // Select process for SJF scheduling
    Process* selectSJF(queue<Process*>& readyQueue) {
        vector<Process*> temp;
        while (!readyQueue.empty()) {
            temp.push_back(readyQueue.front());
            readyQueue.pop();
        }
        auto shortest = min_element(temp.begin(), temp.end(), [](Process* a, Process* b) {
            return a->remainingTime < b->remainingTime;
        });
        Process* p = *shortest;
        temp.erase(shortest);
        for (auto proc : temp) {
            readyQueue.push(proc);
        }
        return p;
    }

    // Update waiting queue for IO bursts
    void updateWaitingQueue(vector<Process*>& waitingQueue, queue<Process*>& readyQueue) {
        for (auto it = waitingQueue.begin(); it != waitingQueue.end(); ) {
            Process* p = *it;
            p->remainingTime--;
            if (p->remainingTime == 0) {
                p->currentBurstIndex++;
                p->state = "ready";
                p->remainingTime = p->burstSequence[p->currentBurstIndex];
                readyQueue.push(p);
                it = waitingQueue.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Handle completion of a CPU burst
    void handleBurstCompletion(Process* p, queue<Process*>& readyQueue, vector<Process*>& waitingQueue) {
        p->currentBurstIndex++;
        if (p->currentBurstIndex < p->burstSequence.size()) {
            if (p->currentBurstIndex % 2 == 1) { // IO burst
                p->state = "waiting";
                p->remainingTime = p->burstSequence[p->currentBurstIndex];
                waitingQueue.push_back(p);
            } else { // Next CPU burst
                p->state = "ready";
                p->remainingTime = p->burstSequence[p->currentBurstIndex];
                readyQueue.push(p);
            }
        } else {
            p->state = "finished";
            cout << "Process " << p->id << " has finished all bursts.\n";
        }
    }
};

int main() {
    DustemOS os;
    os.runOS();
    return 0;
}

