#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <ctime>

using namespace std;


typedef vector<int> vi;

bool debug = 0;
const int MAX_PIVOTS = 30;


void printVector(vi arr, int start, int end) {
    for (int i = start; i < end; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

long long swapCount = 0;
long long compareCount = 0;

inline void swap(vi &arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    swapCount++;
}

inline bool isSmall(int a, int b) {
    compareCount++;
    return a < b;
}

void quickSort(vi &arr, int numPivot, int start, int end) {
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

    int pivots[MAX_PIVOTS] = {0};
    for (int i = 0; i < numPivot; i++) {
        int nextPivotIndex = rand() % (end - start - i) + start + i;
        swap(arr, nextPivotIndex, start + i);

        int cur = start + i - 1;
        while(cur >= start && isSmall(arr[cur + 1], arr[cur])) {
            swap(arr, cur, cur + 1);
            cur--;
        }
    }

    for (int i = 0; i < numPivot; i++) {
        pivots[i] = arr[start + i];
    }

    int nextIndex = end - 1;
    for (int i = start + numPivot - 1; i >= start + (numPivot+1)/2; i--) {
        swap(arr, i, nextIndex);
        nextIndex--;
    }

    int rightLeastIndex = (numPivot + 1) / 2;
    int leftMostIndex = (numPivot - 1) / 2;

    int pivotIndexes[MAX_PIVOTS] = {0};
    int halfPivotNumber = (numPivot + 1) / 2;


    for (int i = 0; i < halfPivotNumber; i++) {
        pivotIndexes[i] = start + halfPivotNumber - 1;
    }
    for (int i = halfPivotNumber; i < numPivot; i++) {
        pivotIndexes[i] = end - numPivot / 2;
    }

    int cur = pivotIndexes[leftMostIndex] + 1;
    while (cur < pivotIndexes[rightLeastIndex]) {
        int indexInPivotPairs = numPivot;
        while (indexInPivotPairs - 1 >= 0 && !isSmall(pivots[indexInPivotPairs - 1], arr[cur])) {
            indexInPivotPairs--;
        }

       if (indexInPivotPairs < rightLeastIndex) {
            int curPosition = cur;
            for (int j = halfPivotNumber - 1; j >= indexInPivotPairs; j--) {
                pivotIndexes[j]++;
                swap(arr, curPosition, pivotIndexes[j]);
                curPosition = pivotIndexes[j];
            }
            cur++;
        } else if (indexInPivotPairs > rightLeastIndex) {
            int curPosition = cur;
            for (int j = halfPivotNumber; j < indexInPivotPairs; j++) {
                pivotIndexes[j]--;
                swap(arr, curPosition, pivotIndexes[j]);
                curPosition = pivotIndexes[j];
            }
        } else {
            cur++;
        }
    }
    for (int i = halfPivotNumber - 1; i >= 0; i--) {
        int prevIndex = -1;
        for (int j = i; j >= 0; j--) {
            if (pivotIndexes[j] == pivotIndexes[prevIndex]) {
                continue;
            }
            swap(arr, i + start, pivotIndexes[j]);
            prevIndex = j;
        }
        for (int j = i - 1; j >= 0; j--) {
            pivotIndexes[j]--;
        }
    }

    for (int i = halfPivotNumber; i < numPivot; i++) {
        int prevIndex = -1;
        for (int j = i; j < numPivot; j++) {
            if (pivotIndexes[j] == pivotIndexes[prevIndex]) {
                continue;
            }
            swap(arr, end - numPivot + i, pivotIndexes[j]);
            prevIndex = j;
        }
        for (int j = i + 1; j < numPivot; j++) {
            pivotIndexes[j]++;
        }
    }
    int startPoint = start;
    for (int i = 0; i < numPivot; i++) {
        int endIndex = pivotIndexes[i];
        quickSort(arr, numPivot, startPoint, endIndex);
        startPoint = pivotIndexes[i];
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

void verify(int piviotsRange, int data_size, int step = 1, int times = 1) {
    vi dataV;
    int data_range = data_size * 10;
    for (int i = 0; i < data_size; i++) {
        dataV.push_back(rand() % data_range);
    }

    double elapsed_secs = 0.0;
    for (int i = 0; i < times; i++) {
        vi copy = getCopy(dataV);

        clock_t begin = clock();
        sort(copy.begin(), copy.end());
        clock_t end = clock();

        elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
    }

    elapsed_secs /= (times * 1.0);

    cout << "BENCHMARK: [" << elapsed_secs << "s]" << endl;

    for (int i = 2; i <= piviotsRange; i+=step) {
        elapsed_secs = 0.0;
        swapCount = 0;
        compareCount = 0;
        for (int k = 0; k < times; k++) {
            vi copy = getCopy(dataV);

            clock_t begin = clock();
            if (debug) printVector(copy, 0, data_size);
            quickSort(copy, i, 0, data_size);
            clock_t end = clock();

            elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;

            for (int i = 0; i < data_size - 1; i++) {
                if (copy[i] > copy[i + 1]) {
                    cout << "FAILED!" << endl;
                    return;
                }
            }
        }
        elapsed_secs /= (times * 1.0);
        swapCount /= times;


//        cout << "[" << i << "]" << " PASSED WITH [" << elapsed_secs << "s]" << endl;
//        cout << "(" << i << "," << elapsed_secs << ") ";
//		cout << "(" << i << "," << swapCount << ") ";
//		cout << "(" << i << "," << swapCount << ") ";
		cout << "(" << i << "," << compareCount << ") ";

    }
}


int main() {
    srand(time(NULL));
    verify(MAX_PIVOTS, 20000000, 1);
}
