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
    mkdir("/BACKUP", 0777);

    return 0;
}

int busca(struct dirent *namelist, char *backup_path){
    struct dirent **backuplist;
    struct stat buf;
    int n, i;

    n = scandir(backup_path, &backuplist, NULL, alphasort);

    if (n < 0)
        perror("scandir()");
    else {
        i = 0;
        while (i < n) {
            printf("NAME -> |%s|", backuplist[i]->d_name);
            printf("\n");
            printf("BACKUP -> |%lld|", (long long)backuplist[i]->d_ino);
            print_type_name(backuplist[i]->d_type);
            stat(backuplist[i]->d_name, &buf);
            print_stat(buf);
            if(backuplist[i]->d_ino == namelist->d_ino)
                printf(" --> DEU ZERO\n ");
            //check()
            free(backuplist[i]);
            i++;
        }
    }
}

int main(int argc, char **argv) {
    struct dirent **namelist;
    struct dirent **backuplist;
    int n, i, s, t;
    struct stat buf;
    char backup_path[] = "../BACKUP";
    char mapa[2][100][512];

    if (argc == 1) {
        n = scandir(".", &namelist, NULL, alphasort);
        s = create_backup(backup_path);
    } else {
        n = scandir(argv[1], &namelist, NULL, alphasort);
        s = create_backup_b(argv[1]);
    }

    if (n < 0)
        perror("scandir()");
    else {
        i = 0;
        while (i < n) {
            t = busca(namelist[i], backup_path);

//            print_type_name(namelist[i]->d_type);
//            stat(namelist[i]->d_name, &buf);
//            print_stat(buf);



            printf("!%s!", namelist[i]->d_name);
            printf(" - !%lld!", (long long)namelist[i]->d_ino);
            if(!strcmp(namelist[i]->d_name, "ls.c")){
                printf("\nRENOMEANDO\n");
                //system("mv ls.c ls_2.c");
            }
            print_type_name(namelist[i]->d_type);
            stat(namelist[i]->d_name, &buf);
            print_stat(buf);

            //check()
            free(namelist[i]);
            i++;
        }
        free(namelist);
    }
    return 0;
}

