#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

void create_binary(int Size, string FileName) {

	ofstream out(FileName, ios::binary | ios::out);


	if (!out.is_open()) {
		cout << "File Not Found";
		exit(1);
	}
	else {
		int x = 0;
		for (int i = 0; i < Size; i++) {
			out.write(reinterpret_cast<const char*>(&i), sizeof(int));
		}
	}


}

void read_binary(int Size, string FileName) {

	fstream in(FileName, ios::binary | ios::in);
	int x = 0;
	for (int i = 0; i < Size; i++) {
		in.read((char*)&x, sizeof x);
		cout << x << " ";
	}

}

void analyze(string FileName) {

	fstream file(FileName, ios::binary | ios::in);
	file.open(FileName);

	//записываем первый элемент как минимум и максимум 
	int max;
	//становимся в начало файла
	file.seekg(0, ios_base::beg);
	file.read((char*)(&max), sizeof max);

	int min;
	file.seekg(0, ios_base::beg);
	file.read((char*)(&min), sizeof min);

	file.seekg(0, ios_base::beg);
	for (int i = 0; i < (sizeof(file) / 10); i++) {
		//создаём массив и передаём в него 10 элементов из файла
		int* arr = new int[10];
		for (int x = 0; x < sizeof(arr); x++) {
			file.read((char*)(&arr[i]), sizeof arr[i]);
			if (arr[i] > max) max = arr[i];
			if (arr[i] < min) min = arr[i];
			cout << arr[i] << " ";
		}
		delete[] arr;
	}
	file.close();
	cout << " max=" << max;
	cout << " min=" << min;


}

int main() {

	string FileName = "D://visual studio//cpp projects//binary//test_file.bin";
	int n = 50;

	create_binary(n, FileName);
	//read_binary(n, FileName);
	analyze(FileName);

}

