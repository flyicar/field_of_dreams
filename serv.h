#ifndef MAX_NICK_SIZE
#define MAX_NICK_SIZE 16
#endif

#ifndef MAX_WORD_SIZE
#define MAX_WORD_SIZE 16
#endif

#ifndef MAX_HINT_SIZE
#define MAX_HINT_SIZE 64
#endif

#ifndef REQ_QLEN
#define REQ_QLEN 10
#endif

#ifndef INIT_CLIENTS_ARR_SIZE
#define INIT_CLIENTS_ARR_SIZE 32
#endif

#ifndef MAX_CLIENT_COUNT
#define MAX_CLIENT_COUNT 10
#endif

#ifndef MIN_CLIENT_COUNT
#define MIN_CLIENT_COUNT 2
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 512
#endif

#ifndef MAX_CMD_SIZE 
#define MAX_CMD_SIZE 32
#endif

//Множество состояний fsm хоста-сервера
enum fsm_states {
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

//Проверка на запуск игры
void server_check_start_game(struct sess_serv *serv, int sd);

//Запуск игры
void server_start_game(struct sess_serv *serv, int sd);

//Ввод слова от хоста
void server_get_host_word(struct sess_serv *serv);

//Ввод подсказки от хоста
void server_get_host_hint(struct sess_serv *serv);

//Выбор хода следующего игрока
void server_select_next_move(struct sess_serv *serv, int sd);

//Отправка клиенту уведомления о начала его хода
void server_send_notif(struct sess_serv *serv, int sd);

/*
 * Проверка информации на коррекстность
 *(проверка данных на соответствие клиента 
 * и клиента текущего хода)
 */
void server_check_valid_inf(struct sess_serv *serv, int sd);

/*
 * Обработка пришедшей информации от клиента,
 * у которого сейчас ход 
 */
void server_handle_client_inf(struct sess_serv *serv, int sd);

//Отправка сообщения о результатах хода
void server_send_msg_res(struct sess_serv *serv, int sd);

//Проверка на получение последней буквы в слове 
void server_check_for_word(struct sess_serv *serv, int sd);

/*
 * Отправка сообщения о победе игрока 
 * и предложение начать новую игру
 */
void server_send_win_req_msg(struct sess_serv *serv, int sd);

/*
 * Ввод ответа от клиента о начале новой игры
 * и обработка ответа
 */
void server_get_ngame_reply(struct sess_serv *serv, int sd);

/*
 * Обработка ответа от клиента о начале новой 
 * игры и обработка ответа
 */
void server_handle_ngame_reply(struct sess_serv *serv, int sd);

//Обработка отключения игрока
void server_handle_client_disconnect(struct sess_serv *serv, int sd);
