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
	/* -- КИРИЛЛИЦА -- */
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "rus");

    printf("\n-- Фёдор Рыжов ИУ4-22Б. База данных. Вариант 15. --\n\n");

    char tmp[128]; //переменная для временной записи
    FILE *booksFile = fopen("books.csv", "r");
    sBooks *booksPtr; //указатель для структуры sBooks
    if (booksFile == NULL)
        printf("Файл не удалось открыть");
    else {

        /* -- КОЛИЧЕСТВО СТРОК -- */
        int numberLines = 0; //счётчик количества строк

        /* -- ПРОВЕРКА ФАЙЛА НА ПУСТОТУ -- */
        fseek(booksFile,0,SEEK_END);
        long pos = ftell(booksFile);
        if(pos > 0) {
            // файл не пустой
            rewind(booksFile);
            while(!feof(booksFile)) { //пока не достигнут конец файла
            if(fgetc(booksFile) == '\n') //если символ из потока перенос строки
                numberLines++;
            }
            numberLines++;
            fclose(booksFile);

            /* -- ЗАПИСЬ ВСЕГО ФАЙЛА В СТРУКТУРУ -- */
            booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //память для первой структуры
            booksFile = fopen("books.csv", "r"); //открываем файл на чтение
            for(int i = 0; i < numberLines; i++) {
                if((fgets(tmp, 128, booksFile)) != NULL) {
                    booksPtr[i].isbn = atoll(strtok(tmp, ";")); //преобразование строки в длинное целое число с разделением ";"
                    strcpy(booksPtr[i].bookAuthor, strtok(NULL, ";")); //копирование строки из символов в атрибут структуры
                    strcpy(booksPtr[i].bookTitle, strtok(NULL, ";"));
                    booksPtr[i].allBooks = atoi(strtok(NULL, ";"));
                    booksPtr[i].usedBooks = atoi(strtok(NULL, "\n"));
                    booksPtr = (sBooks *)realloc(booksPtr, (i+2)*sizeof(sBooks)); //увеличиваем размер блока памяти
                }
            }
            fclose(booksFile);
        }
        else {
			// файл пустой
            numberLines = 0; //строк нет
            booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //память для первой структуры
        }

		/* -- СПИСОК ВСЕХ ФАЙЛОВ -- */
        printf("Количество строк в документе %d\n", numberLines);
        for(int i = 0; i < numberLines; i++) {
            printf(">>> %llu;%s;%s;%d;%d\n", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
        }

		/* -- МЕНЮ КОМАНД -- */
        printf("\nКоманды:\n");
        printf("1 - Добавить новую книгу\n");
        printf("2 - Удалить книгу по номеру ISBN\n");
        printf("7 - Выдать книгу студенту по номеру ISBN\n");
        printf("8 - Принять книгу от студента по номеру ISBN\n");
        printf("12 - Завершить работу программы\n");
        printf("13 - Вывести список\n");

        /* -- РАБОТА С КОМАНДАМИ -- */
        int command;
        while(1) {
            away:
            printf("\nВведите команду: ");
            scanf("%d", &command);

            /* -- ДОБАВЛЕНИЕ НОВОЙ КНИГИ -- */
            if(command == 1) {
				//проверка ISBN
                printf("\nВведите ISBN: ");
                unsigned long long ISBN;
                scanf("%lld", &ISBN);
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        printf("Такая книга уже есть!\n");
                        goto away;
                    }
                }
				//считывание автора
                char c = getchar();
                printf("Введите автора: ");
                int j = 0;
                char author[64];
                while((c = getchar()) != '\n') {
                    author[j] = c;
                    j++;
                }
                author[j] = '\0';
				//считывание названия книги
                printf("Введите название книги: ");
                j = 0;
                char title[64];
                while((c = getchar()) != '\n') {
                    title[j] = c;
                    j++;
                }
                title[j] = '\0';
				//количество книг
                printf("Сколько книг добавить? ");
                int numBooks;
                scanf("%d", &numBooks);

				//выделение доп памяти для структуры
                booksPtr = (sBooks *)realloc(booksPtr, (numberLines+1)*sizeof(sBooks));
				/* Сохраняем всё в структуру */
                booksPtr[numberLines].isbn = ISBN;
                    //printf("%llu\n", booksPtr[numberLines].isbn);
                strcpy(booksPtr[numberLines].bookAuthor, author);
                    //printf("%s\n", booksPtr[numberLines].bookAuthor);
                strcpy(booksPtr[numberLines].bookTitle, title);
                    //printf("%s\n", booksPtr[numberLines].bookTitle);
                booksPtr[numberLines].allBooks = numBooks;
                    //printf("%d\n", booksPtr[numberLines].allBooks);
                booksPtr[numberLines].usedBooks = numBooks;
                //booksPtr = (sBooks *)realloc(booksPtr, (numberLines+1)*sizeof(sBooks));
                numberLines++;

                for(int j = numberLines-1; j < numberLines; j++) {
                    printf("\n>>> %llu;%s;%s;%d;%d", booksPtr[j].isbn, booksPtr[j].bookAuthor, booksPtr[j].bookTitle, booksPtr[j].allBooks, booksPtr[j].usedBooks);
                }
				
				/* Сохранение всех данных из структуры в файл */
                booksFile = fopen("books.csv", "w"); //по сути перезаписываем весь файл
                for(int i = 0; i < numberLines; i++) {
                    fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                    if(i != numberLines-1)
                        fprintf(booksFile, "\n");
                }
                fclose(booksFile);

                printf("\nКнига успешно добавлена!\n");
                command = 0;
                //goto away;
            }

            /* -- СОХРАНЕНИЕ ФАЙЛА -- */
            if(command == 12) {
                booksFile = fopen("books.csv", "w"); //по сути перезаписываем весь файл
                for(int i = 0; i < numberLines; i++) {
                    fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                    if(i != numberLines-1)
                        fprintf(booksFile, "\n");
                }
                fclose(booksFile);
				free(booksPtr); //очистка памяти
                printf("Файл успешно сохранён! Работа с программой завершена\n");
                command = 0;
                goto end;
            }

            /* -- УДАЛЕНИЕ КНИГИ -- */
            if(command == 2) {
                printf("\nВведите ISBN: ");
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
                    printf("\nКнига не найдена!\n");
                    goto away;
                }

                int k = position+1;
                //printf("%d %d", position+1, numberLines);
                printf("\nУдаление строки № %d \n", k);

                /* Сохраняем файл */
                if(booksPtr[position].isbn == 0) {
                    booksFile = fopen("books.csv", "w");
                    for(int i = 0; i < numberLines; i++) {
                        if(booksPtr[i].isbn == 0) {
                            //printf("\nКнига удалена!");
                            continue;
                        }
                        else {
                            fprintf(booksFile, "%llu;%s;%s;%d;%d", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                            if(i != numberLines-1) {
                                if(position == numberLines-1 && i == position-1) //если удаляем последнюю строку, то без \n
                                    printf(booksFile, "");
                                else
                                    fprintf(booksFile, "\n");
                            }
                        }
                    }
                    numberLines--;
                    printf("\nКнига удалена!\n");
                    fclose(booksFile);
                }

                /* Перезапись файла в структуру*/
                booksPtr = (sBooks *)malloc(1*sizeof(sBooks)); //память для первой структуры
                booksFile = fopen("books.csv", "r");
                for(int i = 0; i < numberLines; i++) {
                    if((fgets(tmp, 128, booksFile)) != NULL) {
                        booksPtr[i].isbn = atoll(strtok(tmp, ";")); //преобразование строки в длинное целое число с разделением ;
                        strcpy(booksPtr[i].bookAuthor, strtok(NULL, ";")); //копирование строки из символов в атрибут структуры
                        strcpy(booksPtr[i].bookTitle, strtok(NULL, ";"));
                        booksPtr[i].allBooks = atoi(strtok(NULL, ";"));
                        booksPtr[i].usedBooks = atoi(strtok(NULL, "\n"));
                        booksPtr = (sBooks *)realloc(booksPtr, (i+2)*sizeof(sBooks)); //увеличиваем размер блока памяти
                    }
                }
                fclose(booksFile);
                command = 0;
                //printf("%d", numberLines);
                //goto away;
            }

            /* -- ВЕСЬ СПИСОК -- */
            if(command == 13) {
                printf("\nКоличество строк в документе %d\n", numberLines);
                for(int i = 0; i < numberLines; i++) {
                    printf(">>> %llu;%s;%s;%d;%d\n", booksPtr[i].isbn, booksPtr[i].bookAuthor, booksPtr[i].bookTitle, booksPtr[i].allBooks, booksPtr[i].usedBooks);
                }
                command = 0;
            }

            /* -- ВЫДАТЬ КНИГУ ПО НОМЕРУ ISBN -- */
            if(command == 7) {
                printf("\nВведите ISBN: ");
                unsigned long long ISBN;
                scanf("%llu", &ISBN);
                int position = -1;
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        position = i;
                    }
                }
                if(position == -1) {
                    printf("\nКнига не найдена!\n");
                    goto away;
                }
                if(booksPtr[position].usedBooks == 0)
                    printf("\nДоступных книг нет!\n");
                else {
                    booksPtr[position].usedBooks--;
                    printf("\nКнига выдана!\n");
                }
                command = 0;
            }

            /* -- ПРИНЯТЬ КНИГУ ПО ISBN -- */
            if(command == 8) {
                printf("\nВведите ISBN: ");
                unsigned long long ISBN;
                scanf("%llu", &ISBN);
                int position = -1;
                for(int i = 0; i < numberLines; i++) {
                    if(booksPtr[i].isbn == ISBN) {
                        position = i;
                    }
                }
                if(position == -1) {
                    printf("\nКнига не найдена!\n");
                    goto away;
                }
                if(booksPtr[position].usedBooks == booksPtr[position].allBooks)
                    printf("\nКоличество доступных книг равно количеству книг в библиотеке\n");
                else {
                    booksPtr[position].allBooks++;
                    printf("\nКнига принята!\n");
                }
                command = 0;
            }
        }
    }
    end:
    return 0;
}
