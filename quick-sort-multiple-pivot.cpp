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
	if (end - 1 <= start) {
		return;
	}

	if (end - start <= numPivot) {
		numPivot = 2;
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

    int nextIndex = end - 1;
    for (int i = start + numPivot - 1; i >= start + (numPivot+1)/2; i--) {
	    swap(arr, i, nextIndex);
	    pivotPairs[i - start].index = nextIndex;
        nextIndex--;
	}

    int rightLeastIndex = (pivotPairs.size() + 1) / 2;
    int leftMostIndex = (pivotPairs.size() - 1) / 2;

    int i = pivotPairs[leftMostIndex].index + 1;
	while(i < pivotPairs[rightLeastIndex].index) {
        int indexInPivotPairs = pivotPairs.size();
		while (indexInPivotPairs - 1 >= 0 && pivotPairs[indexInPivotPairs - 1].value >= arr[i]) {
		    indexInPivotPairs--;
		}

        if (indexInPivotPairs < rightLeastIndex){
            while (indexInPivotPairs < rightLeastIndex) {
                swap(arr, i, pivotPairs[indexInPivotPairs].index + 1);
                if (!(indexInPivotPairs > 0 && pivotPairs[indexInPivotPairs].index == pivotPairs[indexInPivotPairs - 1].index)) {
                    swap(arr, pivotPairs[indexInPivotPairs].index, pivotPairs[indexInPivotPairs].index + 1);
                }
                pivotPairs[indexInPivotPairs].index++;
                indexInPivotPairs++;
            }
            if (i == pivotPairs[leftMostIndex].index) {
                i++;
            }
		} else if (indexInPivotPairs > rightLeastIndex){
		    indexInPivotPairs--;
		    while (indexInPivotPairs >= rightLeastIndex) {
                swap(arr, i, pivotPairs[indexInPivotPairs].index - 1);
                if (!(indexInPivotPairs < pivotPairs.size() - 1 && pivotPairs[indexInPivotPairs].index == pivotPairs[indexInPivotPairs + 1].index)) {
                    swap(arr, pivotPairs[indexInPivotPairs].index, pivotPairs[indexInPivotPairs].index - 1);
                }
                pivotPairs[indexInPivotPairs].index--;
                indexInPivotPairs--;
		    }
		} else {
		    i++;
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

void verify(int piviotsRange, int data_size, bool isDebug = false, int times=1) {
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

	for (int i = 2; i <= piviotsRange; i++) {
        elapsed_secs = 0.0;
        swapCount = 0;
	    for (int k = 0; k < times; k++) {
            vi copy = getCopy(dataV);

            clock_t begin = clock();
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


		cout << "[" << i << "]" << " PASSED WITH [" << elapsed_secs << "s]" << endl;
//		cout << "(" << i << "," << elapsed_secs << ") ";
//		cout << "(" << i << "," << swapCount << ") ";
//		cout << "(" << i << "," << compareCount << ") ";


	}
}

int main() {
	srand(time(NULL));
	verify(15, 20000000);
}


