#include <bits/stdc++.h>
using namespace std;

struct Task {
    string name;
    int priority;
    int burst;
};

vector<Task> readTasksFromFile(const string& filename) {
    vector<Task> tasks;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        Task task;

        getline(ss, task.name, ',');
        ss >> task.priority;
        ss.ignore();
        ss >> task.burst;

        // Trim whitespace around task name
        task.name.erase(task.name.find_last_not_of(" \t\n\r") + 1);

        tasks.push_back(task);
    }

    return tasks;
}

void fcfs(const vector<Task>& tasks) {
    cout << "FCFS Scheduling:\n";
    int currentTime = 0;
    int totalWaitingTime = 0;

    for (const auto& task : tasks) {
        cout << "Executing " << task.name << " (Burst: " << task.burst << ")\n";
        totalWaitingTime += currentTime;
        currentTime += task.burst;
    }

    cout << "Total Time: " << currentTime << "\n";
    cout << "Average Waiting Time: " << (double)totalWaitingTime / tasks.size() << "\n";
}

void sjf(vector<Task> tasks) {
    cout << "SJF Scheduling:\n";
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.burst < b.burst;
    });

    int currentTime = 0;
    int totalWaitingTime = 0;

    for (const auto& task : tasks) {
        cout << "Executing " << task.name << " (Burst: " << task.burst << ")\n";
        totalWaitingTime += currentTime;
        currentTime += task.burst;
    }

    cout << "Total Time: " << currentTime << "\n";
    cout << "Average Waiting Time: " << (double)totalWaitingTime / tasks.size() << "\n";
}

void priorityScheduling(vector<Task> tasks) {
    cout << "Priority Scheduling:\n";
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.priority > b.priority; // Higher priority executed first
    });

    int currentTime = 0;
    int totalWaitingTime = 0;

    for (const auto& task : tasks) {
        cout << "Executing " << task.name << " (Priority: " << task.priority << ", Burst: " << task.burst << ")\n";
        totalWaitingTime += currentTime;
        currentTime += task.burst;
    }

    cout << "Total Time: " << currentTime << "\n";
    cout << "Average Waiting Time: " << (double)totalWaitingTime / tasks.size() << "\n";
}

void roundRobin(vector<Task> tasks, int quantum) {
    cout << "Round Robin Scheduling (Quantum: " << quantum << "):\n";
    int currentTime = 0;
    int totalWaitingTime = 0;
    map<string, int> waitingTimes;

    for (const auto& task : tasks) {
        waitingTimes[task.name] = 0;
    }

    while (!tasks.empty()) {
        for (auto it = tasks.begin(); it != tasks.end();) {
            if (it->burst <= quantum) {
                totalWaitingTime += currentTime - waitingTimes[it->name];
                cout << "Executing " << it->name << " (Burst Remaining: " << it->burst << ")\n";
                currentTime += it->burst;
                it = tasks.erase(it);
            } else {
                cout << "Executing " << it->name << " for " << quantum << " time units\n";
                it->burst -= quantum;
                totalWaitingTime += currentTime - waitingTimes[it->name];
                currentTime += quantum;
                waitingTimes[it->name] = currentTime;
                ++it;
            }
        }
    }

    cout << "Total Time: " << currentTime << "\n";
    cout << "Average Waiting Time: " << (double)totalWaitingTime / waitingTimes.size() << "\n";
}

void priorityWithRoundRobin(vector<Task> tasks, int quantum) {
    cout << "Priority with Round Robin Scheduling (Quantum: " << quantum << "):\n";

    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.priority > b.priority;
    });

    int currentTime = 0;
    int totalWaitingTime = 0;
    map<string, int> waitingTimes;

    for (const auto& task : tasks) {
        waitingTimes[task.name] = 0;
    }

    while (!tasks.empty()) {
        for (auto it = tasks.begin(); it != tasks.end();) {
            if (it->burst <= quantum) {
                totalWaitingTime += currentTime - waitingTimes[it->name];
                cout << "Executing " << it->name << " (Priority: " << it->priority << ", Burst Remaining: " << it->burst << ")\n";
                currentTime += it->burst;
                it = tasks.erase(it);
            } else {
                cout << "Executing " << it->name << " for " << quantum << " time units\n";
                it->burst -= quantum;
                totalWaitingTime += currentTime - waitingTimes[it->name];
                currentTime += quantum;
                waitingTimes[it->name] = currentTime;
                ++it;
            }
        }
    }

    cout << "Total Time: " << currentTime << "\n";
    cout << "Average Waiting Time: " << (double)totalWaitingTime / waitingTimes.size() << "\n";
}

int main() {
    vector<Task> tasks = readTasksFromFile("schedule.txt");
    int quantum = 5;  // Define a time quantum for Round Robin algorithms

    fcfs(tasks);
    cout << "\n";
    sjf(tasks);
    cout << "\n";
    priorityScheduling(tasks);
    cout << "\n";
    roundRobin(tasks, quantum);
    cout << "\n";
    priorityWithRoundRobin(tasks, quantum);

    return 0;
}