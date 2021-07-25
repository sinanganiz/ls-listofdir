#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include "jrb.h"
#include "jval.h"
#include <stdbool.h>

bool is_hidden(const char *name);
void red();
void yellow();
void purple();
void blue();
void green();
void cyan();
void reset();

int main(int argc, char *argv[])
{
        char *pathInput = argv[1];
        char *orderInput = argv[2];

        //type; 0-> SADECE okunabilir
        //type; 1-> klasor
        //type; 2-> duzenli dosya
        //type; 3-> softlink
        // 0 -> type
        // 1 -> name
        // 2 -> size
        // 3 -> path
        char listOfDir[1000][4][300];

        // 0 -> name
        // 1 -> size
        // 2 -> path
        char listOfHiddens[1000][3][300];

        // 0 -> READ ONLY
        // 1 -> Directory
        // 2 -> RegularFile
        // 3 -> Softlink
        // 4 -> Hidden Files
        int sumOfType[5] = {0, 0, 0, 0, 0};

        struct stat buf;
        int exists;
        DIR *d;
        struct dirent *de;
        JRB r, tmp;
        char *fn;

        r = make_jrb();
        d = opendir(pathInput);
        if (d == NULL)
        {
                fprintf(stderr, "Couldn't open \".\"\n");
                exit(1);
        }

        for (de = readdir(d); de != NULL; de = readdir(d))
        {
                fn = strdup(de->d_name);
                jrb_insert_str(r, fn, JNULL);
        }
        closedir(d);

        int indexOthers = 0;
        int indexHiddens = 0;
        jrb_traverse(tmp, r)
        {
                char _tempKeys[200];
                strcpy(_tempKeys, pathInput);
                strcat(_tempKeys, "/");
                strcat(_tempKeys, tmp->key.s);

                exists = lstat(_tempKeys, &buf);
                if (exists < 0)
                {
                        char custom_temp[strlen(tmp->key.s)];
                        strcpy(custom_temp, tmp->key.s);
                        strcat(custom_temp, " not found\n");
                        write(2, custom_temp, strlen(custom_temp));
                }
                else
                {

                        if (buf.st_mode && is_hidden(tmp->key.s))
                        {

                                char _bufSize[100];
                                sprintf(_bufSize, "%ld", buf.st_size);

                                char _bufAbsPath[200];
                                sprintf(_bufAbsPath, "%s", realpath(_tempKeys, NULL));

                                strcpy(listOfHiddens[indexHiddens][0], tmp->key.s);
                                strcpy(listOfHiddens[indexHiddens][1], _bufSize);
                                strcpy(listOfHiddens[indexHiddens][2], _bufAbsPath);

                                indexHiddens++;

                                sumOfType[4]++;
                        }

                        if ((buf.st_mode & (S_IRUSR | S_IRGRP | S_IROTH)) && !(buf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) && !(buf.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH)))
                        {

                                char _bufSize[100];
                                sprintf(_bufSize, "%ld", buf.st_size);

                                char _bufAbsPath[200];
                                sprintf(_bufAbsPath, "%s", realpath(_tempKeys, NULL));

                                strcpy(listOfDir[indexOthers][0], "0");
                                strcpy(listOfDir[indexOthers][1], tmp->key.s);
                                strcpy(listOfDir[indexOthers][2], _bufSize);
                                strcpy(listOfDir[indexOthers][3], _bufAbsPath);

                                indexOthers++;

                                sumOfType[0]++;
                        }
                        else if (S_ISDIR(buf.st_mode))
                        {

                                char _bufSize[100];
                                sprintf(_bufSize, "%ld", buf.st_size);

                                char _bufAbsPath[200];
                                sprintf(_bufAbsPath, "%s", realpath(_tempKeys, NULL));

                                strcpy(listOfDir[indexOthers][0], "1");
                                strcpy(listOfDir[indexOthers][1], tmp->key.s);
                                strcpy(listOfDir[indexOthers][2], _bufSize);
                                strcpy(listOfDir[indexOthers][3], _bufAbsPath);

                                indexOthers++;

                                sumOfType[1]++;
                        }
                        else if (S_ISREG(buf.st_mode))
                        {

                                char _bufSize[100];
                                sprintf(_bufSize, "%ld", buf.st_size);

                                char _bufAbsPath[200];
                                sprintf(_bufAbsPath, "%s", realpath(_tempKeys, NULL));

                                strcpy(listOfDir[indexOthers][0], "2");
                                strcpy(listOfDir[indexOthers][1], tmp->key.s);
                                strcpy(listOfDir[indexOthers][2], _bufSize);
                                strcpy(listOfDir[indexOthers][3], _bufAbsPath);

                                indexOthers++;

                                sumOfType[2]++;
                        }
                        else if (S_ISLNK(buf.st_mode) && !S_ISREG(buf.st_mode))
                        {
                                char _bufSize[100];
                                sprintf(_bufSize, "%ld", buf.st_size);

                                char _bufAbsPath[200];
                                sprintf(_bufAbsPath, "%s", realpath(_tempKeys, NULL));

                                strcpy(listOfDir[indexOthers][0], "3");
                                strcpy(listOfDir[indexOthers][1], tmp->key.s);
                                strcpy(listOfDir[indexOthers][2], _bufSize);
                                strcpy(listOfDir[indexOthers][3], _bufAbsPath);

                                indexOthers++;

                                sumOfType[3]++;
                        }
                }
        }

        //sorting
        switch (*orderInput)
        {
        case '0': // A-Z, boyuta göre
                write(1, "\nBoyuta Göre A-Z Sıralama Yapılmıştır \n", strlen("\nBoyuta Göre A-Z Sıralama Yapılmıştır \n"));

                //OTHER LIST
                for (int i = 0; i < indexOthers - 1; i++)
                {
                        long int sizePresent = atoi(listOfDir[i][2]);

                        for (int j = i + 1; j < indexOthers; j++)
                        {
                                long int sizeNext = atoi(listOfDir[j][2]);
                                if (sizePresent < sizeNext)
                                {

                                        char _tmpChangeArr[4][300];
                                        strcpy(_tmpChangeArr[0], listOfDir[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfDir[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfDir[i][2]);
                                        strcpy(_tmpChangeArr[3], listOfDir[i][3]);

                                        strcpy(listOfDir[i][0], listOfDir[j][0]);
                                        strcpy(listOfDir[i][1], listOfDir[j][1]);
                                        strcpy(listOfDir[i][2], listOfDir[j][2]);
                                        strcpy(listOfDir[i][3], listOfDir[j][3]);

                                        strcpy(listOfDir[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfDir[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfDir[j][2], _tmpChangeArr[2]);
                                        strcpy(listOfDir[j][3], _tmpChangeArr[3]);

                                        sizePresent = sizeNext;
                                }
                        }
                }
                //HIDDEN LIST
                for (int i = 0; i < indexHiddens - 1; i++)
                {
                        long int sizePresent = atoi(listOfHiddens[i][1]);

                        for (int j = i + 1; j < indexHiddens; j++)
                        {
                                long int sizeNext = atoi(listOfHiddens[j][1]);
                                if (sizePresent < sizeNext)
                                {

                                        char _tmpChangeArr[3][300];
                                        strcpy(_tmpChangeArr[0], listOfHiddens[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfHiddens[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfHiddens[i][2]);

                                        strcpy(listOfHiddens[i][0], listOfHiddens[j][0]);
                                        strcpy(listOfHiddens[i][1], listOfHiddens[j][1]);
                                        strcpy(listOfHiddens[i][2], listOfHiddens[j][2]);

                                        strcpy(listOfHiddens[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfHiddens[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfHiddens[j][2], _tmpChangeArr[2]);

                                        sizePresent = sizeNext;
                                }
                        }
                }

                break;
        case '1': // Z-A boyuta göre
                write(1, "\nBoyuta Göre Z-A Sıralama Yapılmıştır \n", strlen("\nBoyuta Göre Z-A Sıralama Yapılmıştır \n"));

                //OTHER LIST
                for (int i = 0; i < indexOthers - 1; i++)
                {
                        long int sizePresent = atoi(listOfDir[i][2]);

                        for (int j = i + 1; j < indexOthers; j++)
                        {
                                long int sizeNext = atoi(listOfDir[j][2]);
                                if (sizePresent > sizeNext)
                                {

                                        char _tmpChangeArr[4][300];
                                        strcpy(_tmpChangeArr[0], listOfDir[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfDir[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfDir[i][2]);
                                        strcpy(_tmpChangeArr[3], listOfDir[i][3]);

                                        strcpy(listOfDir[i][0], listOfDir[j][0]);
                                        strcpy(listOfDir[i][1], listOfDir[j][1]);
                                        strcpy(listOfDir[i][2], listOfDir[j][2]);
                                        strcpy(listOfDir[i][3], listOfDir[j][3]);

                                        strcpy(listOfDir[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfDir[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfDir[j][2], _tmpChangeArr[2]);
                                        strcpy(listOfDir[j][3], _tmpChangeArr[3]);

                                        sizePresent = sizeNext;
                                }
                        }
                }
                //HIDDEN LIST
                for (int i = 0; i < indexHiddens - 1; i++)
                {
                        long int sizePresent = atoi(listOfHiddens[i][1]);

                        for (int j = i + 1; j < indexHiddens; j++)
                        {
                                long int sizeNext = atoi(listOfHiddens[j][1]);
                                if (sizePresent > sizeNext)
                                {

                                        char _tmpChangeArr[3][300];
                                        strcpy(_tmpChangeArr[0], listOfHiddens[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfHiddens[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfHiddens[i][2]);

                                        strcpy(listOfHiddens[i][0], listOfHiddens[j][0]);
                                        strcpy(listOfHiddens[i][1], listOfHiddens[j][1]);
                                        strcpy(listOfHiddens[i][2], listOfHiddens[j][2]);

                                        strcpy(listOfHiddens[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfHiddens[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfHiddens[j][2], _tmpChangeArr[2]);

                                        sizePresent = sizeNext;
                                }
                        }
                }
                break;
        case '2': // A-Z isme göre
                write(1, "\nİsme Göre A-Z Sıralama Yapılmıştır \n", strlen("\nİsme Göre A-Z Sıralama Yapılmıştır \n"));

                for (int i = 0; i <= indexOthers; i++)
                        for (int j = i + 1; j <= indexOthers; j++)
                        {
                                //listOfDir[j][0]
                                if (strcmp(listOfDir[i][1], listOfDir[j][1]) > 0)
                                {
                                        // strcpy(temp, str[i]);
                                        // strcpy(str[i], str[j]);
                                        // strcpy(str[j], temp);

                                        char _tmpChangeArr[4][300];
                                        strcpy(_tmpChangeArr[0], listOfDir[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfDir[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfDir[i][2]);
                                        strcpy(_tmpChangeArr[3], listOfDir[i][3]);

                                        strcpy(listOfDir[i][0], listOfDir[j][0]);
                                        strcpy(listOfDir[i][1], listOfDir[j][1]);
                                        strcpy(listOfDir[i][2], listOfDir[j][2]);
                                        strcpy(listOfDir[i][3], listOfDir[j][3]);

                                        strcpy(listOfDir[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfDir[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfDir[j][2], _tmpChangeArr[2]);
                                        strcpy(listOfDir[j][3], _tmpChangeArr[3]);
                                }
                        }

                //HIDDEN LIST

                for (int i = 0; i <= indexHiddens; i++)
                        for (int j = i + 1; j <= indexHiddens; j++)
                        {
                                if (strcmp(listOfHiddens[i][0], listOfHiddens[j][0]) > 0)
                                {
                                        char _tmpChangeArr[3][300];
                                        strcpy(_tmpChangeArr[0], listOfHiddens[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfHiddens[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfHiddens[i][2]);

                                        strcpy(listOfHiddens[i][0], listOfHiddens[j][0]);
                                        strcpy(listOfHiddens[i][1], listOfHiddens[j][1]);
                                        strcpy(listOfHiddens[i][2], listOfHiddens[j][2]);

                                        strcpy(listOfHiddens[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfHiddens[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfHiddens[j][2], _tmpChangeArr[2]);
                                }
                        }
                break;
        case '3': // Z-A isme göre
                write(1, "\nİsme Göre Z-A Sıralama Yapılmıştır \n", strlen("\nİsme Göre Z-A Sıralama Yapılmıştır \n"));

                for (int i = 0; i <= indexOthers; i++)
                        for (int j = i + 1; j <= indexOthers; j++)
                        {
                                //listOfDir[j][0]
                                if (strcmp(listOfDir[i][1], listOfDir[j][1]) < 0)
                                {

                                        char _tmpChangeArr[4][300];
                                        strcpy(_tmpChangeArr[0], listOfDir[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfDir[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfDir[i][2]);
                                        strcpy(_tmpChangeArr[3], listOfDir[i][3]);

                                        strcpy(listOfDir[i][0], listOfDir[j][0]);
                                        strcpy(listOfDir[i][1], listOfDir[j][1]);
                                        strcpy(listOfDir[i][2], listOfDir[j][2]);
                                        strcpy(listOfDir[i][3], listOfDir[j][3]);

                                        strcpy(listOfDir[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfDir[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfDir[j][2], _tmpChangeArr[2]);
                                        strcpy(listOfDir[j][3], _tmpChangeArr[3]);
                                }
                        }

                //HIDDEN LIST

                for (int i = 0; i <= indexHiddens; i++)
                        for (int j = i + 1; j <= indexHiddens; j++)
                        {
                                if (strcmp(listOfHiddens[i][0], listOfHiddens[j][0]) < 0)
                                {
                                        char _tmpChangeArr[3][300];
                                        strcpy(_tmpChangeArr[0], listOfHiddens[i][0]);
                                        strcpy(_tmpChangeArr[1], listOfHiddens[i][1]);
                                        strcpy(_tmpChangeArr[2], listOfHiddens[i][2]);

                                        strcpy(listOfHiddens[i][0], listOfHiddens[j][0]);
                                        strcpy(listOfHiddens[i][1], listOfHiddens[j][1]);
                                        strcpy(listOfHiddens[i][2], listOfHiddens[j][2]);

                                        strcpy(listOfHiddens[j][0], _tmpChangeArr[0]);
                                        strcpy(listOfHiddens[j][1], _tmpChangeArr[1]);
                                        strcpy(listOfHiddens[j][2], _tmpChangeArr[2]);
                                }
                        }
                break;
        }
        //sorting end
        ///////////////////////////////////////////

        write(1, "\nTüm Dosyalar Listeleniyor\n", strlen("\nTüm Dosyalar Listeleniyor\n"));

        //Other Files Listing
        for (int i = 0; i <= indexOthers; i++)
        {

                char tempArr[5000];
                strcpy(tempArr, "");

                if (listOfDir[i][0][0] == '0')
                {
                        blue();
                        sprintf(tempArr, "[Read-Only]\t %-25s \t %-10s\t %s \n", listOfDir[i][1], listOfDir[i][2], listOfDir[i][3]);
                }
                else if (listOfDir[i][0][0] == '1')
                {
                        green();
                        sprintf(tempArr, "[Directory]\t %-25s \t %-10s\t %s \n", listOfDir[i][1], listOfDir[i][2], listOfDir[i][3]);
                }
                else if (listOfDir[i][0][0] == '2')
                {
                        red();
                        sprintf(tempArr, "[Regular File]\t %-25s \t %-10s\t %s \n", listOfDir[i][1], listOfDir[i][2], listOfDir[i][3]);
                }
                else if (listOfDir[i][0][0] == '3')
                {
                        purple();
                        sprintf(tempArr, "[Link]\t\t %-25s \t %-10s\t %s \n", listOfDir[i][1], listOfDir[i][2], listOfDir[i][3]);
                }

                write(1, tempArr, strlen(tempArr));
                reset();
        }

        write(1, "\nGizli Dosyalar Listeleniyor\n", strlen("\nGizli Dosyalar Listeleniyor\n"));

        //Hidden Files Listing
        yellow();
        for (int i = 0; i < indexHiddens; i++)
        {
                char tempArr[5000];
                strcpy(tempArr, "");

                sprintf(tempArr, "[Hidden]\t %-25s \t %-10s\t %s \n", listOfHiddens[i][0], listOfHiddens[i][1], listOfHiddens[i][2]);
                write(1, tempArr, strlen(tempArr));
        }
        reset();


        char summary[100];
        strcpy(summary, "");
        sprintf(summary, "\n[Çıktı Özeti]\nGizli Dosyalar(Toplama Katılmaz): %d\nSadece Okunabilir Dosyalar: %d\nKlasörler: %d\nDüzenli Dosyalar: %d\nLink Dosyaları: %d\nTüm Dosyalar: %d\n", sumOfType[4], sumOfType[0], sumOfType[1], sumOfType[2], sumOfType[3], (sumOfType[0] + sumOfType[1] + sumOfType[2] + sumOfType[3]));
        write(1, "\033[36m", strlen("\033[36m"));
        write(1, summary, strlen(summary));
}

bool is_hidden(const char *name)
{
        if (name[0] == '.')
        {
                return true;
        }
        return false;
}
void red()
{
        write(1, "\033[1;31m", strlen("\033[1;31m"));
}

void yellow()
{
        write(1, "\033[1;33m", strlen("\033[1;33m"));
}
void purple()
{
        write(1, "\033[0;35m", strlen("\033[0;35m"));
}
void blue()
{
        write(1, "\033[0;34m", strlen("\033[0;34m"));
}
void green()
{
        write(1, "\033[0;32m", strlen("\033[0;32m"));
}
void cyan()
{
        write(1, "\033[0;36m", strlen("\033[0;36m"));
}

void reset()
{
        write(1, "\033[0m", strlen("\033[0m"));
}