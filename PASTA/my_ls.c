#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <strings.h>

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
    int s;
    // Não consegui implementar uma função que crie o BACKUP no diretorio ../
    // quando o diretorio é passado por parametro,
    // então vou criar ele no /BACKUP
    mkdir("/BACKUP", 7777);

    return 0;
}

int busca(char *filename, char *backup_path){
    struct dirent **backuplist;
    char backup_file[256];
    strcpy(backup_file, backup_path);
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
            strcpy(backup_path, backup_file);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    struct dirent **namelist;
    struct dirent **backuplist;
    int n, i, s, t;
    struct stat buf;
    struct stat backup_buf;
    char backup_path[] = "../BACKUP/";
    char backup_file[] = "../BACKUP/";

    if (argc == 1) {
        n = scandir(".", &namelist, NULL, alphasort);
        s = create_backup(backup_path);
        t = scandir(backup_path, &backuplist, NULL, alphasort);
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
            busca(namelist[i]->d_name, backup_path);
//
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

