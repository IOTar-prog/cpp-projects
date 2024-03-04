#include <iostream>
#include <mutex>
#include <thread>
#include <time.h>
#include <vector>

using namespace std;
mutex mut;

//Функция для создания массива
void arr_create(unsigned long long* arr, unsigned long long size) {
	for (unsigned long long i = 0; i < size; i++) {
		arr[i] = 1;
	}
}

//Подсчёт суммы элементов массива с начального по конечный индекс
void sum(unsigned long long& result, unsigned long long* arr, unsigned long long begin_index, unsigned long long end_index) {
	for (unsigned long long i = begin_index; i < end_index; i++) {
		mut.lock();
		result += arr[i];
		mut.unlock();
	}
}

int main() {

	unsigned long long arr_size = 200;
	unsigned long long result = 0;
	int num_of_thrd = 10;

	//Создаём массив
	unsigned long long* arr = new unsigned long long[arr_size];
	arr_create(arr, arr_size);

	//Определяем количество потоков
	/*
	cout << "Num of threads: ";
	cin >> num_of_thrd;
	*/

	//проверяем остаток от деления числа элементов массива на кол-во потоков
	if (arr_size % num_of_thrd != 0) {
		int n = sizeof(arr) / sizeof(arr[0]);
		for (int i = 0; i < arr_size % num_of_thrd; i++) {
			result += arr[n - 1];
		}

		for (int i = 0; i < (sizeof(arr) / sizeof(arr[0])); i++) {
			arr_size -= 1;
		}
	}

	vector<thread> threads;
	for (int i = 0; i < num_of_thrd; i++) {

		if (i == 0) {
			threads.emplace_back(thread(sum, std::ref(result), arr, 0, (arr_size / num_of_thrd)));
		}
		else {
			unsigned long long start = (arr_size / num_of_thrd) * i;
			unsigned long long end_id = (start + arr_size / num_of_thrd);

			threads.emplace_back(thread(sum, std::ref(result), arr, start, end_id));
		}

	}

	for (int i = 0; i < num_of_thrd; ++i) {
		threads.at(i).join();
	}

	delete[] arr;
	cout << result;

}