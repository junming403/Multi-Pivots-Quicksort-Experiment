#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <ctime>

using namespace std;

typedef vector<int> vi;

class Pair
{
public:
    int index, value;
    Pair(int i, int v) {
        index = i;
        value = v;
    }
    bool operator<(Pair &other)
    {
        return value < other.value;
    }
};

void printVector(vi arr) {
    for (int i = 0; i < arr.size(); i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

inline void swap(vi &arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int binary_search(vector<Pair> pivotPairs, int val, int start, int end) {
    if (end == start) {
        if (pivotPairs[end].value < val) {
            return pivotPairs.size();
        } else {
            return end;
        }
    }
    int mid = (end + start) / 2;

    if (pivotPairs[mid].value > val) {
        return binary_search(pivotPairs, val, start, mid);
    } else {
        return binary_search(pivotPairs, val, mid + 1, end);
    }
}

void quickSort(vi &arr, int numPivot, int start, int end) {
    if (end <= start) {
        return;
    }

    if (end - start + 1 <= numPivot) {
        numPivot = 1;
    }

    vector<Pair> pivotPairs;

    for (int i = 0; i < numPivot; i++) {
        int nextPivotIndex = rand() % (end - start - i) + start + i;
        swap(arr, nextPivotIndex, start + i);

        int cur = start + i - 1;
        while(cur >= start && arr[cur] > arr[cur + 1]) {
            swap(arr, cur, cur + 1);
            cur--;
        }
    }

    for (int i = start; i < start + numPivot; i++) {
        pivotPairs.push_back(Pair(i, arr[i]));
    }


    for (int i = start + numPivot; i < end; i++) {
//        int indexInPivotPairs = binary_search(pivotPairs, arr[i], 0, pivotPairs.size() - 1);
        int indexInPivotPairs = pivotPairs.size();
        while (indexInPivotPairs - 1 >= 0 && pivotPairs[indexInPivotPairs - 1].value >= arr[i]) {
            indexInPivotPairs--;
        }

        while (indexInPivotPairs < pivotPairs.size()) {
            swap(arr, i, pivotPairs[indexInPivotPairs].index + 1);
            if (!(indexInPivotPairs > 0 && pivotPairs[indexInPivotPairs].index == pivotPairs[indexInPivotPairs - 1].index)) {
                swap(arr, pivotPairs[indexInPivotPairs].index, pivotPairs[indexInPivotPairs].index + 1);
            }
            pivotPairs[indexInPivotPairs].index++;
            indexInPivotPairs++;
        }
    }

    int startPoint = start;
    for (int i = 0; i < pivotPairs.size(); i++) {
        int endIndex = pivotPairs[i].index;
        quickSort(arr, numPivot, startPoint, endIndex);
        startPoint = pivotPairs[i].index + 1;
    }

    quickSort(arr, numPivot, startPoint, end);
}

vi getCopy(vi &arr) {
    vi copy;
    for (int i = 0; i < arr.size(); i++) {
        copy.push_back(arr[i]);
    }
    return copy;
}

void verify(int piviotsRange, int data_size, bool isDebug = false) {
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

    for (int i = 1; i <= piviotsRange; i++) {
        copy = getCopy(dataV);
        begin = clock();
        quickSort(copy, i, 0, data_size);
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        for (int i = 0; i < data_size - 1; i++) {
            if (copy[i] > copy[i + 1]) {
                cout << "FAILED!" << endl;
                return;
            }
        }
        cout << "[" << i << "]" << " PASSED WITH [" << elapsed_secs << "s]" << endl;
    }
}

int main() {
    srand(time(NULL));
    verify(15, 20000000);
}