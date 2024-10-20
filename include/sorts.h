#ifndef _SORTS_H
#define _SORTS_H

#include <utility>
#include <queue>

template <class T> 
void bubbleSort(T* array, int n)
{
	for (int i = 0; i < n - 1; ++i)
		for (int j = i + 1; j < n; ++j)
			if (array[i] > array[j])
				std::swap(array[i], array[j]);
};

template <class T> 
void selectSort(T* array, int n)
{
	int maxIndex;
	for (int i = 0; i < n - 1; ++i)
	{
		maxIndex = i;
		for (int j = i + 1; j < n; ++j)
		{
			if (array[j] < array[maxIndex])
				maxIndex = j;
		}
		std::swap(array[i], array[maxIndex]);
	}
};

template <typename T>
std::queue<T> mergeQueues(const std::queue<T>& q1, const std::queue<T>& q2) {
    std::queue<T> result;
    while (!q1.empty() || !q2.empty()) 
	{
        if (!q1.empty() && (!q2.empty() || q1.front() <= q2.front())) 
		{
            result.push(q1.back());
        } 
		else 
		{
            result.push(q2.back());
        }
    }
    return result;
}

template <typename T>
void mergeSort(std::queue<T>& queue)
{
	int mid = queue.size() / 2;
	std::queue<T> leftQueue;
	std::queue<T> rightQueue;

	for (int i = 0; i < mid; ++i) 
	{
        leftQueue.push(queue.back());
		queue.pop();
    }

    for (int i = 0; i < queue.size() + mid; ++i) 
	{
        rightQueue.push(queue.back());
		queue.pop();
    }

	mergeSort(leftQueue);
    mergeSort(rightQueue);

	std::queue<T> merged = mergeQueues(leftQueue, rightQueue);

    while (!merged.empty()) 
	{
        queue.push(merged.back());
    }
};

#endif // _SORTS_H