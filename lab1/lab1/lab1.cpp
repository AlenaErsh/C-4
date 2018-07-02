// lab1.cpp: определяет точку входа для консольного приложения.
//
/*1. Функцию заполнения текстового файла N целыми случайными числами в
диапазоне от -M до M. Имя файла, N и M поступают на вход функции, файл
возвращается функцией как результат (использовать потоки ввода-вывода).
Реализовать два варианта генерации чисел — в цикле и с использованием
алгоритма std::generate.
2. Функцию, получающую файл на вход, и возвращающую последовательный
контейнер, заполненный числами из файла.
3. Функцию modify, выполняющую преобразование полученного контейнера. На вход
функции должен поступать контейнер, в качестве результата функция должна
возвращать модифицированный контейнер.
4. Перегруженную функцию modify, принимающую на вход итераторы начала и конца
обрабатываемой части контейнера.
5. Функцию, реализующую требуемое преобразование с использованием алгоритма
std::transform вместо функции modify.
6. Функцию, реализующую требуемое преобразование с использованием алгоритма
std::for_each вместо функции modify.
7. Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в
контейнере.
8. Функцию вывода результата в файл и на экран (опционально).*/
#include "stdafx.h"
#include <deque>
#include <iterator>//для вывода элементов контейнера
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <time.h>  

using namespace std;


class	Task {
private:
	int  lastN_mean;
public:
	Task(const int& Val) :lastN_mean(Val){
	}

	int operator ( ) (int& elem) const
	{
		elem += lastN_mean;
		return elem;
	}
};

class RandomNumber
{
private:
	int M;
public:
	RandomNumber(const int& Val) : M(Val) {
	}
	int operator () () const
	{
		return rand() % (2 * M + 1) - M;
	}
};

int Random(int M) {	
	return rand() % (2 * M + 1) - M;
}

// проверка корректности символа в имени файла
bool CorrectSymbol(char c)
{
	switch (c)
	{
	case '.':
	case '*':
	case '?':
	case '<':
	case '>':
	case ':':
	case '|':
		return false;
	default:
		return true;
	}
}


// проверка проверка корректности имени
bool CorrectName(string name)
{
	bool goodsm = true;
	for (size_t i = 1; (i < name.length() && goodsm); i++)
		goodsm = CorrectSymbol(name[i++]);
	return goodsm;
}

//ввод имени файла
string InputFileName()
{
	string filename;

	do
	{
		cout << "Введите имя файла(без расширения):  ";
		cin >> filename;
		if (CorrectName(filename))
		{
			filename += ".txt";
			break;
		}
		else
			cout << "Файл не удалось создать\n";
	} while (true);

	return filename;
}


//генерация значений generate
deque<int> Generate(int N, int M) {
	deque<int> q(N);
	srand((unsigned int)time(NULL));
	generate(q.begin(), q.end(), RandomNumber(M));
	return q;
}

deque<int> Cycle(int N, int M) {
	deque<int> q;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < N; i++)
		q.push_back(Random(M));
	return q;
}

//заполнение файла 
ofstream WriteInFile(string FileName, int N, int M) {
	ofstream fout;
	fout.open(FileName);
	int answ;
	deque<int> q(N);
	cout << "Как хотите сгенерировать значения?" << endl;
	cout << "1)С помощью generate" << endl
		<< "2)В цикле" << endl;
	cin >> answ;
	switch (answ)
	{
	case 1:
		q = Generate(N, M);
		copy(q.begin(), q.end(), ostream_iterator<int>(fout, " "));
		cout << "Файл создан! " << endl;
		break;
	case 2:
		q = Cycle(N, M);
		copy(q.begin(), q.end(), ostream_iterator<int>(fout, " "));
		cout << "Файл создан! " << endl;
		break;
	}

	fout.close();
	return fout;
}

//заполнение контейнера из файла
deque<int> FileToContainer(string FileName) {
	ifstream file(FileName);
	deque<int> q;
	int x;
	file >> x;
	while (!file.eof())
	{
		q.push_back(x);
		file >> x;

	}
	file.close();
	return q;
}

//поиск последнего отрицательного числа через очередь
int LastNegative(deque<int> q) {
	int lastnegative = 0;
	for (auto pos = q.rbegin(); pos != q.rend(); ++pos)
		if (*pos < 0) {
			lastnegative = *pos;
			break;
		}
	return lastnegative;
}

