// Priority Preemptive with Arrival Time
// Mounish Sai (https://github.com/pvsmounish)

#include<iostream>
#include<limits>
using namespace std;

class Process {
public:
    string processName;
    int arrivalTime;
    int cpuBurstTime[2];
    int ioBurstTime;
    int priority;
    bool ioDone;

    int remainingTime;

    int responseTime;
    int completionTime;
    int inceptionTime;


    int waitingTime;
    int turnAroundTime;
    
    int factor;

    void initialize() {
        remainingTime = cpuBurstTime[0];
        inceptionTime = arrivalTime;
        ioDone = 0;
        factor = arrivalTime +  cpuBurstTime[0] + cpuBurstTime[1];
        priority= factor;
    }
};

bool refactorAttributes(Process& p) {
    if (p.ioBurstTime == 0) {
        p.cpuBurstTime[0] += p.cpuBurstTime[1];
        p.cpuBurstTime[1] = 0;
    }
    else if (p.cpuBurstTime[0] == 0) {
        if (p.arrivalTime != p.ioBurstTime) return false;
        p.cpuBurstTime[0] += p.cpuBurstTime[1];
        p.cpuBurstTime[1] = 0;
    }
    return true;
}

int main() {

    freopen("novelin.txt", "r", stdin);
    freopen("novelout.txt", "w", stdout);
    int numOfProcesses;
    cout << "Enter no. of processes: ";
    cin >> numOfProcesses;

    Process processes[numOfProcesses];

    for (int n = 0; n < numOfProcesses; n++) {
        cout << "\nEnter Process Name for " << (n + 1) << ": ";
        cin >> processes[n].processName;
        cout << "Enter Arrival Time for Process " << (n + 1) << ": ";
        cin >> processes[n].arrivalTime;
        cout << "Enter (CPU, IO, CPU) Burst Times for Process " << (n + 1) << ": ";
        cin >> processes[n].cpuBurstTime[0] >> processes[n].ioBurstTime >> processes[n].cpuBurstTime[1];
        //cout << "Enter Priority for Process " << (n + 1) << ": ";
        //cin >> processes[n].priority;
        if (!refactorAttributes(processes[n])) {
            cout << "Process " << (n + 1) << " attributes mismatch\n";
            return 1;
        }

        processes[n].initialize();
    }

    cout << "\n" << endl;

    int currentTime = 0;

    while (true) {

        int currentHighestPriorityIndex = -1;
        int currentHighestPriority = numeric_limits<int>::max();
        int currentMinArrivalTime = numeric_limits<int>::max();

        bool isAllCompleted = true;

        for (int i = 0; i < numOfProcesses; i++) {
            if (processes[i].remainingTime > 0) {
                isAllCompleted = false;
                currentMinArrivalTime = min(currentMinArrivalTime, processes[i].arrivalTime);
            }
        }

        if (isAllCompleted) {
            break;
        }

        if (currentTime < currentMinArrivalTime) currentTime = currentMinArrivalTime;


        for (int i = 0; i < numOfProcesses; i++) {
            if (processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime) {
                if (processes[i].priority < currentHighestPriority) {
                    currentHighestPriority = processes[i].priority;
                    currentHighestPriorityIndex = i;
                }

            }
        }

        Process& cp = processes[currentHighestPriorityIndex];

        if (cp.remainingTime == cp.cpuBurstTime[0] && !cp.ioDone) {
            cp.responseTime = currentTime;
        }

        cp.remainingTime--;
        cp.priority++;
        currentTime++;

        if (cp.remainingTime == 0 && !cp.ioDone) {
            cp.remainingTime = cp.cpuBurstTime[1];
            if (cp.ioBurstTime > 0) {
                cp.arrivalTime = currentTime + cp.ioBurstTime;
                cp.ioDone = 1;
                if (cp.remainingTime == 0) cp.completionTime = cp.arrivalTime;
            }
            else cp.completionTime = currentTime;
        }
        else if (cp.remainingTime == 0) {
            cp.completionTime = currentTime;
        }
    }


    int sumResponseTime = 0;
    int sumCompletionTime = 0;
    int sumWaitingTime = 0;
    int sumTurnAroundTime = 0;
    int maxCompletionTime = -1;

    for (int n = 0; n < numOfProcesses; n++) {
        cout << "\nProcess " << processes[n].processName << ":\n";
        cout << "Response Time: " << processes[n].responseTime << endl;
        cout << "Completion Time: " << processes[n].completionTime << endl;
        maxCompletionTime = max(maxCompletionTime, processes[n].completionTime);
        processes[n].turnAroundTime = processes[n].completionTime - processes[n].inceptionTime;
        processes[n].waitingTime = processes[n].turnAroundTime - (processes[n].cpuBurstTime[0] + processes[n].cpuBurstTime[1] );
        cout << "Waiting Time: " << processes[n].waitingTime << endl;
        cout << "Turn Around Time: " << processes[n].turnAroundTime << "\n" << endl;

        sumResponseTime += processes[n].responseTime;
        sumCompletionTime += processes[n].completionTime;
        sumWaitingTime += processes[n].waitingTime;
        sumTurnAroundTime += processes[n].turnAroundTime;
    }

    cout << "\n\nAverage Response Time for " << (numOfProcesses) << " Processes: " << (float)sumResponseTime / numOfProcesses;
    cout << "\n\nAverage Completion Time for " << (numOfProcesses) << " Processes: " << (float)sumCompletionTime / numOfProcesses;
    cout << "\n\nAverage Waiting Time for " << (numOfProcesses) << " Processes: " << (float)sumWaitingTime / numOfProcesses;
    cout << "\n\nAverage Turn Around Time for " << (numOfProcesses) << " Processes: " << (float)sumTurnAroundTime / numOfProcesses;
    cout << "\n\nThroughput for " << (numOfProcesses) << " Processes: " << (float)numOfProcesses / maxCompletionTime;

    return 0;
}