#ifndef SERV_STRUCTS_H
#define SERV_STRUCTS_H

#include "consts.h"
#include "states.h"

struct game_conf {
    int move_time;
    //Что-то еще)
};

struct sess_client {
    int sd; //Сокет для связи с клиентом
    char cmd[MAX_CMD_SIZE]; //Команда полученная от игрока
    char nick[MAX_NICK_SIZE]; //Никнэйм игрока
    char word[MAX_WORD_SIZE]; //Введеное клиентом слово
    char letter; //Введенная клиентом буква  
    enum fsm_states state; //Состояние fsm каждого клиента
};

struct sess_serv {
    int ls; //Прослушивающий сокет
    struct sess_client **clients; //Динамический массив структур данных о клиентах
    int clients_arr_size; //Длина динамического массива
    int host_start_game; //Флаг начала игры
    char word[MAX_WORD_SIZE]; //Загаданное хостом слово
    char hint[MAX_HINT_SIZE]; //Данная хостом подсказка
    int pos_arr_serv[MAX_WORD_SIZE]; //Булевый массив, содержащий угаданные 
    //позиции от всех игроков
    struct game_conf *conf; //Указатель на структуру конфигурации игры
};

#endif /*SERV_STRUCTS*/
