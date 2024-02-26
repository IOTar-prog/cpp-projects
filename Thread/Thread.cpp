#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
mutex mut;

//Функция для создания массива
void arr(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i+1;
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
void counter(int &sum, int* arr, int size, int begin_index, int end_index) {
	for (int i = begin_index; i < end_index; i++) {
		mut.lock();
		sum += arr[i];
		mut.unlock();
	}
}

int main()
{
	// Размер массива
	int arr_size = 10;

	//Создаём массив
	int* a = new int[arr_size];
	arr(a, arr_size);

	//Выводим массив
	for (int i = 0; i < arr_size; i++) {
		cout << a[i] << ' ';
	}
	cout << '\n';

	//Считаем сумму массива
	cout << "Sum = " << arr_sum(a, arr_size) << "\n";

	//Переменная для хранения результатов вычесления потоков
	int result = 0;

	//Первый поток
	thread thread1{counter,std::ref(result), a, arr_size, 0, 5};
	thread1.join();

	//Второй поток
	thread thread2{ counter,std::ref(result), a, arr_size, 5, 10};
	thread2.join();

	cout << "Result = " << result << "\n";
	
}