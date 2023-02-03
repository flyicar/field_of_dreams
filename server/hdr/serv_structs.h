#ifndef SERV_STRUCTS_H
#define SERV_STRUCTS_H

#include "consts.h"
#include "enums.h"

struct game_conf {
    int move_time;
    //Что-то еще)
};

struct prot_struct {
    char command[COMMAND_SIZE]; //Команда протокола (например, HELLO или START)
    char device_type[DEVICE_NAME]; //тип девайса: SV или CL
    char data[MAX_DATA_SIZE]; //"полезные" данные
};

struct sess_client {
    int sd; //Сокет для связи с клиентом
    char nick[MAX_NICK_SIZE]; //Никнэйм игрока
    char word[MAX_WORD_SIZE]; //Введеное клиентом слово
    char letter; //Введенная клиентом буква  
    enum fsm_states state; //Состояние fsm каждого клиента
};

struct sess_serv {
    int ls; //Прослушивающий сокет
    int clients_arr_size; //Длина динамического массива
    int host_start_game; //Флаг готовности хоста начать игру 
    char word[MAX_WORD_SIZE]; //Загаданное хостом слово
    char hint[MAX_HINT_SIZE]; //Данная хостом подсказка
	int client_count; //Текущее число клиентов подключенных к серверу
	int player_count; //Число играющих игроков
	int players_queue[MAX_CLIENT_COUNT];
    int pos_arr_serv[MAX_WORD_SIZE]; //Булевый массив, содержащий угаданные 
    //позиции от всех игроков
	int fd[2]; //Дескрипторы пайпа для связи с хостом
    struct game_conf *conf; //Указатель на структуру конфигурации игры
	struct prot_struct *msg; //Указатель на структуру сообщения протокола
    struct sess_client **clients; //Динамический массив структур данных о клиентах
};

#endif /*SERV_STRUCTS*/
