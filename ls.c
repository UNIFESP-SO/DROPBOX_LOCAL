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
    struct dirent **namelist;
    struct dirent **backup_list;
    int m, n, i;
    struct stat buf;
    struct stat backup_buf;

    n = scandir("./PASTA", &namelist, NULL, alphasort);
    system("mkdir BACKUP_PASTA");
    system("cp -R -f ./PASTA/. BACKUP_PASTA");
    m = scandir("BACKUP_PASTA", &backup_list, NULL, alphasort);


    if (n < 0)
        perror("scandir()");
    else {
        i = 0;
        while (i < n) {
            printf("%s", namelist[i]->d_name);
            print_type_name(namelist[i]->d_type);
            stat(namelist[i]->d_name, &buf);
            print_stat(buf);
                free(namelist[i]);

            printf("BACKUP-->%s", backup_list[i]->d_name);
            print_type_name(backup_list[i]->d_type);
            stat(backup_list[i]->d_name, &backup_buf);
            print_stat(backup_buf);
                free(backup_list[i]);

	    i++;
        }
        free(namelist);
        free(backup_list);
    }
    return 0;
}

