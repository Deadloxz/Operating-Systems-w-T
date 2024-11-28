#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unordered_map> 

using namespace std;

const int MAX_CYCLE_LIMIT = 10000;   // Maximum possible cycle limit
const int PAGE_SIZE = 4096;          // 4 KB page size
const int NUM_PAGES = 256;           // Number of virtual pages

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
    unordered_map<int, int> pageTable; 

    Process(int id, vector<int> bursts, int arrival)
        : id(id), burstSequence(bursts), arrivalTime(arrival), currentBurstIndex(0), remainingTime(bursts[0]), state("ready") {
        initializePageTable();
    }

    void initializePageTable() {
        for (int i = 0; i < NUM_PAGES; ++i) {
            pageTable[i] = -1; 
        }
    }
};

class MMU {
    unordered_map<int, int> tlb; 

public:
    int translate(int virtualAddress, Process* process) {
        int pageNumber = virtualAddress / PAGE_SIZE;
        int offset = virtualAddress % PAGE_SIZE;

        if (tlb.find(pageNumber) != tlb.end()) {
            return tlb[pageNumber] * PAGE_SIZE + offset;
        }

        int frameNumber = process->pageTable[pageNumber];
        if (frameNumber == -1) {
            frameNumber = loadPageIntoMemory(pageNumber, process); 
        }

        tlb[pageNumber] = frameNumber;
        return frameNumber * PAGE_SIZE + offset;
    }

private:
    int loadPageIntoMemory(int pageNumber, Process* process) {
        static int nextFrame = 0;
        process->pageTable[pageNumber] = nextFrame;
        cout << "Page fault: Loading virtual page " << pageNumber << " into frame " << nextFrame << "\n";
        return nextFrame++;
    }
};

class DustemOS {
public:
    DustemOS() : loggedIn(false), processCounter(0), mmu() {
        users.push_back({"admin", "password"});
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
                cout << "Cycle " << cycle << ": Process " << newProcess->id << " created.\n";
            }

            if (currentProcess == nullptr && !readyQueue.empty()) {
                currentProcess = (schedulingAlgorithm == "FCFS") ? selectFCFS(readyQueue) : selectSJF(readyQueue);
                currentProcess->state = "running";
                cout << "Cycle " << cycle << ": Process " << currentProcess->id << " starts running.\n";
            }

            if (currentProcess != nullptr) {
                currentProcess->remainingTime--;
                if (cycle % 2 == 0) generateMemoryInstruction(currentProcess); 
                if (currentProcess->remainingTime == 0) {
                    cout << "Cycle " << cycle << ": Process " << currentProcess->id << " completes CPU burst.\n";
                    handleBurstCompletion(currentProcess, readyQueue, waitingQueue);
                    currentProcess = nullptr;
                }
            }


            updateWaitingQueue(waitingQueue, readyQueue);

            if (readyQueue.empty() && waitingQueue.empty() && currentProcess == nullptr) {
                cout << "Cycle " << cycle << ": Simulation complete.\n";
                break;
            }

            cycle++;
        }

        if (cycle >= cycleLimit) {
            cout << "Simulation reached the random cycle limit of " << cycleLimit << " cycles.\n";
        }
    }

private:
    vector<User> users;
    bool loggedIn;
    int processCounter;
    MMU mmu;

    void boot() {
        cout << "System is now booting...\n";
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

    Process* createRandomProcess() {
        vector<int> bursts;
        int numBursts = 2 + (rand() % 4);
        for (int i = 0; i < numBursts; ++i) {
            bursts.push_back(1 + rand() % 10);
        }
        return new Process(processCounter++, bursts, 0);
    }

    Process* selectFCFS(queue<Process*>& readyQueue) {
        Process* p = readyQueue.front();
        readyQueue.pop();
        return p;
    }

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

    void generateMemoryInstruction(Process* process) {
        int virtualAddress = rand() % (NUM_PAGES * PAGE_SIZE);
        int physicalAddress = mmu.translate(virtualAddress, process);
        cout << "Process " << process->id << " executes memory instruction (virtual: " << virtualAddress
             << ", physical: " << physicalAddress << ")\n";
    }

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

    void handleBurstCompletion(Process* p, queue<Process*>& readyQueue, vector<Process*>& waitingQueue) {
        p->currentBurstIndex++;
        if (p->currentBurstIndex < p->burstSequence.size()) {
            if (p->currentBurstIndex % 2 == 1) {
                p->state = "waiting";
                p->remainingTime = p->burstSequence[p->currentBurstIndex];
                waitingQueue.push_back(p);
            } else {
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
