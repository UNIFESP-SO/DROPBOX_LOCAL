#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <pthread.h>

#define FILE_NAME_SIZE 512

char backup_path[] = "../BACKUP/";

int copy_file(char *origem) {
	int fd_o, fd_d;
	char destino[FILE_NAME_SIZE];
	fd_o = open(origem, O_RDONLY);
	if (fd_o == -1) {
		perror("open()");
		return 0;
	}
	strcpy(destino, backup_path);
    strcat(destino, origem);
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
	return 1;
}

void *thread_copia(void *args) {
	int *s;
	s = (int *)calloc(1, sizeof(int));
	*s = copy_file((char *)args);
	pthread_exit(s);
}

void print_stat(struct stat buf){
	printf ("\n\t inode: %lld \
		 \n\t acesso: %s \
	 modif:  %s \
	 status: %s",
		(long long) buf.st_ino,
		ctime(&buf.st_atime),
		ctime(&buf.st_mtime),
		ctime(&buf.st_ctime));
}

#define print_type(t) printf(" %s ", #t);

void print_type_name(unsigned char type) {
	switch(type) {
		case DT_DIR: print_type(DT_DIR); break;
		case DT_REG: print_type(DT_REG); break;
	}
}

int create_backup(char *backup_path){
    int s;
    s = mkdir(backup_path, 0777);
    if(s < 0){
        perror("mkdir()");
        return -1;
    }
    return 0;
}

int create_backup_b(char *argv){
    //int s;
    // Não consegui implementar uma função que crie o BACKUP no diretorio ../
    // quando o diretorio é passado por parametro,
    // então vou criar ele no /BACKUP
    mkdir("/BACKUP", 7777);

    return 0;
}

int busca(char *filename){
    struct dirent **backuplist;
    int n, i;

    n = scandir(backup_path, &backuplist, NULL, alphasort);

    if (n < 0){
        perror("scandir()");
        return -1;
    }
    else {
        i = 2;
        while (i < n) {
            if(!strcmp(filename, backuplist[i]->d_name)){
                free(backuplist[i]);
                free(backuplist);
                return 1;
            }
            free(backuplist[i]);
            i++;
        }
    }
    return 0;
}

#define MAX_T 10
int main(int argc, char **argv) {
    struct dirent **namelist;
    struct dirent **backuplist;
    int n, i, b, j = 0;
    pthread_t tid[MAX_T];
    struct stat buf;

    if (argc == 1) {
        n = scandir(".", &namelist, NULL, alphasort);
        create_backup(backup_path);
        scandir(backup_path, &backuplist, NULL, alphasort);
    } else {
        n = scandir(argv[1], &namelist, NULL, alphasort);
    //    s = create_backup_b(argv[1]);
    }

    if (n < 0)
        perror("scandir()");
    else {
        i = 2;
        while (i < n) {
            stat(namelist[i]->d_name, &buf);
            b = busca(namelist[i]->d_name);
            if(b < 0){
                perror("busca()");
            }
            else if(b == 0){
                pthread_create(&tid[j], NULL, &thread_copia, &namelist[i]->d_name);
                j++;
            }
//            if(i < t){
//                printf("\n\\/--------------------BACKUP--------------------------\\/\n");
//                strcat(backup_file, backuplist[i]->d_name);
//                printf("\nbackup_file = %s\n", backup_file);
//                stat(backup_file, &backup_buf);
//                printf("%s", backuplist[i]->d_name);
//                printf(" - %lld\n", (long long)backuplist[i]->d_ino);
//                print_type_name(backuplist[i]->d_type);
//                print_stat(backup_buf);
//                strcpy(backup_file, backup_path);
//
//                printf("\nbackup_file final = %s\n", backup_file);
//                printf("\n/\\----------------------------------------------/\\\n");
//                free(backuplist[i]);
//            }

            free(namelist[i]);
            i++;
        }
        free(namelist);
        free(backuplist);
    }
    return 0;
}

