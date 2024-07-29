#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	char com = argv[1][0];
	int key = 0;
	if (argc >= 3) key = atoi(argv[2]);
	else key = 741;
	if(com == 'c')
	{
		// Создать блок разделяемой памяти с ключем 325, размером 1024 б.
		int fd = shmget(key, 1024, IPC_CREAT | 0644);
		if(fd < 0)
		{
			perror("Ошибка создания разделяемой памяти !");
			cout<<endl;
		}
		else cout<<"Разделяемая память создана !"<<endl;
		return 0;
	}
	else if(com == 's')
	{
		int fd = shmget(key, 0, 0);
		if(fd < 0)
		{
			perror("Ошибка записи сообщения !");
			cout<<endl;
		}
		else
		{// Соединить блок с переменной addr
			char *addr = (char*)shmat(fd, 0, 0);
			// Скопировать в addr строку "Hello!" . strcpy(addr, "Hello!");
			strcpy(addr, argv[3]);
			// Отсоединить блок от переменной addr
			shmdt((char*)addr);
			cout<<"Сообщение помещено в разделяемую память !"<<endl;
		}
		return 0;
	}
	else if(com == 'r')
	{
		int fd = shmget(key, 0, 0);
		if(fd < 0)
		{
			perror("Ошибка чтения сообщения !");
			cout<<endl; 
		}
		else
		{
			char *addr = (char*)shmat(fd, 0, 0);
			cout<<"Сообщение из разделяемой памяти:"<<endl;
			cout<<addr<<endl;
			shmdt((char*)addr);
		}
		return 0;
	}
	else if(com == 'd')
	{
		int fd = shmget(key, 0, 0);
		if(fd < 0)
		{
			perror("Ошибка удаления разделяемой памяти !");
			cout<<endl;
	}
		else
		{
			// Удалить блок разделяемой памяти
			shmctl(fd, IPC_RMID, 0);
			cout<<"Разделяемая память удалена !"<<endl;
		}
		return 0;
	}
}

