// lab1.cpp: ���������� ����� ����� ��� ����������� ����������.
//
/*1. ������� ���������� ���������� ����� N ������ ���������� ������� �
��������� �� -M �� M. ��� �����, N � M ��������� �� ���� �������, ����
������������ �������� ��� ��������� (������������ ������ �����-������).
����������� ��� �������� ��������� ����� � � ����� � � ��������������
��������� std::generate.
2. �������, ���������� ���� �� ����, � ������������ ����������������
���������, ����������� ������� �� �����.
3. ������� modify, ����������� �������������� ����������� ����������. �� ����
������� ������ ��������� ���������, � �������� ���������� ������� ������
���������� ���������������� ���������.
4. ������������� ������� modify, ����������� �� ���� ��������� ������ � �����
�������������� ����� ����������.
5. �������, ����������� ��������� �������������� � �������������� ���������
std::transform ������ ������� modify.
6. �������, ����������� ��������� �������������� � �������������� ���������
std::for_each ������ ������� modify.
7. �������, ����������� ����� � ������� �������������� �����, ������������ �
����������.
8. ������� ������ ���������� � ���� � �� ����� (�����������).*/
#include "stdafx.h"
#include <deque>
#include <iterator>//��� ������ ��������� ����������
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

// �������� ������������ ������� � ����� �����
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


// �������� �������� ������������ �����
bool CorrectName(string name)
{
	bool goodsm = true;
	for (size_t i = 1; (i < name.length() && goodsm); i++)
		goodsm = CorrectSymbol(name[i++]);
	return goodsm;
}

//���� ����� �����
string InputFileName()
{
	string filename;

	do
	{
		cout << "������� ��� �����(��� ����������):  ";
		cin >> filename;
		if (CorrectName(filename))
		{
			filename += ".txt";
			break;
		}
		else
			cout << "���� �� ������� �������\n";
	} while (true);

	return filename;
}


//��������� �������� generate
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

//���������� ����� 
ofstream WriteInFile(string FileName, int N, int M) {
	ofstream fout;
	fout.open(FileName);
	int answ;
	deque<int> q(N);
	cout << "��� ������ ������������� ��������?" << endl;
	cout << "1)� ������� generate" << endl
		<< "2)� �����" << endl;
	cin >> answ;
	switch (answ)
	{
	case 1:
		q = Generate(N, M);
		copy(q.begin(), q.end(), ostream_iterator<int>(fout, " "));
		cout << "���� ������! " << endl;
		break;
	case 2:
		q = Cycle(N, M);
		copy(q.begin(), q.end(), ostream_iterator<int>(fout, " "));
		cout << "���� ������! " << endl;
		break;
	}

	fout.close();
	return fout;
}

//���������� ���������� �� �����
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

//����� ���������� �������������� ����� ����� �������
int LastNegative(deque<int> q) {
	int lastnegative = 0;
	for (auto pos = q.rbegin(); pos != q.rend(); ++pos)
		if (*pos < 0) {
			lastnegative = *pos;
			break;
		}
	return lastnegative;
}

//����� ���������� �������������� ����� ����� ���������
int LastNegative(deque<int> ::iterator it_begin, deque<int> ::iterator it_end) {
	int lastnegative = 0;
	for (auto pos = it_begin; pos != it_end; ++pos)
		if (*pos < 0) {
			lastnegative = *pos;
		}
	return lastnegative;
}

//������� �������������� ����������
void Modify(deque<int> &q) {
	int lastNegative = LastNegative(q) / 2;
	for (auto pos = q.begin(); pos != q.end(); ++pos)
		*pos += lastNegative;
}

//������� �������������� ����������(�������������)
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

//���������� ������� � Transform
void WithTransform(deque<int> &q) {
	int lastnegtive = LastNegative(q)/2;
	transform(q.begin(), q.end(), q.begin(), Task(lastnegtive));
}

//���������� ������� � For_Each
void WithForEach(deque<int> &q) {
	int lastnegtive = LastNegative(q)/2;
	for_each(q.begin(), q.end(), Task(lastnegtive));
}

//����� ����� ���� ���������� ����������
int SumContainer(deque<int>& q) {
	int sum = 0, averA = 0;
	for (auto pos = q.begin(); pos != q.end(); ++pos) { sum += *pos; }
	return sum;
}

//����� �������� ���������������
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

//������ ������� �� �����
void OutputScreen(const deque<int>& q) {
	for (auto pos = q.begin(); pos != q.end(); pos++) { cout << *pos << " "; }
	cout << endl << "��������� ������� �� �����" << endl;
}

//������ ������� � ���� � �� ����� 
void PrintFile(string FileName, deque<int> &q) {
	ofstream out;
	out.open(FileName, ios::out);
	if (out) {
		for (auto pos = q.begin(); pos != q.end(); ++pos)
		{
			out << *pos << endl;
			cout << *pos << " ";
		}
		cout << endl << "��������� ������� � ���� � �� �����" << endl;
		out.close();
	}
	else
		cout << "������ �������� �����" << endl;
}

int Menu()
{
	cout << " --------------------����---------------------  " << endl;
	cout << "1 - Modify(�������������� ������ )" << endl;
	cout << "2 - Modify(�������������� ������ - ������������� �������) " << endl;
	cout << "3 - �������������� � ������� transform" << endl;
	cout << "4 - �������������� � ������� for_each" << endl;
	cout << "5 - ��������� �����" << endl;
	cout << "6 - ��������� ������� ��������������" << endl;
	cout << "7 - ����� �� ����� " << endl;
	cout << "8 - ����� � ����" << endl;
	cout << "0 - ����� " << endl;
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
	cout << "������� ���������� ����� �����:" << endl;
	cin >> N;
	cout << "������� �������� (M)" << endl;
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
			cout << "����� 0" << endl;
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
			cout << "����� : " << SumContainer(deq) << endl;
			break;
		case 6:
			cout << "������� ��������������: " << AverageContainer(deq) << endl;			
			break;
		case 7:
			cout << "�������: " << endl;
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

