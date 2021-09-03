#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>

const unsigned short SER_PORT= 6666;
#define SER_IP "127.0.0.1"

int main(){
	/*1.建立socket*/
	int cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd == -1){
		perror("socket error");
		exit(1);
	}
	/*2.connect连接请求*/
	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	inet_pton(AF_INET, SER_IP, &ser_addr.sin_addr.s_addr);
	int ret = connect(cli_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	if(ret == -1){
		perror("connect error");
		exit(1);
	}
	/*5.read读数据*/
	while(1){
		char buf[1024];
		std::cin.getline(buf, sizeof(buf));
		write(cli_fd, buf, strlen(buf));
		int len = read(cli_fd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, len);
	}
	close(cli_fd);

	return 0;
}
