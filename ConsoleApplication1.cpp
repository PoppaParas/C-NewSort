#include <stdio.h>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <map>
#include <string>
#include <functional>

using namespace std;
using namespace chrono;

using Clock = steady_clock::time_point;
using FormatType = map<string, double>;


struct ArrayReturn {
	double Duration;
	vector<int> Array;
};



duration<double> ReturnDuration( int Denominator) {
    // Perform the division at runtime to get the number of seconds.
    double seconds_value = static_cast<double>(1) / Denominator;

    // Create and return a duration with that value.
    return duration<double>(seconds_value);
}


static FormatType ClockFormats = {
    {"Nanoseconds",1000000000},
    {"Microseconds",1000000},
    {"Milliseconds",1000},
    {"Centiseconds",100},
    {"Decaseconds",10},
    {"Seconds",1}
};
static map<string, Clock> ClockMaps = {};
static int TotalArrays = 0;

Clock GetClock() {
    return high_resolution_clock::now();
}

Clock SetClock(string Index) {
    Clock CurrentTime = GetClock();

    ClockMaps[Index] = CurrentTime;

    return CurrentTime;

}

double GetPassed(string Index, double ReturnFormat) {
    Clock CurrentTime = GetClock();
    if (ClockMaps.find(Index) == ClockMaps.end()) return 0.0;
    
    Clock Timestamp = ClockMaps[Index];
    return (double)((CurrentTime - Timestamp).count()) / ReturnFormat; /// (double)ReturnFormat.count();
}


// Function to merge two sorted subarrays into a single sorted array
// arr[] is the main array
// left, mid, and right are indices to define the subarrays
// Subarray 1 is arr[left..mid]
// Subarray 2 is arr[mid+1..right]
void merge(vector<int>& arr, int left, int mid, int right) {
    // Calculate sizes of the two subarrays to be merged
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors (arrays) to hold the data of the subarrays
    vector<int> L(n1);
    vector<int> R(n2);

    // Copy data from the main array to the temporary arrays
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }

    // --- Merge the temporary arrays back into the main array arr[left..right] ---

    // Initial indices for the first, second, and merged subarrays
    int i = 0; // Initial index for the first subarray (L)
    int j = 0; // Initial index for the second subarray (R)
    int k = left; // Initial index for the merged subarray (in arr)

    // Compare elements from L and R and place the smaller one into arr
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy any remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// The main function that implements Merge Sort
// arr[] is the array to be sorted
// left is the starting index, and right is the ending index
void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) {
        return; // Base case: If the array has 0 or 1 elements, it's already sorted
    }

    // Find the middle point to divide the array into two halves
    int mid = left + (right - left) / 2;

    // Recursively call mergeSort for the two halves
    mergeSort(arr, left, mid);      // Sort the first half
    mergeSort(arr, mid + 1, right); // Sort the second half

    // Merge the sorted halves
    merge(arr, left, mid, right);
}

// Utility function to print the array
void printArray(vector<int>& arr) {
    cout << TotalArrays << ": ";
    TotalArrays++;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Driver code to test the implementation

ArrayReturn CreateArray(int Amount) {
	random_device rd;
	//    mt19937 is the Mersenne Twister engine, a high-quality standard.
	mt19937 gen(rd());
	auto start = high_resolution_clock::now();

	// 3. Define the distribution (the range of numbers you want).
	uniform_int_distribution<> distrib(0, 1000000000);


	vector<int> CurrentArray(Amount);


	for (int& num : CurrentArray)
	{
		// Call the distribution with the generator to get a number.
		num = distrib(gen);
	}
    
	auto end = high_resolution_clock::now();
	auto milliseconds = duration_cast<chrono::milliseconds>(end - start);
	double Seconds = double(milliseconds.count()) / 1000;

	ArrayReturn NewReturn;
	NewReturn.Array = CurrentArray;
	NewReturn.Duration = Seconds;
	return NewReturn;
}

int main() {
	vector<double> Durations = {};
	int ArrayAmount = 50;
    for (const pair<string,double>Object : ClockFormats) {
        cout << Object.first << ": " << Object.second << endl;
    }
    
    SetClock("StartThread");
	for (int i = 0; i < ArrayAmount; i++) {
		ArrayReturn NewReturn = CreateArray(100);
        //printArray(NewReturn.Array);
        NewReturn.Array.clear();
		Durations.push_back(NewReturn.Duration);
	}
	double Total = 0;
	double Average = 0;
	for (double Duration : Durations) {
		Total += Duration;
	}
	Average = Total / Durations.size();
	cout << ArrayAmount << " arrays generated. Time taken: " << GetPassed("StartThread",ClockFormats["Nanoseconds"]) << " seconds" << endl;
	cout << "Avg time taken: " << Average << " seconds" << endl;
	return 0;
}