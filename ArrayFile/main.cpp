#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

using namespace std;

// --- ARRAY MANAGEMENT MODULE (Requirements 3.1 - 3.2) ---
class ArrayManager {
public:
    // 3.1 Input from console and save to a text file
    static int* inputAndSaveText(int& n, const string& filename) {
        cout << "Enter array size: ";
        cin >> n;
        if (n <= 0) throw invalid_argument("Size must be positive!");

        int* arr = new int[n];
        cout << "Enter " << n << " elements:\n";
        for (int i = 0; i < n; i++) {
            cout << "[" << i << "]: ";
            cin >> arr[i];
        }

        ofstream outFile(filename);
        if (outFile) {
            outFile << n << "\n";
            for (int i = 0; i < n; i++) outFile << arr[i] << " ";
            outFile.close();
            cout << "Data saved to text file: " << filename << endl;
        }
        return arr;
    }

    // 3.2 Random generation and save to a binary file
    static int* generateAndSaveBinary(int& n, const string& filename) {
        cout << "Enter size for random array: ";
        cin >> n;
        if (n <= 0) throw invalid_argument("Size must be positive!");

        int* arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = rand() % 201 - 100; // Range [-100, 100]

        ofstream outFile(filename, ios::binary);
        if (outFile) {
            outFile.write(reinterpret_cast<char*>(&n), sizeof(int));
            outFile.write(reinterpret_cast<char*>(arr), sizeof(int) * n);
            outFile.close();
            cout << "Data saved to binary file: " << filename << endl;
        }
        return arr;
    }

    // Helper to save any dynamic array to text
    static void saveToText(const string& filename, int* arr, int n) {
        ofstream outFile(filename);
        if (outFile) {
            outFile << n << "\n";
            for (int i = 0; i < n; i++) outFile << arr[i] << " ";
            outFile.close();
        }
    }
};

// --- CONTAINER MODULE (Requirement 3.4) ---
void readIntoVectorAndPrint(const string& filename, bool isBinary) {
    vector<int> container;
    int n = 0;

    if (isBinary) {
        ifstream in(filename, ios::binary);
        if (in.read(reinterpret_cast<char*>(&n), sizeof(int))) {
            container.resize(n);
            in.read(reinterpret_cast<char*>(container.data()), sizeof(int) * n);
        }
    }
    else {
        ifstream in(filename);
        if (in >> n) {
            int temp;
            while (in >> temp) container.push_back(temp);
        }
    }

    cout << "\n[STL Vector Container] Content: ";
    for (const auto& val : container) cout << val << " ";
    cout << endl;
}

// --- TASK SOLUTIONS (Requirement 3.3) ---

// Task 1: Combine A and B into C with specific order (A+, B+, A-, B-)
void Task1_CombineArrays() {
    int n, m;
    cout << "\n--- TASK 1: Combine Arrays ---\n";
    int* A = ArrayManager::inputAndSaveText(n, "A_input.txt");
    int* B = ArrayManager::generateAndSaveBinary(m, "B_input.bin");

    int* C = new int[n + m];
    int current = 0;

    // Logic: Positive A, Positive B, Negative A, Negative B
    for (int i = 0; i < n; i++) if (A[i] >= 0) C[current++] = A[i];
    for (int i = 0; i < m; i++) if (B[i] >= 0) C[current++] = B[i];
    for (int i = 0; i < n; i++) if (A[i] < 0) C[current++] = A[i];
    for (int i = 0; i < m; i++) if (B[i] < 0) C[current++] = B[i];

    cout << "Resulting Array C: ";
    for (int i = 0; i < current; i++) cout << C[i] << " ";
    cout << endl;

    ArrayManager::saveToText("Result_Task1.txt", C, current);
    delete[] A; delete[] B; delete[] C;
}

// Task 2: Find max element in range [ak, bk] to the right of first positive
void Task2_FindMaxInRange() {
    int n;
    cout << "\n--- TASK 2: Max in Range ---\n";
    int* arr = ArrayManager::generateAndSaveBinary(n, "Task2_Input.bin");

    int ak, bk;
    cout << "Enter range boundaries [ak, bk]: ";
    cin >> ak >> bk;

    int firstPosIdx = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) { firstPosIdx = i; break; }
    }

    if (firstPosIdx == -1 || firstPosIdx == n - 1) {
        cout << "No elements exist to the right of the first positive element.\n";
    }
    else {
        int maxVal = -2147483648, maxIdx = -1;
        for (int i = firstPosIdx + 1; i < n; i++) {
            if (arr[i] >= ak && arr[i] <= bk) {
                if (arr[i] > maxVal) {
                    maxVal = arr[i];
                    maxIdx = i;
                }
            }
        }
        if (maxIdx != -1)
            cout << "First maximum at index: " << maxIdx << " (Value: " << maxVal << ")\n";
        else
            cout << "No elements found within the specified range.\n";
    }
    delete[] arr;
}

// Task 3: Cyclic shift right by k positions (N >= 200)
void Task3_CyclicShift() {
    int n;
    cout << "\n--- TASK 3: Cyclic Shift (N >= 200) ---\n";
    cout << "Enter array size: ";
    cin >> n;
    if (n < 200) {
        cout << "Size increased to 200 per requirements.\n";
        n = 200;
    }

    int* A = new int[n];
    for (int i = 0; i < n; i++) A[i] = i + 1; // Sequential fill for testing

    int k;
    cout << "Enter shift positions (k): ";
    cin >> k;
    k %= n; // Handle k > n

    // Shift logic using reversal algorithm
    auto reversePart = [](int* start, int* end) {
        while (start < end) swap(*start++, *end--);
        };

    reversePart(A, A + n - 1);
    reversePart(A, A + k - 1);
    reversePart(A + k, A + n - 1);

    cout << "Shift complete. First 15 elements: ";
    for (int i = 0; i < 15; i++) cout << A[i] << " ";
    cout << "..." << endl;

    ArrayManager::saveToText("Result_Task3.txt", A, n);
    delete[] A;
}

// --- MAIN INTERFACE ---

void displayMenu() {
    cout << "\n====================================\n";
    cout << "         LABORATORY WORK MENU       \n";
    cout << "====================================\n";
    cout << "1. Task 1: Combine Arrays A & B into C\n";
    cout << "2. Task 2: Find Max in Range [ak, bk]\n";
    cout << "3. Task 3: Cyclic Shift Right (N >= 200)\n";
    cout << "4. Container: Read File into Vector (3.4)\n";
    cout << "0. Exit\n";
    cout << "------------------------------------\n";
    cout << "Select an option: ";
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    int choice;

    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        try {
            switch (choice) {
            case 1: Task1_CombineArrays(); break;
            case 2: Task2_FindMaxInRange(); break;
            case 3: Task3_CyclicShift(); break;
            case 4:
                cout << "1. Read Text (Result_Task1.txt)\n2. Read Binary (Task2_Input.bin)\nChoice: ";
                int sub; cin >> sub;
                readIntoVectorAndPrint(sub == 1 ? "Result_Task1.txt" : "Task2_Input.bin", sub != 1);
                break;
            case 0:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid selection! Try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Critical Error: " << e.what() << endl;
        }
    }
    return 0;
}