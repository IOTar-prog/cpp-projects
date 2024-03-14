#include <iostream>
#include <mutex>
#include <thread>
#include <time.h>
#include <vector>

using namespace std;
mutex mut;

double result = 0;		//результат

double f(double x) {
	return x * x;
}

double trapezoidal(double& res, double a, double b, int n) {

	double h = (b - a) / n;
	double sum = 0;

	for (int i = 0; i <= n; i++) {
		double x = a + i * h;
		sum += f(x);
	}

	return (h / 2) * sum;

	mut.lock();
	res += (f(a + i * h) + f(a + (i + 1) * h)) / 2 * h;
	mut.unlock();

}

int main() {

	double a = 0;			 //начальная координата
	double b = 0;			 //конечная координата
	double h = 0;			 //шаг интегрирования
	double n = 0;			 //число разбиений n
	int num_of_thrd = 0;	 //количество потоков


	//Вводим количество потоков
	cout << "Num of threads: ";
	cin >> num_of_thrd;

	//Создаём контейнер для хранения потоков и параллельно считаем интеграл
	vector<thread> threads;
	clock_t thrd_start_time = clock();

	for (int i = 0; i < num_of_thrd; i++) {

		unsigned long long start = (arr_size / num_of_thrd) * i;
		unsigned long long end_id = (start + arr_size / num_of_thrd);

		threads.emplace_back(thread(sum, std::ref(result), arr, start, end_id));
	}

	//Соединяем потоки
	for (int i = 0; i < num_of_thrd; ++i) {
		threads.at(i).join();
	}

	clock_t thrd_end_time = clock();

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

}