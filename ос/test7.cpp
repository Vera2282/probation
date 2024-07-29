#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>

using namespace std;

int semfd;
struct sembuf sb;
void UpSem(int semno)
{
	sb.sem_num = semno;
	sb.sem_op = 1;
	semop(semfd, &sb, 1);
}

void DownSem(int semno)/*Уменьшает значение семафора semno*/
{
	sb.sem_num = semno;
	sb.sem_op = -1;
	semop(semfd, &sb, 1);
}

int main(int argc, char* argv[])
{
	semfd = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
	UpSem(0);/*Увеличение нулевого семафора (чтобы первый процесс смог начать работу)*/
	int key = atoi(argv[1]);
	int fd = shmget(key, 512, IPC_CREAT | 0644);
	if(fd < 0)/*Первый дочерний процесс*/
	{
		perror("Ошибка вызова shmget()");
		cout<<endl;
	}
	else cout<<"Разделяемая память создана"<<endl;
	int* arr = (int*)shmat(fd, 0, 0);
	if(fork() == 0)
	{
		cout<<"Первый начал !"<<endl;
		int count = 0;
		while(arr[0] < 40)
		{
			count++;
			DownSem(0);/*Выполнение критической секции*/
			arr[arr[0]] = 2;
			arr[0]++;
			UpSem(1);

		}
		cout<<"Первый закончил !"<<endl;
		cout<<"1-count: "<<count<<endl;
		return 0;
	}
	if(fork() == 0)/*Второй дочерний процесс*/
	{
		cout<<"Второй начал !"<<endl;
		int count = 0;
		while(arr[0] < 40)
		{
			count ++;
			DownSem(1);/*Выполнение критической секции*/
			arr[arr[0]] = 1;
			arr[0]++;
			UpSem(0);
		}
		cout<<"Второй закончил!"<<endl;
		cout<<"2-count: "<<count<<endl;
		return 0;
	}
	wait(0);
	wait(0);
	for(int i = 0; i < 40; i++)
	{
		cout<<arr[i]<<", ";
	}
	cout<<endl;
	shmctl(fd, IPC_RMID, 0);
	semctl(semfd, 0, IPC_RMID);/*Удаляем набор семафоров*/
	cout<<"Разделяемая память удалена"<<endl;
}
