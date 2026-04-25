#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// ================= CONSTANT =================
#define MAX 500

// ================= USER =================
struct User {
    char username[50];
    char password[50];
} currentUser;

// ================= TASK =================
struct Task {
    int id;
    char name[100];
    char category[50];
    int day, month;
    int hour;
    int priority;
    bool done;
};

Task tasks[MAX];
int taskCount = 0;

// ================= HABIT =================
struct Habit {
    char name[100];
    int streak;
    int total;
    Habit* next;
};

Habit* habitHead = NULL;

// ================= STACK =================
struct Stack {
    int data[MAX];
    int top;
} undoStack;

// ================= QUEUE =================
struct Queue {
    int data[MAX];
    int front, rear;
} scheduleQ;

// ================= LOG =================
char logs[1000][100];
int logCount = 0;

// ================= INIT =================
void init() {
    undoStack.top = -1;
    scheduleQ.front = 0;
    scheduleQ.rear = -1;
}

// ================= STACK =================
void push(int x) {
    if (undoStack.top < MAX - 1)
        undoStack.data[++undoStack.top] = x;
}

int pop() {
    if (undoStack.top == -1) return -1;
    return undoStack.data[undoStack.top--];
}

// ================= QUEUE =================
void enqueue(int x) {
    if (scheduleQ.rear < MAX - 1)
        scheduleQ.data[++scheduleQ.rear] = x;
}

int dequeue() {
    if (scheduleQ.front > scheduleQ.rear) return -1;
    return scheduleQ.data[scheduleQ.front++];
}

// ================= LOG =================
void addLog(const char* msg) {
    strcpy(logs[logCount++], msg);
}

void showLogs() {
    cout << "\n--- LOG HISTORY ---\n";
    for (int i = 0; i < logCount; i++)
        cout << i + 1 << ". " << logs[i] << endl;
}

// ================= LOGIN =================
void registerUser() {
    cout << "Enter Username: ";
    cin >> currentUser.username;
    cout << "Enter Password: ";
    cin >> currentUser.password;

    ofstream f("user.txt");
    f << currentUser.username << " " << currentUser.password;
    f.close();

    cout << "Registered Successfully!\n";
}

int loginUser() {
    char u[50], p[50];

    cout << "Login\n";
    cout << "Username: "; cin >> u;
    cout << "Password: "; cin >> p;

    ifstream f("user.txt");
    if (!f) return 0;

    f >> currentUser.username >> currentUser.password;

    if (strcmp(u, currentUser.username) == 0 &&
        strcmp(p, currentUser.password) == 0) {
        cout << "Login Successful!\n";
        return 1;
    }

    return 0;
}

// ================= TASK =================
void addTask() {
    Task &t = tasks[taskCount];

    cout << "\n--- Add Task ---\n";
    cout << "ID: "; cin >> t.id;

    cout << "Name: ";
    cin.ignore();
    cin.getline(t.name, 100);

    cout << "Category: ";
    cin.getline(t.category, 50);

    cout << "Date (day month): ";
    cin >> t.day >> t.month;

    cout << "Hour (0-23): ";
    cin >> t.hour;

    cout << "Priority (1=High,2=Med,3=Low): ";
    cin >> t.priority;

    t.done = false;

    enqueue(t.id);
    addLog("Task Added");

    taskCount++;
    cout << "Task Added!\n";
}

// ================= DISPLAY =================
void showTasks() {
    cout << "\n--- TASK LIST ---\n";

    for (int i = 0; i < taskCount; i++) {
        cout << tasks[i].id << " | "
             << tasks[i].name << " | "
             << tasks[i].category << " | "
             << tasks[i].day << "/" << tasks[i].month << " | "
             << tasks[i].hour << "h | "
             << "P:" << tasks[i].priority << " | "
             << (tasks[i].done ? "Done" : "Pending") << endl;
    }
}

// ================= MARK DONE =================
void markDone() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            tasks[i].done = true;
            push(id);
            addLog("Task Completed");
            cout << "Done!\n";
            return;
        }
    }

    cout << "Not found!\n";
}

// ================= UNDO =================
void undoTask() {
    int id = pop();

    if (id == -1) {
        cout << "Nothing to undo\n";
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            tasks[i].done = false;
            addLog("Undo Performed");
            cout << "Undo Successful!\n";
        }
    }
}

// ================= FILTER =================
void filterCategory() {
    char cat[50];

    cout << "Enter Category: ";
    cin >> cat;

    cout << "\n--- FILTERED ---\n";

    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].category, cat) == 0) {
            cout << tasks[i].name << endl;
        }
    }
}

