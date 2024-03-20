#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <time.h>
#include <vector>

using namespace std;
mutex mut;
double result = 0;

void InitArray(double* Arr1, double* Arr2, size_t arr_size)
{
	for (unsigned int i = 0; i < arr_size; i++)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(1, 3);

		Arr1[i] = dis(gen);
	}

	for (size_t i = 0; i < arr_size; i++)
	{
		Arr2[i] = 1 / Arr1[i];
	}
}

void Thrd_product(double& prod, double* Arr1, double* Arr2, size_t start_indx, size_t end_indx) {
	for (size_t i = 0; i < end_indx - start_indx; i++) {
		mut.lock();
		prod += Arr1[start_indx + i] * Arr2[start_indx + i];
		mut.unlock();
	}
}

int main()
{

	size_t n = 0;				//размер массива (количество измерений вектора)
	int thread_count = 0;		//количество потоков
	

	//Задаём размер массива
	cout << "Enter size of array: ";
	cin >> n;
	cout << "\n";

	double* Arr1 = new double[n];
	double* Arr2 = new double[n];

	//Задаём количество потоков
	cout << "Enter count of threads: ";
	cin >> thread_count;
	cout << "\n";

	//Заполняем массивы
	InitArray(Arr1, Arr2, n);
	cout << "array is create" << "\n";

	//Считаем стандартной функцией
	clock_t std_start_time = clock();
	Thrd_product(result, Arr1, Arr2, 0, n);
	clock_t std_end_time = clock();
	double std_time = (double)(std_end_time - std_start_time) / CLOCKS_PER_SEC;

	cout << "std result=" << result << "\n";
	cout << "std time =" << std_time << "\n";
	result = 0;
	
	//Считаем многопоточной функцией

	//Проверяем остаток от деления числа элементов массива на количество потоков
	if (n % thread_count != 0) {
		int size_of_arr = sizeof(Arr1) / sizeof(Arr1[0]);
		for (int i = 0; i < n % thread_count; i++) {
			result += Arr1[size_of_arr - 1] * Arr2[size_of_arr - 1];
		}

		n -= n % thread_count;
	}

	//Создаём контейнер для хранения потоков и параллельно считаем произведение
	vector<thread> threads;
	clock_t thrd_start_time = clock();
	for (int i = 0; i < thread_count; i++) {

			size_t start_indx = (n / thread_count) * i;
			size_t end_indx = (start_indx + n / thread_count);

			threads.emplace_back(thread(Thrd_product, std::ref(result), Arr1, Arr2, start_indx, end_indx));

	}

	//Соединяем потоки
	for (int i = 0; i < thread_count; ++i) {
		threads.at(i).join();
	}
	clock_t thrd_end_time = clock();

	cout << "Thread result= " << result << " \n";
	double thrd_time_sec = (double)(thrd_end_time - thrd_start_time) / CLOCKS_PER_SEC;
	cout << "Thread time= " << thrd_time_sec << " \n";

	delete[]Arr1;
	delete[]Arr2;
	return 0;
}

