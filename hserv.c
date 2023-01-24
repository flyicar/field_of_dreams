#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum fsm_states {
    fsm_prog_start, //Запуск программы
    fsm_wait_client, //Ожидание подключения клиентов
    fsm_start_game, //Начало игры
    fsm_wait_word_hint, //Ожидание ввода слова и подсказки от хоста-сервера 
    fsm_choice_client_move, //Выбор хода следующего игрока
    fsm_wait_client_move_result, //Ожидание результатов хода 
    fsm_handle_client_inf, //Обработка информации от клиента
    fsm_guess_letter, //Угадана буква
    fsm_guess_word, //Угадано слово 
    fsm_guess_nothing, //Ничего не угадано
    fsm_client_win, //Победа игрока
    fsm_wait_new_game, //Ожидание начала новой игры
    fsm_disconnect_other_player_move, //Отключение клиента в чужой ход 
    fsm_prog_end, //Завершение программы
    fsm_error //Ошибка выполнения программы
};

struct client_inf {
    /*
     * Инфомация о конкретном игроке
     */
};

struct session_serv {
    enum fsm_states state;
    /*
     * Данные сервера, состояние fsm,
       массив структур клиентов, и т.д.
     */
};

void server_init(struct session _serv *serv) {
     
}

//Функция описывающая состояния 
void make_fms_step(struct session_serv *serv) {
    switch (serv->state) {
        case fsm_prog_start:
            printf("Running server...\n");

            printf("Ready\n")
            break;
        case fsm_wait_client:
        case fsm_start_game:
        case fsm_wait_word_hint:
        case fsm_choice_client_move:
        case fsm_wait_client_move_result:
        case fsm_handle_client_inf:
        case fsm_guess_letter:
        case fsm_guess_word:
        case fsm_guess_nothing:
        case fsm_client_win:
        case fsm_wait_new_game:
        case fsm_disconnect_other_player_move:
        case fsm_prog_end:
        case fsm_error:
            printf("Error!\n");
            exit(-1);
    }
}

int main(int argc, char *argv[]) {

    struct session_serv sess_serv;
    int port;


    
    make_fms_step(&sess_serv);
    
    return 0;
}
