
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

constexpr size_t size = 1000000000ull; 
using iter = std::vector<int>::iterator;

std::vector<int> populateVectorRandomly()
{

	std::random_device randomDevice;
	std::mt19937 rng(randomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> randDist(1, 5);


	std::vector<int> vector;
	vector.reserve(size);

	for (size_t i = 0; i < size; ++i)
		vector.emplace_back(randDist(rng));
	return vector;
}



void maxmin(int& max, int& min, iter begin, iter end)
{
	max = 0;
	min = 0;
	for (auto it = begin; it != end; ++it)
	{
		if (*it > max)
			max = *it;
		if (*it < min)
			min = *it;
	}
}

void printVector(iter begin, iter end)
{
	std::cout << "Vector: ";
	for (auto it = begin; it != end; ++it)
	{
		std::cout << *it << ", ";
	}
	std::cout << std::endl;
}

void testThread()
{
	std::vector<int> myVec(size, 1);// = populateVectorRandomly();
	//printVector(myVec.begin(), myVec.end());
	int maxA = 0, maxB = 0, maxC = 0, maxD = 0;
	int minA = 0, minB = 0, minC = 0, minD = 0;


	iter iter_begin = myVec.begin();
	iter iter_end = myVec.end();
	iter iter_1_4 = myVec.begin();
	iter iter_half = myVec.begin();
	iter iter_3_4 = myVec.begin();
	std::advance(iter_1_4, (int)size / 4);
	std::advance(iter_half, (int)size / 2);
	std::advance(iter_3_4, 3 * ((int)size / 4));

	auto start = std::chrono::system_clock::now();

	std::thread t1(maxmin, std::ref(maxA), std::ref(minA), iter_begin, iter_1_4);
	std::thread t2(maxmin, std::ref(maxB), std::ref(minB), iter_1_4, iter_half);
	std::thread t3(maxmin, std::ref(maxC), std::ref(minC), iter_half, iter_3_4);
	std::thread t4(maxmin, std::ref(maxD), std::ref(minD), iter_3_4, iter_end);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	auto end = std::chrono::system_clock::now();

	std::cout << "Time for 4 threads:" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	std::cout << "Result on 4 threads: MAX: " << std::max({ maxA, maxB, maxC, maxD}) << " MIN: " << std::max({ minA, minB, minC, minD}) << std::endl;

	int max = 0; int min = 0;
	auto start2 = std::chrono::system_clock::now();
	maxmin(max, min, iter_begin, iter_end);
	auto end2 = std::chrono::system_clock::now();

	std::cout << "Time for 4 threads:" << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << std::endl;

	std::cout << "Result on 4 threads: MAX: " << max << " MIN: " << min << std::endl;
}

