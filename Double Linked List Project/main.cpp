#include <assert.h>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <list>
#include <vector>

#include "dlist.h"
#include "llist.h"

#define ARRSIZE 1000

template <class Iter>
void selectionSort(Iter, Iter);	// implement

int main(int argc, char** argv) {
	// generate random array
	srand (time(NULL));
	int arr[ARRSIZE];
	for (int i = 0; i < ARRSIZE; i++) {
		arr[i] = rand() % 100;
	}

	// vector
	std::vector<int> vec, vec2;
	for (int i = 0; i < ARRSIZE; i++) vec.push_back(arr[i]);
	vec2 = vec;
	selectionSort(vec.begin(), vec.end());

	// singly linked list
	List<int> lst;
	for (int i = 0; i < ARRSIZE; i++) lst.push_front(arr[i]);
	selectionSort(lst.begin(), lst.end());

	// doubly linked list
	DList<int> dlst;
	for (int i = 0; i < ARRSIZE; i++) dlst.push_front(arr[i]);
	selectionSort(dlst.begin(), dlst.end());

	// check sort
	std::sort(vec2.begin(), vec2.end());
	assert(vec==vec2);

	std::list<int> stlist;
	for (int i = 0; i < ARRSIZE; i++) { stlist.push_front(arr[i]); }
	stlist.sort();

	auto dit = dlst.begin();
	auto stlit = stlist.begin();
	while (dit != dlst.end() && stlit != stlist.end()) {
		assert(*dit == *stlit);
		++dit; ++stlit;
	}
}

// Selection Sort, takes the first element and goes through all the elements to find the smallest value, values are swaped and so on
// The sort continues on with O(n^2) times.
template <class Iter>
void selectionSort(Iter begin, Iter end) {

    Iter itNext;
    for(Iter it = begin; it != end; ++it){
        Iter lowest = it;
        itNext = it;
        ++itNext;
        for(;itNext != end; ++itNext){
            if(*itNext < *lowest) lowest = itNext;
        }
        if(it != lowest){
            auto temp = *it;
            *it = *lowest;
            *lowest = temp;
        }
    }
}