//поиск последнего отрицательного числа через итераторы
int LastNegative(deque<int> ::iterator it_begin, deque<int> ::iterator it_end) {
	int lastnegative = 0;
	for (auto pos = it_begin; pos != it_end; ++pos)
		if (*pos < 0) {
			lastnegative = *pos;
		}
	return lastnegative;
}

//функция преобразования контейнера
void Modify(deque<int> &q) {
	int lastNegative = LastNegative(q) / 2;
	for (auto pos = q.begin(); pos != q.end(); ++pos)
		*pos += lastNegative;
}

//функция преобразования контейнера(перегруженная)
deque<int> Modify(deque<int> ::iterator it_begin, deque<int> ::iterator it_end) {
	deque<int> q;
	int lastNegative = LastNegative(it_begin,it_end) / 2;
	for (auto pos = it_begin; pos != it_end; pos++)
	{	
		*pos += lastNegative;
		q.push_back(*pos);
	}
	return q;
}

//выполнение задания с Transform
void WithTransform(deque<int> &q) {
	int lastnegtive = LastNegative(q)/2;
	transform(q.begin(), q.end(), q.begin(), Task(lastnegtive));
}

//выполнение задания с For_Each
void WithForEach(deque<int> &q) {
	int lastnegtive = LastNegative(q)/2;
	for_each(q.begin(), q.end(), Task(lastnegtive));
}

//поиск суммы всех эллементов контейнера
int SumContainer(deque<int>& q) {
	int sum = 0, averA = 0;
	for (auto pos = q.begin(); pos != q.end(); ++pos) { sum += *pos; }
	return sum;
}

//поиск среднего арифметического
double AverageContainer(deque<int>& q) {
	int n = 0;
	if (q.empty()) {
		return 0;
	}
	else {
		for (auto pos = q.begin(); pos != q.end(); ++pos) { n++; }
		return (double)SumContainer(q) / n;
	}
}

//печать очереди на экран
void OutputScreen(const deque<int>& q) {
	for (auto pos = q.begin(); pos != q.end(); pos++) { cout << *pos << " "; }
	cout << endl << "Результат выведен на экран" << endl;
}

//печать очереди в файл и на экран 
void PrintFile(string FileName, deque<int> &q) {
	ofstream out;
	out.open(FileName, ios::out);
	if (out) {
		for (auto pos = q.begin(); pos != q.end(); ++pos)
		{
			out << *pos << endl;
			cout << *pos << " ";
		}
		cout << endl << "Результат выведен в файл и на экран" << endl;
		out.close();
	}
	else
		cout << "Ошибка открытия файла" << endl;
}

int Menu()
{
	cout << " --------------------Меню---------------------  " << endl;
	cout << "1 - Modify(преобразование списка )" << endl;
	cout << "2 - Modify(преобразование списка - перегруженный вариант) " << endl;
	cout << "3 - Преобразование с помощью transform" << endl;
	cout << "4 - Преобразование с помощью for_each" << endl;
	cout << "5 - Посчитать сумму" << endl;
	cout << "6 - Посчитать среднее арифметическое" << endl;
	cout << "7 - Вывод на экран " << endl;
	cout << "8 - Вывод в файл" << endl;
	cout << "0 - Выход " << endl;
	int answ;
	cin >> answ;
	return answ;
}


int main()
{
	setlocale(LC_ALL, "rus");
	ofstream OutputFile;
	ifstream InputFile;
	string FileName = "";
	deque<int> deq;
	int N, M;
	cout << "Введите количество целых чисел:" << endl;
	cin >> N;
	cout << "Введите диапазон (M)" << endl;
	cin >> M;
	FileName = InputFileName();
	WriteInFile(FileName, N, M);
	deq = FileToContainer(FileName);
	int answ;
	do {
		answ = Menu();
		switch (answ)
		{
		case 0:
			cout << "Выход 0" << endl;
			break;
		case 1:
			Modify(deq);
			break;
		case 2:
			deq = Modify(deq.begin(), deq.end());
			break;
		case 3:
			WithTransform(deq);
			break;
		case 4:
			WithForEach(deq);
			break;
		case 5:
			cout << "Сумма : " << SumContainer(deq) << endl;
			break;
		case 6:
			cout << "Среднее арифметическое: " << AverageContainer(deq) << endl;			
			break;
		case 7:
			cout << "Очередь: " << endl;
			OutputScreen(deq);
			break;
		case 8:
			PrintFile(FileName, deq);
			break;
		}
	} while (answ != 0);
	system("pause");
	return 0;
}

