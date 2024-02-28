#include <iostream>
#include <mutex>
#include <thread>
#include <time.h>

using namespace std;
mutex mut;

//Функция для создания массива
void arr(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i + 1;
	}
}

//Функция для подсёта суммы элементов массива
int arr_sum(int* arr, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	return sum;
}

//Подсчёт суммы элементов масива с начального по конечный индекс
void counter(int& sum, int* arr, int size, int begin_index, int end_index) {
	for (int i = begin_index; i < end_index; i++) {
		mut.lock();
		sum += arr[i];
		mut.unlock();
	}
}

int main()
{
	// Размер массива
	int arr_size = 100000;

	//Создаём массив
	int* a = new int[arr_size];
	arr(a, arr_size);

	//Замеряем время выполнения
	clock_t std_start = clock();
	//Считаем сумму массива
	cout << "Std_sum = " << arr_sum(a, arr_size) << "\n";
	clock_t std_end = clock();

	//Переменная для хранения результатов вычесления потоков
	int thrd_sum = 0;

	clock_t thrd_start = clock();
	//Первый поток
	thread thread1{ counter,std::ref(thrd_sum), a, arr_size, 0, (arr_size / 2) };
	thread1.join();

	//Второй поток
	thread thread2{ counter,std::ref(thrd_sum), a, arr_size, (arr_size / 2), arr_size };
	thread2.join();

	cout << "Thrd_sum = " << thrd_sum << "\n";
	clock_t thrd_end = clock();

	double std_time_sec = (double)(std_end - std_start) / CLOCKS_PER_SEC;
	double thrd_time_sec = (double)(thrd_end - thrd_start) / CLOCKS_PER_SEC;
	cout << "Std time: " << std_time_sec << "\n";
	cout << "Thread time: " << thrd_time_sec << "\n";

}