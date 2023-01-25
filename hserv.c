#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Множество состояний fsm хоста-сервера
enum fsm_states {
    fsm_start_prog, //Запуск программы
    fsm_wait_client, //Ожидание подключения клиентов
    fsm_start_game, //Начало игры
    fsm_wait_word_hint, //Ожидание ввода слова и подсказки от хоста-сервера 
    fsm_select_client_move, //Выбор хода следующего игрока
    fsm_wait_client_move_result, //Ожидание результатов хода 
    fsm_handle_client_inf, //Обработка информации от клиента
    fsm_guess_letter, //Угадана буква
    fsm_guess_word, //Угадано слово 
    fsm_guess_nothing, //Ничего не угадано
    fsm_client_win, //Победа игрока
    fsm_wait_new_game, //Ожидание начала новой игры
    fsm_disconnect_other_player_move, //Отключение клиента в чужой ход 
    fsm_end_prog, //Завершение программы
    fsm_error //Ошибка выполнения программы
};

//Описание шагов fsm  
void server_fms_step(struct session_serv *serv) {
    switch (serv->state) {
        case fsm_start_prog:
            server_init(serv);
            server_go(serv);
            serv->state = fsm_wait_client;
            break;
        case fsm_wait_client:
            server_check_start_game(serv);
            serv->state = fsm_start_game;
            break;
        case fsm_start_game:
            server_start_game(serv);
            serv->state = fsm_wait_word_hint;
            break;
        case fsm_wait_word_hint:
            server_get_word();  
            server_get_hint();
            serv->state = fsm_select_client_move;
            break;
        case fsm_select_client_move:
            server_select_next_move(serv);
            server_send_notif(serv);
            serv->state = fsm_wait_client_move_result;
            break;
        case fsm_wait_client_move_result:
            server_check_valid_inf(serv);
            serv->state = fsm_handle_client_inf;
            break;
        case fsm_handle_client_inf:
            server_handle_client_inf(serv);
            break;
        case fsm_guess_word:
            server_send_msg(serv);
            serv->state = fsm_client_win;
            break;
        case fsm_guess_letter:
            serv_check_for_word(serv);
            break;
        case fsm_guess_nothing:
            server_send_msg_res(serv);
            serv->state = fsm_select_client_move;
            break;
        case fsm_client_win:
            server_send_win_req_msg(serv);
            serv->state = fsm_wait_new_game;
            break;
        case fsm_wait_new_game:
            server_check_valid_inf(serv);
            server_get_ngame_reply(serv);
            //server_handle_ngame_reply(serv);
            break;
        case fsm_disconnect_other_player_move:
             
            break;
        case fsm_end_prog:
            break;
        case fsm_error:
            printf("Error!\n");
            exit(-1);
    }
}

//Инициализация сервера
int server_init(struct session_serv *serv) {
     
}

//Запуск сервера
int server_go(struct session_serv *serv) {
    
}

/*
 * Проверка наличия игроков и потверждение хостом
 * начала игры 
 */
int server_check_start_game(struct session_serv *serv) {
        
}

//Запуск игры
int server_start_game(struct session_serv *serv) {

}

//Ввод слова 
int server_get_word(struct session_serv *serv) {

}

//Ввод подсказки
int server_get_hint(struct session_serv *serv) {
    
}

//Выбор хода следующего игрока
int server_select_next_move(struct session_serv *serv) {

}

//Отправка клиенту уведомления о начала его хода
int server_send_notif(struct session_serv *serv) {

}

/*
 * Проверка информации на коррекстность
 *(проверка данных на соответствие клиента 
 * и клиента текущего хода)
 */
int server_check_valid_inf(struct session_serv *serv) {
    
}

/*
 * Обработка пришедшей информации от клиента,
 * у которого сейчас ход 
 */
int server_handle_client_inf(struct session_serv *serv) {
    
}

//Отправка сообщения о результатах хода
int server_send_msg_res(struct session_serv *serv) {

}

//Проверка на получение последней буквы в слове 
int server_check_for_word(struct session_serv *serv) {
       
}

/*
 * Отправка сообщения о победе игрока 
 * и предложение начать новую игру
 */
int server_send_win_req_msg(struct session_serv *serv) {
        
}

/*
 * Ввод ответа от клиента о начале новой игры
 * и обработка ответа
 */
int server_get_ngame_reply(struct session_serv *serv) {

}

//Обработка ответа от клиента о начале новой игры
и обработка ответа
int server_handle_ngame_reply(struct session_serv *serv) {

}

int server_handle_client_disconnect(struct session_serv *serv) {
    
}

//MAIN
int main(int argc, char *argv[]) {

    struct session_serv sess_serv;
    int port;

    sess_serv.state = fsm_start_prog;
    
    server_fms_step(&sess_serv);
    
    return 0;
}
