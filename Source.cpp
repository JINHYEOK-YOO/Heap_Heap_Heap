/* 2014707040 ������ */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using namespace std;

#define MAX_SIZE 151 // 0��° ��� ��� X
int V[4][2] = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} }; // ���� ����

/* ���� �� ��� Ŭ���� */
class HeapNode
{
public:
	HeapNode(char init_data = '0') : data(init_data) {}
	void setData(char new_data) { data = new_data; }
	char getData() { return data; }

	/* �ƽ�Ű�ڵ� ������ �ٸ��Ƿ� �񱳿����� �����ε� */
	bool operator<(const HeapNode& node)
	{
		if ((data >= 'A' && data <= 'Z') && (node.data >= 'a' && node.data <= 'z'))
			return false;
		else if ((data >= 'a' && data <= 'z') && (node.data >= 'A' && node.data <= 'Z'))
			return true;
		else
			return data < node.data;
	}
	bool operator>(const HeapNode& node)
	{
		if ((data >= 'A' && data <= 'Z') && (node.data >= 'a' && node.data <= 'z'))
			return true;
		else if ((data >= 'a' && data <= 'z') && (node.data >= 'A' && node.data <= 'Z'))
			return false;
		else
			return data > node.data;
	}

private:
	char data;
};

/* ����� �迭�� ���ǵ� �� Ŭ���� */
class HeapArray
{
public:
	HeapArray() : size(0) {}
	void add(char data);
	void remove();
	HeapNode& getParent(int i) { return node[i / 2]; } // i��° ����� �θ� ��� ��ȯ
	HeapNode& getLeft(int i) { return node[i * 2]; } // i��° ����� ���� �ڽ� ��� ��ȯ
	HeapNode& getRight(int i) { return node[i * 2 + 1]; } // i��° ����� ������ �ڽ� ��� ��ȯ
	bool empty() { return size == 0; }
	bool full() { return size == MAX_SIZE - 1; }
	void print1(int i, int depth);
	void not_rotated(char** frame, int n, int depth, int center, int row);
	void print2();
	void H(char** H_tree, int n, int i, int j, int h, int v, int U, int D, int R, int L);
	int H_center(int n) { return n <= 1 ? 0 : 2 * H_center(n / 4) + 1; } // H-tree���� �߽� ��ǥ�� ���ϴ� �Լ�
	int hor_depth(int n) { return n <= 7 ? 1 : 2 * hor_depth(n / 4); } // H-tree���� ���� ���̸� ���ϴ� �Լ�
	int vert_depth(int n) { return n <= 15 ? 1 : 2 * hor_depth(n / 4); } // H-tree���� ���� ���̸� ���ϴ� �Լ�
	void print3();

private:
	HeapNode node[MAX_SIZE];
	int size; // ������ ����� ����
};

/* ���� ���ο� ��带 �߰� */
void HeapArray::add(char data)
{
	if (full())
		return;

	HeapNode new_node(data);
	int i = ++size; // �ε��� ����

	while ((i != 1) && (new_node > getParent(i))) // ��Ʈ �ƴϰ� �θ� ��庸�� ũ��
	{
		node[i] = getParent(i); // �θ� ��� �����
		i /= 2; // ���� �̵�
	}
	node[i].setData(data); // ���� ��ġ�� ����
}

/* ������ ��Ʈ ��带 ���� */
void HeapArray::remove()
{
	if (empty())
		return;

	HeapNode last = node[size]; // ������ ���
	int location = 1; // �ʱ� ��ġ ��Ʈ
	int child = 2; // ���� �ڽ� ��ġ
	while (child <= size)
	{
		if ((child < size) && (getLeft(location) < getRight(location))) // �ڽ��� �� �� �� �ְ� ������ �ڽ��� �� ũ�ٸ�
			child++; // ������ �ڽİ� ��
		if ((last > node[child]) || (last.getData() == node[child].getData())) // ������ ��� ���� �ڽĺ��� ũ��
			break; // �̵� �Ϸ�
		else // �� �ܰ� �Ʒ��� �̵�
		{
			node[location] = node[child];
			location = child;
			child *= 2;
		}
	}
	node[location] = last; // ���� ��ġ�� ����
	size--; // ��� ���� ����
}

/* rotated form ��� �Լ� */
void HeapArray::print1(int i, int depth)
{
	if (i <= size)
	{
		/* Backward In-Order Traversal */
		print1(i * 2 + 1, depth + 1); // ������ �ڽ�
		cout << setw(2 * depth) << node[i].getData() << endl; // �ڱ��ڽ�
		print1(i * 2, depth + 1); // ���� �ڽ�
	}
}

/* not-rotated form�� ���� ��� �Լ� */
void HeapArray::not_rotated(char** frame, int n, int depth, int center, int row)
{
	if (n > size)
		return;
	frame[depth][center] = node[n].getData(); // �ڱ��ڽ�
	if (2 * n <= size) // ���� �ڽ�
		not_rotated(frame, 2 * n, depth + 1, center - (int)pow(2, row - (depth + 2)), row);
	if (2 * n + 1 <= size) // ������ �ڽ�
		not_rotated(frame, 2 * n + 1, depth + 1, center + (int)pow(2, row - (depth + 2)), row);
}

