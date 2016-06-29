#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("uso: %s <arq_origem> <arq_destino>\n", argv[0]);
		return 0;
	}
	char *origem = argv[1];
	char *destino = argv[2];
	int fd_o, fd_d;
	fd_o = open(origem, O_RDONLY);
	if (fd_o == -1) {
		perror("open()");
		return 0;
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd_d = open(destino, O_CREAT | O_RDWR | O_APPEND, mode);
	if(fd_d == -1) {
		perror("open()");
		close(fd_o);
		return 0;
	}
#define BLOCO 4096
	int nr, ns, nw, n;
	unsigned char buffer[BLOCO];
	void *ptr_buff;
	do {
		nr = read(fd_o, buffer, BLOCO);
		if (nr == -1) {
			perror("read()");
			close(fd_o);
			close(fd_d);
			return 0;
		}
		else if (nr > 0) {
			ptr_buff = buffer;
			nw = nr;
			ns = 0;
			do {
				n = write(fd_d, ptr_buff + ns, nw);
				if (n == -1) {
					perror("write()");
                	        	close(fd_o);
	        	                close(fd_d);
		                        return 0;
				}
				ns += n;
				nw -= n;
			} while (nw > 0);
		}
	}while(nr > 0);
	close(fd_o);
	close(fd_d);
	return 0;
}