// ================= DELETE =================
void deleteTask() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < taskCount - 1; j++)
                tasks[j] = tasks[j + 1];

            taskCount--;
            addLog("Task Deleted");
            cout << "Deleted!\n";
            return;
        }
    }

    cout << "Not Found!\n";
}

// ================= HABIT =================
void addHabit() {
    Habit* h = new Habit;

    cout << "Habit Name: ";
    cin.ignore();
    cin.getline(h->name, 100);

    h->streak = 0;
    h->total = 0;
    h->next = habitHead;
    habitHead = h;

    addLog("Habit Added");
}

void showHabits() {
    Habit* t = habitHead;

    cout << "\n--- HABITS ---\n";

    while (t) {
        cout << t->name
             << " | Streak: " << t->streak
             << " | Total: " << t->total << endl;
        t = t->next;
    }
}

void updateHabit() {
    char name[100];

    cout << "Enter Habit: ";
    cin.ignore();
    cin.getline(name, 100);

    Habit* t = habitHead;

    while (t) {
        if (strcmp(t->name, name) == 0) {
            t->streak++;
            t->total++;
            addLog("Habit Updated");
            cout << "Updated!\n";
            return;
        }
        t = t->next;
    }

    cout << "Not found!\n";
}

// ================= FILE =================
void saveTasks() {
    ofstream f("tasks.txt");

    for (int i = 0; i < taskCount; i++) {
        f << tasks[i].id << " "
          << tasks[i].name << " "
          << tasks[i].category << " "
          << tasks[i].day << " "
          << tasks[i].month << " "
          << tasks[i].hour << " "
          << tasks[i].priority << " "
          << tasks[i].done << "\n";
    }

    f.close();
    cout << "Saved!\n";
}

void loadTasks() {
    ifstream f("tasks.txt");

    while (f >> tasks[taskCount].id) {
        f >> tasks[taskCount].name
          >> tasks[taskCount].category
          >> tasks[taskCount].day
          >> tasks[taskCount].month
          >> tasks[taskCount].hour
          >> tasks[taskCount].priority
          >> tasks[taskCount].done;

        taskCount++;
    }
}

// ================= ASCII CHART =================
void showChart() {
    cout << "\n--- PROGRESS ---\n";

    for (int i = 0; i < taskCount; i++) {
        cout << "Task " << tasks[i].id << ": ";
        if (tasks[i].done)
            cout << "*****";
        else
            cout << "**";
        cout << endl;
    }
}

// ================= RECOMMEND =================
void recommend() {
    int pending = 0;

    for (int i = 0; i < taskCount; i++)
        if (!tasks[i].done) pending++;

    cout << "\n--- RECOMMENDATION ---\n";

    if (pending > 5)
        cout << "Too many pending tasks!\n";
    else
        cout << "Good progress!\n";
}

// ================= SCHEDULE =================
void processSchedule() {
    int id = dequeue();

    if (id == -1) {
        cout << "No tasks!\n";
        return;
    }

    cout << "Processing Task ID: " << id << endl;
}

// ================= MENU =================
void menu() {
    int ch;

    while (1) {
        cout << "\n===== DAILY PLANNER =====\n";
        cout << "1.Add Task\n2.Show Tasks\n3.Mark Done\n4.Undo\n";
        cout << "5.Filter\n6.Delete\n7.Add Habit\n8.Show Habit\n";
        cout << "9.Update Habit\n10.Save\n11.Load\n";
        cout << "12.Chart\n13.Recommend\n14.Schedule\n15.Logs\n16.Exit\n";

        cout << "Choice: ";
        cin >> ch;

        switch (ch) {
            case 1: addTask(); break;
            case 2: showTasks(); break;
            case 3: markDone(); break;
            case 4: undoTask(); break;
            case 5: filterCategory(); break;
            case 6: deleteTask(); break;
            case 7: addHabit(); break;
            case 8: showHabits(); break;
            case 9: updateHabit(); break;
            case 10: saveTasks(); break;
            case 11: loadTasks(); break;
            case 12: showChart(); break;
            case 13: recommend(); break;
            case 14: processSchedule(); break;
            case 15: showLogs(); break;
            case 16: exit(0);
            default: cout << "Invalid\n";
        }
    }
}

// ================= MAIN =================
int main() {
    init();

    if (!loginUser()) {
        cout << "Register First!\n";
        registerUser();
    }

    loadTasks();
    menu();

    return 0;
}
