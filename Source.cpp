/* 2014707040 유진혁 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using namespace std;

#define MAX_SIZE 151 // 0번째 요소 사용 X
int V[4][2] = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} }; // 단위 벡터

/* 힙의 각 노드 클래스 */
class HeapNode
{
public:
	HeapNode(char init_data = '0') : data(init_data) {}
	void setData(char new_data) { data = new_data; }
	char getData() { return data; }

	/* 아스키코드 순서와 다르므로 비교연산자 오버로딩 */
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

/* 노드의 배열로 정의된 힙 클래스 */
class HeapArray
{
public:
	HeapArray() : size(0) {}
	void add(char data);
	void remove();
	HeapNode& getParent(int i) { return node[i / 2]; } // i번째 요소의 부모 노드 반환
	HeapNode& getLeft(int i) { return node[i * 2]; } // i번째 요소의 왼쪽 자식 노드 반환
	HeapNode& getRight(int i) { return node[i * 2 + 1]; } // i번째 요소의 오른쪽 자식 노드 반환
	bool empty() { return size == 0; }
	bool full() { return size == MAX_SIZE - 1; }
	void print1(int i, int depth);
	void not_rotated(char** frame, int n, int depth, int center, int row);
	void print2();
	void H(char** H_tree, int n, int i, int j, int h, int v, int U, int D, int R, int L);
	int H_center(int n) { return n <= 1 ? 0 : 2 * H_center(n / 4) + 1; } // H-tree에서 중심 좌표를 구하는 함수
	int hor_depth(int n) { return n <= 7 ? 1 : 2 * hor_depth(n / 4); } // H-tree에서 가로 깊이를 구하는 함수
	int vert_depth(int n) { return n <= 15 ? 1 : 2 * hor_depth(n / 4); } // H-tree에서 세로 깊이를 구하는 함수
	void print3();

private:
	HeapNode node[MAX_SIZE];
	int size; // 힙에서 노드의 개수
};

/* 힙에 새로운 노드를 추가 */
void HeapArray::add(char data)
{
	if (full())
		return;

	HeapNode new_node(data);
	int i = ++size; // 인덱스 변수

	while ((i != 1) && (new_node > getParent(i))) // 루트 아니고 부모 노드보다 크면
	{
		node[i] = getParent(i); // 부모 노드 끌어내림
		i /= 2; // 위로 이동
	}
	node[i].setData(data); // 최종 위치에 저장
}

/* 힙에서 루트 노드를 제거 */
void HeapArray::remove()
{
	if (empty())
		return;

	HeapNode last = node[size]; // 마지막 노드
	int location = 1; // 초기 위치 루트
	int child = 2; // 왼쪽 자식 위치
	while (child <= size)
	{
		if ((child < size) && (getLeft(location) < getRight(location))) // 자식이 양 쪽 다 있고 오른쪽 자식이 더 크다면
			child++; // 오른쪽 자식과 비교
		if ((last > node[child]) || (last.getData() == node[child].getData())) // 마지막 노드 값이 자식보다 크면
			break; // 이동 완료
		else // 한 단계 아래로 이동
		{
			node[location] = node[child];
			location = child;
			child *= 2;
		}
	}
	node[location] = last; // 최종 위치에 저장
	size--; // 노드 개수 감소
}

/* rotated form 출력 함수 */
void HeapArray::print1(int i, int depth)
{
	if (i <= size)
	{
		/* Backward In-Order Traversal */
		print1(i * 2 + 1, depth + 1); // 오른쪽 자식
		cout << setw(2 * depth) << node[i].getData() << endl; // 자기자신
		print1(i * 2, depth + 1); // 왼쪽 자식
	}
}

/* not-rotated form을 위한 재귀 함수 */
void HeapArray::not_rotated(char** frame, int n, int depth, int center, int row)
{
	if (n > size)
		return;
	frame[depth][center] = node[n].getData(); // 자기자신
	if (2 * n <= size) // 왼쪽 자식
		not_rotated(frame, 2 * n, depth + 1, center - (int)pow(2, row - (depth + 2)), row);
	if (2 * n + 1 <= size) // 오른쪽 자식
		not_rotated(frame, 2 * n + 1, depth + 1, center + (int)pow(2, row - (depth + 2)), row);
}

/* 2차원 배열을 이용한 not-rotated form 출력 */
void HeapArray::print2()
{
	int row = (int)(log(size) / log(2)) + 1; // 행의 개수
	int col = (int)pow(2, row) - 1; // 열의 개수
	int center = col / 2; // 한 행의 중심

	/* 배열 크기를 변수로 받기 위해 동적 할당 */
	char** frame = new char*[row];
	for (int i = 0; i < row; i++)
	{
		frame[i] = new char[col];
		memset(frame[i], ' ', col);
	}

	/* 배열 채운 후 출력 */
	not_rotated(frame, 1, 0, center, row);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << frame[i][j];
		cout << endl;
	}

