/* ============================================================
Tests findMin and findMax functions
Shikuan Huang
============================================================ */

#include <iostream>
#include "findMinMax.h"
using namespace std;

int main() {
	int array[10] = {9, 22, 41, 51, 33, 2, 425, 2, 4, 11};
	cout << "The minimum element is at index " << findMin(array, 10) 
		 << " and the maximum element is at index " << findMax(array, 10) << endl;
	
	return 0;
}
