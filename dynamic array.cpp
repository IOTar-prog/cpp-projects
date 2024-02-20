#include <fstream>
#include <stdio.h>
#include <iostream>

using namespace std;

int GetIntNum(string Message)
{
	cout << Message;
	int num;
	cin >> num;
	return num;
}

void InitArray(double* Arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << "Enter " << i + 1 << " elem ";
		cin >> Arr[i];
	}
}

void PrintArray(double* Arr, int size)
{
	cout << "{ ";
	for (int i = 0; i < size; i++)
	{
		cout << Arr[i] << "; ";
	}
	cout << " }";
}

void WriteTextInFile(double* Arr, int size, string FileName)
{
	ofstream out;
	out.open(FileName);
	if (!out.is_open())
	{
		cout << "File Not Found";
		exit(1);
	}
	out << "{ ";
	for (int i = 0; i < size; i++)
	{
		out << Arr[i] << "; ";
	}
	out << " }";
}

void WriteBinaryInFile(double* Arr, int size, string FileName)
{
	ofstream out(FileName, ios::binary);
	out.open(FileName);
	if (!out.is_open())
	{
		cout << "File Not Found";
		exit(1);
	}

	for (int i = 0; i < size; i++)
	{
		double x = Arr[i];
		out.write(reinterpret_cast<const char*>(&x), sizeof(double));
	}

	out.close();
}

int main()
{
	int n;

	n = GetIntNum("Input ArraySize ");

	double* Arr = new double[n];

	InitArray(Arr, n);
	PrintArray(Arr, n);
	//WriteTextInFile(Arr, n, "D:\\Учеба\\3 курс\\Параллельные вычисления\\Arr.txt");
	WriteBinaryInFile(Arr, n, "C:\\Users\\ilyat\\Desktop\\cpp\\Bynary.bin");
}
