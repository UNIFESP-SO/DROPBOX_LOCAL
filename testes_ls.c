#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_stat(struct stat buf){
	printf ("\n\t acesso: %s \
	 modif:  %s \
	 status: %s ",
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

int main(int argc, char **argv) {
    struct dirent **namelist_local;
    struct dirent **backup;
    int n, m, i, j=0;
    struct stat buf;
    struct stat b_backup;

    while(1){
        if (argc == 1) {
            n = scandir(".", &namelist_local, NULL, alphasort);
        } else {
            n = scandir(argv[1], &namelist_local, NULL, alphasort);
        }
        m = scandir("/home-local/aluno/BACKUP_DROPBOX/BACKUP_PASTA", &backup, NULL, alphasort);
        if (n < 0)
            perror("scandir()");
        else {
            i = 0;
            while (i < n) {
//                printf("%s", namelist_local[i]->d_name);
//                print_type_name(namelist_local[i]->d_type);
//                stat(namelist_local[i]->d_name, &buf);
//                print_stat(buf);
//                    free(namelist_local[i]);

                printf("BACKUP --> %s", backup[i]->d_name);
                print_type_name(backup[i]->d_type);
                stat(backup[i]->d_name, &b_backup);
                print_stat(b_backup);
                    free(backup[i]);
                i++;
            }
            free(namelist_local);
            free(backup);
        }
        printf("\n%d\n", j);
        j++;
        sleep(1);
        system("clear");
    }
    return 0;
}

