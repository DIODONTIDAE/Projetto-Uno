#include <stdio.h>
#include <dirent.h>
#include <string.h>

char* cd(char *filename, char *added);
void ls(char *filename);
void clscr(void)
{
   printf("\033[2J");
   printf("\033[0;0f");
}




int main()
{
    int mode=0;
    char filename[256];
    /*
    char tempname[20];
    while (mode<4)
    {
        clscr();
        scanf("%d",&mode);
        switch (mode)
        {
        case 1:{clscr();ls(filename);break;}
        case 2:{clscr();scanf("%s",tempname);strcat(tempname,"/");clscr();cd(filename,tempname);ls(filename);break;}
        case 3:{clscr();strcat(filename,"/..");ls(filename);break;}
        }
    }
    //ls(filename);*/
    scanf("%s",filename);
    ls(filename);
    return 0;
}
char* cd(char* filename,char* added)
{
    strcat(filename,added);
    return filename;
}

void ls(char* filename)
{
    DIR *dfd;
    struct dirent *dp;

    dfd=opendir(filename);
    while( (dp=readdir(dfd)) != NULL )
            printf("%10s.dir\n", dp->d_name);
    closedir(dfd);
}
