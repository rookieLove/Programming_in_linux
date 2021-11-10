#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void do_sig_child(int signo)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(0, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status))
            printf("------------child %d exit %d\n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
    }
}

int main(void)
{
    pid_t pid;
    int i;
    //阻塞SIGCHLD
    for (i = 0; i < 10; i++) {
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
            sys_err("fork error");
    }

    if (pid == 0) {     //10个子进程
	int n= 1;
	while(n--){
	        printf("child ID %d\n", getpid());
    //    	sleep(1);
	}
        return i+1;
    } else if (pid > 0) { // 父进程
        //SIGCHLD阻塞
		// 注册捕捉函数
        struct sigaction act;
        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
		/*
		子进程结束时，父进程会收到SIGCHILD信号。
		默认动作为忽略这个信号,此处我们利用自定义函数打印子进程
		的回收状态
		*/
        sigaction(SIGCHLD, &act, NULL);
        //解除对SIGCHLD的阻塞
        
        while (1) {
            printf("Parent ID %d\n", getpid());
            sleep(1);
        }
    }

    return 0;
}

