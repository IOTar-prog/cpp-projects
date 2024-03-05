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

	unsigned long long arr_size = 2000022;
	unsigned long long result = 0;
	int num_of_thrd = 0;

	//Создаём массив
	unsigned long long* arr = new unsigned long long[arr_size];
	arr_create(arr, arr_size);

	//Определяем количество потоков
	cout << "Num of threads: ";
	cin >> num_of_thrd;


	//Проверяем остаток от деления числа элементов массива на кол-во потоков
	if (arr_size % num_of_thrd != 0) {
		int n = sizeof(arr) / sizeof(arr[0]);
		for (int i = 0; i < arr_size % num_of_thrd; i++) {
			result += arr[n - 1];
		}

		for (int i = 0; i < (sizeof(arr) / sizeof(arr[0])); i++) {
			arr_size -= 1;
		}
	}

	//Создаём контейнер для хранения потоков и параллельно считаем сумму
	vector<thread> threads;
	clock_t thrd_start_time = clock();
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
	clock_t thrd_end_time = clock();

	//Соединяем потоки
	for (int i = 0; i < num_of_thrd; ++i) {
		threads.at(i).join();
	}

	//Выводим результат и время выполнения параллельной функции
	cout << "Thread sum = " << result << " \n";
	double thrd_time_sec = (double)(thrd_end_time - thrd_start_time) / CLOCKS_PER_SEC;
	cout << "Thread time = " << thrd_time_sec << " \n";

	//Обнуляем результат и считаем стандартной функцией 
	result = 0;
	clock_t std_start_time = clock();
	sum(result, arr, 0, arr_size);
	clock_t std_end_time = clock();

	//Выводим результат и время выполнения стандартной функции
	cout << "std sum = " << result << " \n";
	double std_time_sec = (double)(std_end_time - std_start_time) / CLOCKS_PER_SEC;
	cout << "Std time = " << std_time_sec << " \n";

	delete[] arr;

}