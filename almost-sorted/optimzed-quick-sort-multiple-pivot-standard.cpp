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
//    swapCount++;
}

void quickSort(vi &arr, int numPivot, int start, int end) {
    if (end - 1 <= start) {
        return;
    }

    if (end - start < 50) {
        for (int i = start + 1; i < end; i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (arr[j + 1] < arr[j]) {
                    swap(arr, j + 1, j);
                } else {
                    break;
                }
            }
        }
        return;
    }

//    if (end - start <= numPivot) {
//        numPivot = 2;
//    }

    int pivots[MAX_PIVOTS] = {0};
    for (int i = 0; i < numPivot; i++) {
        int nextPivotIndex = rand() % (end - start - i) + start + i;
        swap(arr, nextPivotIndex, start + i);

        int cur = start + i - 1;
        while(cur >= start && arr[cur] > arr[cur + 1]) {
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

    int i = pivotIndexes[leftMostIndex] + 1;
    while (i < pivotIndexes[rightLeastIndex]) {
        int indexInPivotPairs = numPivot;
        while (indexInPivotPairs - 1 >= 0 && pivots[indexInPivotPairs - 1] >= arr[i]) {
            indexInPivotPairs--;
        }

       if (indexInPivotPairs < rightLeastIndex) {
            int curPosition = i;
            for (int j = halfPivotNumber - 1; j >= indexInPivotPairs; j--) {
                pivotIndexes[j]++;
                swap(arr, curPosition, pivotIndexes[j]);
                curPosition = pivotIndexes[j];
            }
            i++;
        } else if (indexInPivotPairs > rightLeastIndex) {
            int curPosition = i;
            for (int j = halfPivotNumber; j < indexInPivotPairs; j++) {
                pivotIndexes[j]--;
                swap(arr, curPosition, pivotIndexes[j]);
                curPosition = pivotIndexes[j];
            }
        } else {
            i++;
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

void verify(int piviotsRange, int data_size, double sorted_rate = 0, int times = 1) {
    vi dataV;
    int data_range = data_size * 10;
    for (int i = 0; i < data_size; i++) {
        dataV.push_back(rand() % data_range);
    }
    sort(dataV.begin(), dataV.end());
    for (int k = 0; k < data_size * sorted_rate; k++) {
        int i = rand() % data_size;
        int j = rand() % data_size;
        swap(dataV, i, j);
    }


    for (int i = 2; i <= piviotsRange; i++) {
        double elapsed_secs = 0.0;
        for (int k = 0; k < times; k++) {
            vi copy = getCopy(dataV);

            clock_t begin = clock();
            quickSort(copy, i, 0, data_size);
            clock_t end = clock();

            elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;

//            for (int i = 0; i < data_size - 1; i++) {
//                if (copy[i] > copy[i + 1]) {
//                    cout << "FAILED!" << endl;
//                    return;
//                }
//            }
        }

        elapsed_secs = elapsed_secs / (times * 1.0);

//      cout << "[" << i << "]" << " PASSED WITH [" << elapsed_secs << "s]" << endl;
      cout << "(" << i << "," << elapsed_secs << ") ";
//		cout << "(" << i << "," << swapCount << ") ";
//		cout << "(" << i << "," << compareCount << ") ";
    }


}


int main() {
    srand(time(NULL));
//    for (double unsorted_rate = 0.0; unsorted_rate < 0.5; unsorted_rate+=0.05) {
//        verify(MAX_PIVOTS, 20000000, unsorted_rate);
//    }
    verify(MAX_PIVOTS, 20000000, 0.25);
}