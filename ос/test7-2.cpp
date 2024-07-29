#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    int key = atoi(argv[1]);
    int fd = shmget(key, 742, IPC_CREAT | 0644);

    if (fd < 0) {
        perror("Ошибка вызова shmget()");
        cout << endl;
        return 1;
    } else {
        cout << "Разделяемая память создана" << endl;
    }

    int* arr = (int*)shmat(fd, 0, 0);

    if (fork() == 0) {
        cout << "Первый начал!" << endl;
        int count = 0;

        while (arr[0] < 40) {
            if (arr[0] % 2 == 0) {
                arr[arr[0]] = 2;
                arr[0]++;
                count++;
            }
        }

        cout << "Первый закончил!" << endl;
        cout << "1-count: " << count << endl;
        return 0;
    }

    if (fork() == 0) {
        cout << "Второй начал!" << endl;
        int count = 0;

        while (arr[0] < 40) {
            if (arr[0] % 2 == 1) {
                arr[arr[0]] = 1;
                arr[0]++;
                count++;
            }
        }

        cout << "Второй закончил!" << endl;
        cout << "2-count: " << count << endl;
        return 0;
    }

    wait(0);
    wait(0);

    for (int i = 0; i < 40; i++) {
        cout << arr[i] << ", ";
    }

    cout << endl;

    shmctl(fd, IPC_RMID, 0);

    cout << "Разделяемая память удалена" << endl;
}

