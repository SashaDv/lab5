#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

void f1();
void f2();

LONG a[10];

int main() {
	for (int i = 0; i < 10; i++) a[i] = 0;
	HANDLE hMutex = CreateMutex(NULL, FALSE, "MyMutex");
	if (hMutex == NULL)
		cout << "Create mutex failed" << GetLastError() << endl;
	HANDLE thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f1, NULL, 0, 0);
	if (thread1 == NULL) return GetLastError();
	HANDLE thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f2, NULL, 0, 0);
	if (thread2 == NULL) return GetLastError();
	cin.get();
	CloseHandle(hMutex);
	TerminateThread(thread1, 0);
	CloseHandle(thread1);
	TerminateThread(thread2, 0);
	CloseHandle(thread2);
	return 0;
}

void f1() {
	HANDLE hMutex01 = OpenMutex(SYNCHRONIZE,
		FALSE, "MyMutex");
	if (hMutex01 == NULL)
		cout << "Open mutex01 failed" << GetLastError() << endl;
	srand(time_t(NULL));
	while (true)
	{
		WaitForSingleObject(hMutex01, INFINITE);
		cout << "t1: ";
		for (int i = 0; i < 10; i++) {
			a[i] = rand() * 300 / (RAND_MAX)-150;
			cout << a[i] << " ";
			Sleep(220);
		}
		cout << endl;
		ReleaseMutex(hMutex01);
	}
	CloseHandle(hMutex01);
	return;
}

void f2() {
	HANDLE hMutex02 = OpenMutex(SYNCHRONIZE,
		FALSE, "MyMutex");
	if (hMutex02 == NULL)
		cout << "Open mutex02 failed" << GetLastError() << endl;
	srand(time_t(NULL));
	while (true)
	{
		WaitForSingleObject(hMutex02, INFINITE);
		cout << "t2: ";
		int sum = 0;
		for (int i = 0; i < 9; i++, sum += a[i]) {
			cout << a[i] << " ";
			Sleep(100);
		}
		cout << sum << endl << endl;
		ReleaseMutex(hMutex02);
	}
	CloseHandle(hMutex02);
	return;
}