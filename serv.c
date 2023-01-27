#include "serv.h"

void server_check_start_game(struct sess_serv *serv, int sd) {

    serv->clients[sd]->state = fsm_start_game;
}

//Запуск игры
void server_start_game(struct sess_serv *serv, int sd) {

}

//Ввод слова от хоста
void server_get_host_word(struct sess_serv *serv) {

}

//Ввод подсказки от хоста
void server_get_host_hint(struct sess_serv *serv) {
    
}

//Выбор хода следующего игрока
void server_select_next_move(struct sess_serv *serv, int sd) {

}

//Отправка клиенту уведомления о начала его хода
void server_send_notif(struct sess_serv *serv, int sd) {

}

/*
 * Проверка информации на коррекстность
 *(проверка данных на соответствие клиента 
 * и клиента текущего хода)
 */
void server_check_valid_inf(struct sess_serv *serv, int sd) {
    
}

/*
 * Обработка пришедшей информации от клиента,
 * у которого сейчас ход 
 */
void server_handle_client_inf(struct sess_serv *serv, int sd) {
    
}

//Отправка сообщения о результатах хода
void server_send_msg_res(struct sess_serv *serv, int sd) {

}

//Проверка на получение последней буквы в слове 
void server_check_for_word(struct sess_serv *serv, int sd) {
       
}

/*
 * Отправка сообщения о победе игрока 
 * и предложение начать новую игру
 */
void server_send_win_req_msg(struct sess_serv *serv, int sd) {
        
}

/*
 * Ввод ответа от клиента о начале новой игры
 * и обработка ответа
 */
void server_get_ngame_reply(struct sess_serv *serv, int sd) {

}

/*
 * Обработка ответа от клиента о начале новой 
 * игры и обработка ответа
 */
void server_handle_ngame_reply(struct sess_serv *serv, int sd) {

}

//Обработка отключения клиента
void server_handle_client_disconnect(struct sess_serv *serv, int sd) {
    
}
