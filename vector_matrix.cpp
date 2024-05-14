#include <thread>
#include <iostream>
#include <random>
#include <mutex>

using namespace std;

// создаём вектор и заполняем случайными значениями
void VectorInit(double* pVector, size_t size) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<>dis(1, 2);

	for (size_t i = 0; i < size; i++) {
		pVector[i] = dis(gen);
	}
}

// создаём матрицу и заполняем её случайными значениями
void MatrixInit(double* pMatrix, double* pVector, size_t rows, size_t cols)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<>dis(1, 2);

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			pMatrix[i * cols + j] = dis(gen);
		}
	}
}

// функция, создающая и инициализирующая объекты
void ProcessInit(double*& pMatrix, double*& pVector, double*& pResult, size_t& rows, size_t& cols)
{
	pMatrix = new double[rows * cols];
	pVector = new double[cols];
	pResult = new double[rows];
	MatrixInit(pMatrix, pVector, rows, cols);
	VectorInit(pVector, cols);
}

// функция, утилизирующая объекты
void ProcessTermination(double*& pMatrix, double*& pVector, double*& pResult)
{
	delete[] pMatrix;
	delete[] pVector;
	delete[] pResult;
}

// выводим матрицу
void MatrixPrint(double* pMatrix, size_t rows, size_t cols) {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			cout << pMatrix[i * cols + j] << "\t";
		}
		cout << endl;
	}
}

// выводим вектор
void VectorPrint(double* pVector, size_t cols)
{
	for (size_t i = 0; i < cols; i++) {
		cout << pVector[i] << "\t";
	}
	cout << endl;
}

// стандартное перемножение
void std_mult(double* pMatrix, double* pVector, size_t rows, size_t cols, double* pResult) {
	for (size_t i = 0; i < rows; i++) {
		pResult[i] = 0;
		for (size_t j = 0; j < cols; j++) {
			pResult[i] += pMatrix[i * cols + j] * pVector[j];
		}
	}
}

// перемножение в потоках
void thrd_mult(size_t CurrThread, size_t number_of_threads, double* pMatrix, double* pVector, size_t cols, double* pResult) {

	for (size_t i = cols * CurrThread / number_of_threads; i < cols * (CurrThread + 1) / number_of_threads; i++) {
		pResult[i] = 0;
		for (size_t j = 0; j < cols; j++) {
			pResult[i] += pMatrix[i * cols + j] * pVector[j];
		}
	}
}

void createThreads(size_t number_of_threads, double* pMatrix, double* pVector, size_t cols, double* pResult) {
	vector<thread> threads;

	for (size_t i = 0; i < number_of_threads; i++) {
		threads.emplace_back(thread(thrd_mult, i, number_of_threads, pMatrix, pVector, cols, pResult));
	}

	for(auto& t : threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	cout << endl;
	cout << endl;
}

int main() {
	double* pMatrix;
	double* pVector;
	double* pResult;
	size_t rows, cols, NumOfThreads;

	cout << "Enter number of rows: ";
	cin >> rows;
	cout << "Enter number of cols: ";
	cin >> cols;
	cout << endl;

	ProcessInit(pMatrix, pVector, pResult, rows, cols);

	//cout << "init matrix: " << endl;
	//MatrixPrint(pMatrix, rows, cols);
	//cout << endl;
	//cout << "init vector: " << endl;
	//VectorPrint(pVector, cols);
	//cout << endl;

	auto start0 = chrono::high_resolution_clock::now();

	std_mult(pMatrix, pVector, rows, cols, pResult);

	auto end0 = chrono::high_resolution_clock::now();
	chrono::duration < double > duration0 = end0 - start0;
	cout << "Multiply Std time: " << duration0.count() << " s" << endl;

	cout << endl;
	cout << "result vector: " << endl;
	VectorPrint(pResult, cols);
	cout << endl;

	cout << endl;

	cout << "Enter number of threads: ";
	cin >> NumOfThreads;
	cout << endl;

	auto start1 = chrono::high_resolution_clock::now();

	createThreads(NumOfThreads, pMatrix, pVector, cols, pResult);
	
	auto end1 = chrono::high_resolution_clock::now();
	chrono::duration < double > duration1 = end1 - start1;
	cout << "Multiply Thr time: " << duration1.count() << " s" << endl << endl;

	VectorPrint(pResult, cols);
}