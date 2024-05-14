#include <iostream>
#include <vector>
#include <cstdlib> // For srand and rand functions
#include <ctime>   // For time function
#include <climits> // For INT_MIN
#include <algorithm> // For std::max

using namespace std;

//---------------------------o(n)---------------------------

// Function to generate a random integer between min and max (inclusive)
int randomInt() {
    return -25 + rand() % (74 - (-25) + 1);
}

int maxSubArray_n(int size, vector<int> nums) {
    int max_so_far = nums[0];
    int max_ending_here = nums[0];

    for (size_t i = 1; i < size; ++i) {
        max_ending_here = max(nums[i], max_ending_here + nums[i]);
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

//---------------------------o(n^2)---------------------------
int maxSubArray_n2(int size,const vector<int>& nums) {
    int max_sum = INT_MIN;

    for (size_t i = 0; i < size; ++i) {
        int current_sum = 0;
        for (size_t j = i; j < size; ++j) {
            current_sum += nums[j];
            max_sum = max(max_sum, current_sum);
        }
    }

    return max_sum;
}

//---------------------------o(n^3)---------------------------

int maxSubArray_n3(int size,const vector<int>& nums) {
    int max_sum = INT_MIN;

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {
            int current_sum = 0;
            for (size_t k = i; k <= j; ++k) {
                current_sum += nums[k];
            }
            max_sum = std::max(max_sum, current_sum);
        }
    }

    return max_sum;
}

//---------------------------main---------------------------
int main(int argc,char* argv[]) {
    if(argc != 2){
        cout << "Usage: " << argv[0] << " <size>" << endl;
        return 1;
    }
    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(0)));
    int size = atoi(argv[1]);
    vector<int> arr(static_cast<std::size_t>(size));
    
    //vector<int> random_seed = (vector<int>)*argv[2];
    for(size_t i = 0; i < size; i++){
        //int random_num = rand()%100-25;
        arr[i] = randomInt();
    }

    // Find and print the maximum subarray sum
    cout << "Maximum subarray sum - o(n): " << maxSubArray_n(size,arr) << endl;
    cout << "Maximum subarray sum - o(n^2): " << maxSubArray_n2(size,arr) << endl;
    cout << "Maximum subarray sum - o(n^3): " << maxSubArray_n3(size,arr) << endl;

    return 0;
}