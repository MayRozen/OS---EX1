#include <iostream>
#include <vector>
#include <cstdlib> // For srand and rand functions
#include <ctime>   // For time function

#define INT_MIN -25

//---------------------------o(n)---------------------------

// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

int maxSubArray_n(const std::vector<int>& nums) {
    int max_so_far = nums[0];
    int max_ending_here = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        max_ending_here = std::max(nums[i], max_ending_here + nums[i]);
        max_so_far = std::max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

//---------------------------o(n^2)---------------------------
int maxSubArray_n2(const std::vector<int>& nums) {
    int n = nums.size();
    int max_sum = INT_MIN;

    for (int i = 0; i < n; ++i) {
        int current_sum = 0;
        for (int j = i; j < n; ++j) {
            current_sum += nums[j];
            max_sum = std::max(max_sum, current_sum);
        }
    }

    return max_sum;
}

//---------------------------o(n^3)---------------------------

int maxSubArray_n3(const std::vector<int>& nums) {
    int n = nums.size();
    int max_sum = INT_MIN;

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            int current_sum = 0;
            for (int k = i; k <= j; ++k) {
                current_sum += nums[k];
            }
            max_sum = std::max(max_sum, current_sum);
        }
    }

    return max_sum;
}

//---------------------------main---------------------------
int main(int size,std::vector<int>  random_seed) {
    // Seed the random number generator with the current time
    srand(time(nullptr));

    for(int i=0; i<size; i++){
        int random_num = rand()%100-25;
        std::cout<<random_num<<std::endl;
    }

    // Find and print the maximum subarray sum
    std::cout << "Maximum subarray sum - o(n): " << maxSubArray_n(random_seed) << std::endl;
    std::cout << "Maximum subarray sum - o(n^2): " << maxSubArray_n2(random_seed) << std::endl;
    std::cout << "Maximum subarray sum - o(n^3): " << maxSubArray_n3(random_seed) << std::endl;

    return 0;
}