	/* 동적 할당 해제 */
	for (int i = 0; i < row; i++)
		delete[] frame[i];
	delete[] frame;
}

/* H-tree form을 위한 재귀 함수 */
void HeapArray::H(char** H_tree, int n, int i, int j, int h, int v, int U, int D, int R, int L)
{
	if (n > size)
		return;
	H_tree[i][j] = node[n].getData(); //자기자신
	if (2 * n <= size) // 왼쪽 자식의 경우
	{
		for (int index = 1; index < h; index++) // 왼쪽으로 #
			H_tree[i + index*V[L][0]][j + index*V[L][1]] = '#';
		for (int index = 1; index < v; index++) // 왼쪽 위아래로 #
		{
			H_tree[i + v * V[L][0] + index * V[U][0]][j + v * V[L][1] + index * V[U][1]] = '#';
			H_tree[i + v * V[L][0] + index * V[D][0]][j + v * V[L][1] + index * V[D][1]] = '#';
		}
		H_tree[i + h*V[L][0]][j + h*V[L][1]] = node[2 * n].getData(); // 왼쪽 자식 저장
		H(H_tree, 4 * n, i + v*(V[L][0] + V[U][0]), j + h*(V[L][1] + V[U][1]), h / 2, v / 2, D, U, L, R); // 왼쪽 위로 이동
		H(H_tree, 4 * n + 1, i + v*(V[L][0] + V[D][0]), j + h*(V[L][1] + V[D][1]), h / 2, v / 2, U, D, R, L); // 왼쪽 아래로 이동
	}
	if (2 * n + 1 <= size) // 오른쪽 자식의 경우
	{
		for (int index = 1; index < h; index++) // 오른쪽으로 #
			H_tree[i + index*V[R][0]][j + index*V[R][1]] = '#';
		for (int index = 1; index < v; index++) // 오른쪽 위아래로 #
		{
			H_tree[i + v * V[R][0] + index * V[U][0]][j + v * V[R][1] + index * V[U][1]] = '#';
			H_tree[i + v * V[R][0] + index * V[D][0]][j + v * V[R][1] + index * V[D][1]] = '#';
		}
		H_tree[i + h*V[R][0]][j + h*V[R][1]] = node[2 * n + 1].getData(); // 오른쪽 자식 저장
		H(H_tree, 4 * n + 2, i + v*(V[R][0] + V[D][0]), j + h*(V[R][1] + V[D][1]), h / 2, v / 2, U, D, R, L); // 오른쪽 아래로 이동
		H(H_tree, 4 * n + 3, i + v*(V[R][0] + V[U][0]), j + h*(V[R][1] + V[U][1]), h / 2, v / 2, D, U, L, R); // 오른쪽 위로 이동
	}
}

/* 2차원 배열을 이용한 H-tree form 출력 */
void HeapArray::print3()
{
	int side = H_center(size) * 2 + 1; // 배열 한 변의 길이

	/* 배열 크기를 변수로 받기 위해 동적 할당 */
	char** H_tree = new char*[side];
	for (int i = 0; i < side; i++)
	{
		H_tree[i] = new char[side];
		memset(H_tree[i], ' ', side);
	}

	/* 배열 채운 후 출력 */
	H(H_tree, 1, H_center(size), H_center(size), hor_depth(size), vert_depth(size), 0, 1, 2, 3);
	for (int i = 0; i < side; i++)
	{
		for (int j = 0; j < side; j++)
			cout << H_tree[i][j];
		cout << endl;
	}

	/* 동적 할당 해제 */
	for (int i = 0; i < side; i++)
		delete[] H_tree[i];
	delete[] H_tree;
}

int main()
{
	ifstream inStream("input.txt"); // 파일 입력
	if (inStream.fail()) // 파일 열기 실패 시 종료
	{
		cout << "Input file opening failed.\n";
		exit (1);
	}

	int size; // 연산 횟수
	string operations; // 연산 문자열

	inStream >> size; // 첫 번째 줄 입력

	if (size > 150) // 범위 초과 시
		size = 150; // 150 저장

	while (!inStream.eof()) // 파일의 끝까지
	{
		/* 공백 문자를 제거한 나머지 문자를 문자열에 저장*/
		string temp;
		inStream >> temp;
		operations += temp;
	}

	HeapArray myHeap; // 힙 생성
	int i = 0; // 문자열 인덱스
	for (int n = 0; n < size; n++) // 파일의 정수 값만큼 연산
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

	/* 출력 */
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