#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;
using namespace chrono;

// Insertion Sort
void insertionSort(int* data, int size) {
    for (int i = 1; i < size; ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            --j;
        }
        data[j + 1] = key;
    }
}

// Quick Sort
int partition(int* data, int low, int high) {
    int pivot = data[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (data[j] <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);
    return i + 1;
}

void quickSort(int* data, int size) {
    stack<pair<int, int>> s;
    s.push(make_pair(0, size - 1));
    while (!s.empty()) {
        int low = s.top().first;
        int high = s.top().second;
        s.pop();
        if (low < high) {
            int pi = partition(data, low, high);
            s.push(make_pair(low, pi - 1));
            s.push(make_pair(pi + 1, high));
        }
    }
}

// Counting Sort
void countingSort(int* data, int size) {
    const int MAX_VAL = 1000;
    int* count = new int[MAX_VAL + 1]();
    int* output = new int[size];
    for (int i = 0; i < size; ++i)
        ++count[data[i]];
    int index = 0;
    for (int i = 1; i <= MAX_VAL; ++i)
        while (count[i]-- > 0)
            output[index++] = i;
    for (int i = 0; i < size; ++i)
        data[i] = output[i];
    delete[] count;
    delete[] output;
}

// Heap Sort
void heapify(int* data, int size, int i) {
    int largest = i;
    int l = 2*i + 1, r = 2*i + 2;
    if (l < size && data[l] > data[largest]) largest = l;
    if (r < size && data[r] > data[largest]) largest = r;
    if (largest != i) {
        swap(data[i], data[largest]);
        heapify(data, size, largest);
    }
}

void heapSort(int* data, int size) {
    for (int i = size/2 - 1; i >= 0; i--)
        heapify(data, size, i);
    for (int i = size - 1; i > 0; i--) {
        swap(data[0], data[i]);
        heapify(data, i, 0);
    }
}

// Generate random data in range [1, 1000]
void generateDataFile(const string& filename, int totalCount) {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "  Error opening file for writing.\n";
        return;
    }

    srand(time(0));
    for (int i = 0; i < totalCount; ++i) {
        int val = rand() % 1000 + 1;   
        file << val << " ";
    }
    file.close();
    cout << "  100000 random numbers written to file.\n";
}

 void loadDataFromFile(const string& filename, int* data, int size) {
    ifstream file(filename);
    if (!file) {
        cerr << " Error opening file for reading.\n";
        return;
    }

    for (int i = 0; i < size; ++i) {
        file >> data[i];
    }
    file.close();
}

// Function to test performance of a sorting algorithm
 void testSortPerformance(void (*sortFunc)(int*, int), const string& sortName, int* originalData, int size) {
    int* data = new int[size];
    copy(originalData, originalData + size, data);
    auto startTime = high_resolution_clock::now();
    sortFunc(data, size);
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime).count();
    cout << sortName << " \t --> " << duration << " microseconds" << endl;
    delete[] data;
}

int main() {
    
    string filename = "big_random_data.txt";
    const int TOTAL_NUMBERS = 100000;

   
    generateDataFile(filename, TOTAL_NUMBERS);

    
    int sizes[10] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};

    cout << "\n< Testing for Incremental Data Sizes >\n";

    for (int i = 0; i < 10; ++i) {
        int size = sizes[i];
        int* data = new int[size];

        loadDataFromFile(filename, data, size);

        cout << "\n--- Testing for Size: " << size << " ---\n\n";
        testSortPerformance(insertionSort, "Insertion Sort", data, size);
        testSortPerformance(quickSort, "Quick Sort", data, size);
        testSortPerformance(countingSort, "Counting Sort", data, size);
        testSortPerformance(heapSort, "Heap Sort", data, size);
        cout << "----------------------------------------------------\n";

        delete[] data;
    }

    return 0;
}