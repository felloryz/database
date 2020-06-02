#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

typedef struct {
    unsigned long long isbn;
    char bookAuthor[128];
    char bookTitle[128];
    int allBooks;
    int usedBooks;
} sBooks;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "rus");

    printf("-- Ը��� ����� ��4-22�. ���� ������. ������� 15. --\n\n");

    char tmp[128];
    FILE *booksFile = fopen("books.csv", "r");
    sBooks *booksPtr; //��������� ��� ��������� sBooks
    if (booksFile == NULL)
        printf("���� �� ������� �������");
    else {
        /* -- ���������� ����� -- */
        int numberLines = 0; //������� ���������� �����

        /* -- �������� ����� �� ������� -- */
        fseek(booksFile,0,SEEK_END);
        long pos = ftell(booksFile);
        if(pos > 0) {
            // ���� �� ������
            rewind(booksFile);
            while(!feof(booksFile)) { //���� �� ��������� ����� �����
            if(fgetc(booksFile) == '\n') //���� ������ �� ������ ������� ������
                numberLines++;
            }
            numberLines++;
            fclose(booksFile);

            /* -- ������ ����� ����� � ��������� -- */

            booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //������ ��� ������ ���������
            booksFile = fopen("books.csv", "r");
            for(int i = 0; i < numberLines; i++) {
                if((fgets(tmp, 128, booksFile)) != NULL) {
                    booksPtr[i].isbn = atoll(strtok(tmp, ";")); //�������������� ������ � ������� ����� ����� � ����������� ;
                    strcpy(booksPtr[i].bookAuthor, strtok(NULL, ";")); //����������� ������ �� �������� � ������� ���������
                    strcpy(booksPtr[i].bookTitle, strtok(NULL, ";"));
                    booksPtr[i].allBooks = atoi(strtok(NULL, ";"));
                    booksPtr[i].usedBooks = atoi(strtok(NULL, "\n"));
                    booksPtr = (sBooks *)realloc(booksPtr, (i+2)*sizeof(sBooks)); //����������� ������ ����� ������
                }
            }
            fclose(booksFile);
        }
        else {
            numberLines = 0;
            booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //������ ��� ������ ���������
        }

        printf("���������� ����� � ��������� %d\n", numberLines);
        for(int i = 0; i < numberLines; i++) {
            printf(">>> %llu;%s;%s;%d;%d\n", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
        }

        printf("\n�������:\n");
        printf("1 - �������� ����� �����\n");
        printf("2 - ������� ����� �� ������ ISBN\n");
        printf("7 - ������ ����� �������� �� ������ ISBN\n");
        printf("8 - ������� ����� �� �������� �� ������ ISBN\n");
        printf("12 - ��������� ������ ���������\n");
        printf("13 - ������� ������\n");

        /* -- ������ � ���������� -- */
        int command;
        while(1) {
            away:
            printf("\n������� �������: ");
            scanf("%d", &command);

            /* -- ���������� ����� ����� -- */
            if(command == 1) { //���������� ����� �����

                printf("\n������� ISBN: ");
                unsigned long long ISBN;
                scanf("%lld", &ISBN);
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        printf("����� ����� ��� ����!\n");
                        goto away;
                    }
                }

                char c = getchar();
                printf("������� ������: ");
                int j = 0;
                char author[64];
                while((c = getchar()) != '\n') {
                    author[j] = c;
                    j++;
                }
                author[j] = '\0';


                printf("������� �������� �����: ");
                j = 0;
                char title[64];
                while((c = getchar()) != '\n') {
                    title[j] = c;
                    j++;
                }
                title[j] = '\0';

                printf("������� ���� ��������? ");
                int numBooks;
                scanf("%d", &numBooks);

                booksPtr = (sBooks *)realloc(booksPtr, (numberLines+1)*sizeof(sBooks));
                booksPtr[numberLines].isbn = ISBN;
                    //printf("%llu\n", booksPtr[numberLines].isbn);
                strcpy(booksPtr[numberLines].bookAuthor, author);
                    //printf("%s\n", booksPtr[numberLines].bookAuthor);
                strcpy(booksPtr[numberLines].bookTitle, title);
                    //printf("%s\n", booksPtr[numberLines].bookTitle);
                booksPtr[numberLines].allBooks = numBooks;
                    //printf("%d\n", booksPtr[numberLines].allBooks);
                booksPtr[numberLines].usedBooks = numBooks;
                booksPtr = (sBooks *)realloc(booksPtr, (numberLines+1)*sizeof(sBooks));
                numberLines++;

                for(int j = numberLines-1; j < numberLines; j++) {
                    printf("\n>>> %llu;%s;%s;%d;%d", booksPtr[j].isbn, booksPtr[j].bookAuthor, booksPtr[j].bookTitle, booksPtr[j].allBooks, booksPtr[j].usedBooks);
                }

                booksFile = fopen("books.csv", "w"); //�� ���� �������������� ���� ����
                for(int i = 0; i < numberLines; i++) {
                    fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                    if(i != numberLines-1)
                        fprintf(booksFile, "\n");
                }
                fclose(booksFile);

                printf("\n����� ������� ���������!\n");
                command = 0;
                //goto away;
            }

            /* -- ���������� ����� -- */
            if(command == 12) {
                booksFile = fopen("books.csv", "w"); //�� ���� �������������� ���� ����
                for(int i = 0; i < numberLines; i++) {
                    fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                    if(i != numberLines-1)
                        fprintf(booksFile, "\n");
                }
                fclose(booksFile);
                printf("���� ������� �������! ������ � ���������� ���������\n");
                command = 0;
                goto end;
            }

            /* -- �������� ����� -- */
            if(command == 2) {
                printf("\n������� ISBN: ");
                unsigned long long ISBN;
                scanf("%llu", &ISBN);
                //printf("%d\n", ISBN);
                int position = -1;
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        position = i;
                        booksPtr[i].isbn = 0;
                    }
                }

                if(position == -1) {
                    printf("\n����� �� �������!\n");
                    goto away;
                }

                int k = position+1;
                //printf("%d %d", position+1, numberLines);
                printf("\n�������� ������ � %d \n", k);

                /* ��������� ���� */
                if(booksPtr[position].isbn == 0) {
                    booksFile = fopen("books.csv", "w");
                    for(int i = 0; i < numberLines; i++) {
                        if(booksPtr[i].isbn == 0) {
                            //printf("\n����� �������!");
                            continue;
                        }
                        else {
                            fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                            if(i != numberLines-1) {
                                if(position == numberLines-1 && i == position-1)
                                    printf(booksFile, "");
                                else
                                    fprintf(booksFile, "\n");
                            }
                        }
                    }
                    numberLines--;
                    printf("\n����� �������!\n");
                    fclose(booksFile);
                }

                /* ���������� ����� � ���������*/
                booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //������ ��� ������ ���������
                booksFile = fopen("books.csv", "r");
                for(int i = 0; i < numberLines; i++) {
                    if((fgets(tmp, 128, booksFile)) != NULL) {
                        booksPtr[i].isbn = atoll(strtok(tmp, ";")); //�������������� ������ � ������� ����� ����� � ����������� ;
                        strcpy(booksPtr[i].bookAuthor, strtok(NULL, ";")); //����������� ������ �� �������� � ������� ���������
                        strcpy(booksPtr[i].bookTitle, strtok(NULL, ";"));
                        booksPtr[i].allBooks = atoi(strtok(NULL, ";"));
                        booksPtr[i].usedBooks = atoi(strtok(NULL, "\n"));
                        booksPtr = (sBooks *)realloc(booksPtr, (i+2)*sizeof(sBooks)); //����������� ������ ����� ������
                    }
                }
                fclose(booksFile);
                command = 0;
                //printf("%d", numberLines);
                //goto away;
            }

            /* -- ���� ������ -- */
            if(command == 13) {
                printf("\n���������� ����� � ��������� %d\n", numberLines);
                for(int i = 0; i < numberLines; i++) {
                    printf(">>> %llu;%s;%s;%d;%d\n", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                }
                command = 0;
            }

            /* -- ������ ����� �� ������ ISBN -- */
            if(command == 7) {
                printf("\n������� ISBN: ");
                unsigned long long ISBN;
                scanf("%llu", &ISBN);
                int position = -1;
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        position = i;
                    }
                }
                if(position == -1) {
                    printf("\n����� �� �������!\n");
                    goto away;
                }
                if(booksPtr[position].usedBooks == 0)
                    printf("\n��������� ���� ���!\n");
                else {
                    booksPtr[position].usedBooks--;
                    printf("\n����� ������!\n");
                }
                command = 0;
            }

            /* -- ������� ����� �� ISBN -- */
            if(command == 8) {
                printf("\n������� ISBN: ");
                unsigned long long ISBN;
                scanf("%llu", &ISBN);
                int position = -1;
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        position = i;
                    }
                }
                if(position == -1) {
                    printf("\n����� �� �������!\n");
                    goto away;
                }
                if(booksPtr[position].usedBooks == booksPtr[position].allBooks)
                    printf("\n���������� ��������� ���� ����� ���������� ���� � ����������\n");
                else {
                    booksPtr[position].allBooks++;
                    printf("\n����� �������!\n");
                }
                command = 0;
            }
        }
    }
    end:
    return 0;
}
