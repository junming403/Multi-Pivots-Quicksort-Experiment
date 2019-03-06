#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <ctime>

using namespace std;
bool debug = 0;
long long numSwap;
long long numCompare;

typedef vector<int> vi;

void printVector(vi arr, int start, int end) {
    for (int i = start; i < end; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

inline void swap(vi &arr, int i, int j) {
    numSwap++;
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

inline bool isSmall(int a, int b) {
    numCompare++;
    return a < b;
}

inline int partition(vi &arr, int start, int end) {
    int pivotIndex = start, pivot = arr[start];
    if (debug) {
        printf("pivot: %d\n", pivot);
        cout << "before partition: ";
        printVector(arr, start, end);
    }
    for (int i = start + 1; i < end; i++) {
        if (isSmall(arr[i], pivot)) {
            pivotIndex++;
            swap(arr, i, pivotIndex);
        }

        if (debug) {
            cout << "after round " << i - start << ": ";
            printVector(arr, start, end);
        }
    }
    swap(arr, pivotIndex, start);
    if (debug) {
        cout << "pivotIndex: " << pivotIndex - start << endl;
        cout << "after partition: ";
        printVector(arr, start, end);
        cout << endl;
    }
    return pivotIndex;
}

void quickSort(vi &arr, int start, int end) {
    if (end - 1 <= start) {
        return;
    }

    if (end - start < 50) {
        for (int i = start + 1; i < end; i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (isSmall(arr[j + 1], arr[j])) {
                    swap(arr, j + 1, j);
                } else {
                    break;
                }
            }
        }
        return;
    }

    int pivotIndex = rand() % (end - start) + start;
    swap(arr, pivotIndex, start);
    pivotIndex = partition(arr, start, end);

    quickSort(arr, start, pivotIndex);
    quickSort(arr, pivotIndex + 1, end);

}

vi getCopy(vi &arr) {
    vi copy;
    for (int i = 0; i < arr.size(); i++) {
        copy.push_back(arr[i]);
    }
    return copy;
}

void verify(int testTimes, int data_size) {
    vi dataV;
    for (int i = 0; i < data_size; i++) {
        dataV.push_back(rand() % data_size);
    }
    vi copy = getCopy(dataV);

    clock_t begin = clock();
    sort(copy.begin(), copy.end());
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "BENCHMARK: [" << elapsed_secs << "s]" << endl;

    for (int i = 1; i <= testTimes; i++) {
        numSwap = 0;
        numCompare = 0;
        copy = getCopy(dataV);
        begin = clock();
        quickSort(copy, 0, data_size);
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        for (int i = 0; i < data_size - 1; i++) {
            if (copy[i] > copy[i + 1]) {
                cout << "FAILED!" << endl;
                return;
            }
        }
        cout << "[" << i << "]" << " PASSED WITH [" << elapsed_secs << "s]" << endl;
        printf("Number of swaps: %lld\n\n", numSwap);
        printf("Number of comparasions: %lld\n\n", numCompare);
    }
}

int main() {
    srand(time(NULL));
    verify(1, 20000000);
}


