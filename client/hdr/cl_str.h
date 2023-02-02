#ifndef CL_STR_H
#define CL_STR_H

#define NICKNAME_SIZE 16
#define MAX_LETTER_SIZE 32

//Данные клиента. Хранится слово (а точнее его угаданные составляющие) и никнейм.
struct client_info {
    int sockfd;
    char nickname[NICKNAME_SIZE];
    bool letter_arr[MAX_LETTER_SIZE];
};

#endif
