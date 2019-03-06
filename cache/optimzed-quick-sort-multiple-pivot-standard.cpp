#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <map>

using namespace std;

class Node {
public:
    int key, value;
    Node *prev, *next;
    Node(int k, int v): key(k), value(v), prev(NULL), next(NULL) {}
};

class DoublyLinkedList {
    Node *front, *rear;

    bool isEmpty() {
        return rear == NULL;
    }

public:
    DoublyLinkedList(): front(NULL), rear(NULL) {}

    Node* add_page_to_head(int key, int value) {
        Node *page = new Node(key, value);
        if(!front && !rear) {
            front = rear = page;
        }
        else {
            page->next = front;
            front->prev = page;
            front = page;
        }
        return page;
    }

    void move_page_to_head(Node *page) {
        if(page==front) {
            return;
        }
        if(page == rear) {
            rear = rear->prev;
            rear->next = NULL;
        }
        else {
            page->prev->next = page->next;
            page->next->prev = page->prev;
        }

        page->next = front;
        page->prev = NULL;
        front->prev = page;
        front = page;
    }

    void remove_rear_page() {
        if(isEmpty()) {
            return;
        }
        if(front == rear) {
            delete rear;
            front = rear = NULL;
        }
        else {
            Node *temp = rear;
            rear = rear->prev;
            rear->next = NULL;
            delete temp;
        }
    }
    Node* get_rear_page() {
        return rear;
    }

};

class LRUCache{
    int capacity, size;
    DoublyLinkedList *pageList;
    map<int, Node*> pageMap;

public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        size = 0;
        pageList = new DoublyLinkedList();
        pageMap = map<int, Node*>();
    }

    void clear(int capacity) {
        this->capacity = capacity;
        size = 0;
        pageList = new DoublyLinkedList();
        pageMap = map<int, Node*>();
    }

    int get(int key) {
        if(pageMap.find(key)==pageMap.end()) {
            return -1;
        }
        int val = pageMap[key]->value;

        // move the page to front
        pageList->move_page_to_head(pageMap[key]);
        return val;
    }

    void put(int key, int value) {
        if(pageMap.find(key)!=pageMap.end()) {
            // if key already present, update value and move page to head
            pageMap[key]->value = value;
            pageList->move_page_to_head(pageMap[key]);
            return;
        }

        if(size == capacity) {
            // remove rear page
            int k = pageList->get_rear_page()->key;
            pageMap.erase(k);
            pageList->remove_rear_page();
            size--;
        }

        // add new page to head to Queue
        Node *page = pageList->add_page_to_head(key, value);
        size++;
        pageMap[key] = page;
    }

    ~LRUCache() {
        map<int, Node*>::iterator i1;
        for(i1=pageMap.begin();i1!=pageMap.end();i1++) {
            delete i1->second;
        }
        delete pageList;
    }
};


typedef vector<int> vi;

bool debug = 0;
const int MAX_PIVOTS = 30;


int BUFFER_SIZE = 5;
const int BLOCK_SIZE = 100;
const int INPUT_SIZE = 50000;

LRUCache input_cache(BUFFER_SIZE);

int accesses = 0;
int accessElement(vi arr, int i) {
    int page_index = i / BLOCK_SIZE;
    if (input_cache.get(page_index) == -1) {
        accesses++;
        input_cache.put(page_index, 1);
    }
    return arr[i];
}


inline void swap(vi &arr, int i, int j) {
    int temp = accessElement(arr, i);
    arr[i] = accessElement(arr, j);
    arr[j] = temp;
}

void quickSort(vi &arr, int numPivot, int start, int end) {
    if (end - 1 <= start) {
        return;
    }

    if (end - start < 50) {
        for (int i = start + 1; i < end; i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (accessElement(arr, j + 1) < accessElement(arr, j)) {
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
        while(cur >= start && accessElement(arr, cur) > accessElement(arr, cur + 1)) {
            swap(arr, cur, cur + 1);
            cur--;
        }
    }

    for (int i = 0; i < numPivot; i++) {
        pivots[i] = accessElement(arr, start + i);
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
        while (indexInPivotPairs - 1 >= 0 && pivots[indexInPivotPairs - 1] >= accessElement(arr, i)) {
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

inline int partition(vi &arr, int start, int end) {
    int pivotIndex = start, pivot = accessElement(arr, start);
    for (int i = start + 1; i < end; i++) {
        if (accessElement(arr, i) < pivot) {
            pivotIndex++;
            swap(arr, i, pivotIndex);
        }
    }
    swap(arr, pivotIndex, start);
    return pivotIndex;
}

void quickSort_one_pivot(vi &arr, int start, int end) {
    if (end - 1 <= start) {
        return;
    }

    if (end - start < 50) {
        for (int i = start + 1; i < end; i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (accessElement(arr, j + 1) < accessElement(arr, j)) {
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

    quickSort_one_pivot(arr, start, pivotIndex);
    quickSort_one_pivot(arr, pivotIndex + 1, end);
}

void verify(int piviotsRange, int data_size) {
    vi dataV;
    int data_range = data_size * 10;
    for (int i = 0; i < data_size; i++) {
        dataV.push_back(rand() % data_range);
    }

    for (int i = 1; i <= piviotsRange; i++) {
        accesses = 0;
        input_cache.clear(BUFFER_SIZE);
        vi copy = getCopy(dataV);

        if (i == 1) {
            quickSort_one_pivot(copy, 0, data_size);
        } else {
            quickSort(copy, i, 0, data_size);
        }

//        cout << "[" << i << "]" << " PASSED WITH [" << accesses << " I/O costs]" << endl;
        cout << "(" << i << "," << accesses << ") ";

    }
}


int main() {
    srand(time(NULL));
    for (int i = 20; i <= 25; i+=5) {
        BUFFER_SIZE = i;
        cout << "BUFFER SIZE " << BUFFER_SIZE << endl;
        verify(MAX_PIVOTS, INPUT_SIZE);
        cout << endl;
    }
}
