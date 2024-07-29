#include <cstring>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

void (*signal(int signal_num, void(*func)(int)))(int);
int dup(int filedes);
int dup2(int filedes, int filedes2);
int chdir(const char *path);

void catch_signal(int sig_num)
{
	cout<<endl;
	cout<<"by !"<<endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	signal(SIGINT, catch_signal);
	while(true)
	{
		char cmd[100];
		char* param[20];
		char* param2[20];
		int ind = 0;
		cout<<"go!>> ";
		cin.getline(cmd, 100);
		if (strcmp(cmd,"exit") == 0) break;
		param[ind] = strtok(cmd, " ");
		while(param[ind] != NULL)	
		{
			param[++ind] = strtok(NULL, " ");
		}
		bool flag = false;
		bool flag2 = false;
		char* file_name;
		for(int i = 0; i < ind; i++)
		{
			if (strcmp(param[i], ">") == 0)
			{
				flag = true;
				file_name = param[i+1];
				param[i] = NULL;
				break;
			}
			else if(strcmp(param[i], "|") == 0)
			{
				flag2 = true;
				int j = i+1;
				while(param[j] != NULL)
				{
					param2[j-i-1] = param[j];
					j++;
				}
				param[i] = NULL;
				param2[j-i-1] = NULL;
				break;
			}

		}
		if(flag2)
		{
			for(int i = 0; i <= ind; i++)
			{
				printf("param[%d] = %s\n", i, param[i]);
			}
			for(int i = 0; i <= ind; i++)
			{
				printf("param2[%d] = %s\n", i, param2[i]);
			}
			int pipe(int filesdes[2]);
			int pp[2];
			pipe(pp);
			if(!fork())
			{
				dup2(pp[1], 1);
				close(pp[0]);
				close(pp[1]);
				execvp(param[0], param);
			}
			if(!fork())
			{
				dup2(pp[0], 0);
				close(pp[0]);
				close(pp[1]);
				execvp(param2[0], param2);
			}
			close(pp[0]);
			close(pp[1]);
			wait(0);
			wait(0);
			continue;
		}
		if (strcmp(param[0], "echo") == 0)
		{
			cout<<"Вывод"<<endl;
			for(int i = 1; i < ind; i++)
			{
				cout<<param[i];
			}
		}
		if (strcmp(param[0], "cd") == 0)
		{
			cout<<"Смена каталога"<<endl;
			chdir(param[1]);
			continue;
		}
		if (fork() == 0)
		{	
			if (flag)
			{
				cout<<"Перенаправление потоков"<<endl;
				int fd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0644);
				dup2(fd, 1);
				close(fd);	
			}
			cout<<"Команда"<<endl;
			execvp(param[0], param);
			cout<<"Error !";
			return 0;
		}
		wait(0);
	}
	return 0;
}