/* 2���� �迭�� �̿��� not-rotated form ��� */
void HeapArray::print2()
{
	int row = (int)(log(size) / log(2)) + 1; // ���� ����
	int col = (int)pow(2, row) - 1; // ���� ����
	int center = col / 2; // �� ���� �߽�

	/* �迭 ũ�⸦ ������ �ޱ� ���� ���� �Ҵ� */
	char** frame = new char*[row];
	for (int i = 0; i < row; i++)
	{
		frame[i] = new char[col];
		memset(frame[i], ' ', col);
	}

	/* �迭 ä�� �� ��� */
	not_rotated(frame, 1, 0, center, row);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << frame[i][j];
		cout << endl;
	}

	/* ���� �Ҵ� ���� */
	for (int i = 0; i < row; i++)
		delete[] frame[i];
	delete[] frame;
}

/* H-tree form�� ���� ��� �Լ� */
void HeapArray::H(char** H_tree, int n, int i, int j, int h, int v, int U, int D, int R, int L)
{
	if (n > size)
		return;
	H_tree[i][j] = node[n].getData(); //�ڱ��ڽ�
	if (2 * n <= size) // ���� �ڽ��� ���
	{
		for (int index = 1; index < h; index++) // �������� #
			H_tree[i + index*V[L][0]][j + index*V[L][1]] = '#';
		for (int index = 1; index < v; index++) // ���� ���Ʒ��� #
		{
			H_tree[i + v * V[L][0] + index * V[U][0]][j + v * V[L][1] + index * V[U][1]] = '#';
			H_tree[i + v * V[L][0] + index * V[D][0]][j + v * V[L][1] + index * V[D][1]] = '#';
		}
		H_tree[i + h*V[L][0]][j + h*V[L][1]] = node[2 * n].getData(); // ���� �ڽ� ����
		H(H_tree, 4 * n, i + v*(V[L][0] + V[U][0]), j + h*(V[L][1] + V[U][1]), h / 2, v / 2, D, U, L, R); // ���� ���� �̵�
		H(H_tree, 4 * n + 1, i + v*(V[L][0] + V[D][0]), j + h*(V[L][1] + V[D][1]), h / 2, v / 2, U, D, R, L); // ���� �Ʒ��� �̵�
	}
	if (2 * n + 1 <= size) // ������ �ڽ��� ���
	{
		for (int index = 1; index < h; index++) // ���������� #
			H_tree[i + index*V[R][0]][j + index*V[R][1]] = '#';
		for (int index = 1; index < v; index++) // ������ ���Ʒ��� #
		{
			H_tree[i + v * V[R][0] + index * V[U][0]][j + v * V[R][1] + index * V[U][1]] = '#';
			H_tree[i + v * V[R][0] + index * V[D][0]][j + v * V[R][1] + index * V[D][1]] = '#';
		}
		H_tree[i + h*V[R][0]][j + h*V[R][1]] = node[2 * n + 1].getData(); // ������ �ڽ� ����
		H(H_tree, 4 * n + 2, i + v*(V[R][0] + V[D][0]), j + h*(V[R][1] + V[D][1]), h / 2, v / 2, U, D, R, L); // ������ �Ʒ��� �̵�
		H(H_tree, 4 * n + 3, i + v*(V[R][0] + V[U][0]), j + h*(V[R][1] + V[U][1]), h / 2, v / 2, D, U, L, R); // ������ ���� �̵�
	}
}

/* 2���� �迭�� �̿��� H-tree form ��� */
void HeapArray::print3()
{
	int side = H_center(size) * 2 + 1; // �迭 �� ���� ����

	/* �迭 ũ�⸦ ������ �ޱ� ���� ���� �Ҵ� */
	char** H_tree = new char*[side];
	for (int i = 0; i < side; i++)
	{
		H_tree[i] = new char[side];
		memset(H_tree[i], ' ', side);
	}

	/* �迭 ä�� �� ��� */
	H(H_tree, 1, H_center(size), H_center(size), hor_depth(size), vert_depth(size), 0, 1, 2, 3);
	for (int i = 0; i < side; i++)
	{
		for (int j = 0; j < side; j++)
			cout << H_tree[i][j];
		cout << endl;
	}

	/* ���� �Ҵ� ���� */
	for (int i = 0; i < side; i++)
		delete[] H_tree[i];
	delete[] H_tree;
}

int main()
{
	ifstream inStream("input.txt"); // ���� �Է�
	if (inStream.fail()) // ���� ���� ���� �� ����
	{
		cout << "Input file opening failed.\n";
		exit (1);
	}

	int size; // ���� Ƚ��
	string operations; // ���� ���ڿ�

	inStream >> size; // ù ��° �� �Է�

	if (size > 150) // ���� �ʰ� ��
		size = 150; // 150 ����

	while (!inStream.eof()) // ������ ������
	{
		/* ���� ���ڸ� ������ ������ ���ڸ� ���ڿ��� ����*/
		string temp;
		inStream >> temp;
		operations += temp;
	}

	HeapArray myHeap; // �� ����
	int i = 0; // ���ڿ� �ε���
	for (int n = 0; n < size; n++) // ������ ���� ����ŭ ����
	{
		if (operations[i] == 'I') // insert
		{
			myHeap.add(operations[i + 1]);
			i += 2;
		}
		else if (operations[i] == 'D') // delete
		{
			myHeap.remove();
			i++;
		}
	}

	/* ��� */
	cout << "1. rotated form\n" << endl;
	myHeap.print1(1, 1);
	cout << endl << "===============================================================================" << endl << endl;

	cout << "2. not-rotated form\n" << endl;
	myHeap.print2();
	cout << endl << "===============================================================================" << endl << endl;

	cout << "3. H-tree form\n" << endl;
	myHeap.print3();
	cout << endl;

	inStream.close();

	return 0;
}