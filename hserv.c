#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef MAX_NICK_SIZE
#define MAX_NICK_SIZE 32
#endif

#ifndef REQ_QLEN
#define REQ_QLEN 32
#endif

#ifndef INIT_CLIENTS_ARR_SIZE
#define INIT_SESS_ARR_SIZE 32
#endif

#ifndef MAX_CLIENT_COUNT
#define MAX_CLIENT_COUNT 10
#endif

#ifndef MIN_CLIENT_COUNT
#define MIN_CLIENT_COUNT 2
#endif

#ifndef MAX_BUFFER_SIZE 512

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

struct sess_client {
    int sd; //Сокет для связи с клиентом
    char nick[MAX_NICK_SIZE]; //Никнэйм игрока
    enum fsm_states state; //Состояние fsm каждого клиента
};

struct sess_serv {
    int ls, //Прослушивающий сокет
    struct sess_client **clients; //Динамический массив структур данных о клиентах
    int clients_arr_size; //Длина динамического массива
};

//Описание шагов fsm  
void server_fms_step(struct session_serv *serv) {
    switch (serv->state) {
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
            server_handle_ngame_reply(serv);
            break;
        case fsm_disconnect_other_player_move:
            break;
        case fsm_end_prog:
            break;
    }
}

int server_read_from_client(struct sess_client *client)
{
    size_t size;
    from_client[MAX_BUFFER_SIZE];

    size = read(sess->sd, from_client, sizeof(MAX_BUFFER_SIZE));
    if(size <= 0) {
        perror("read");
        exit(1);
    }

    if (sess->state == fsm_finish)
        return 0;
    return 1;
}

struct sess_client *make_new_sess_client(int fd) {

    struct sess_client *client = malloc(sizeof(*sess));

    client->sd = sd;
    client->nick = NULL;
    client->state = fsm_start_prog;

    return client;
}

void server_accept_client(struct server_str *serv)
{
    int sd, i;
    struct sockaddr_in cliaddr;
    socklen_t addr_len;
    int newlen;

    addr_len = sizeof(cliaddr);
    sd = accept(serv->ls, (struct sockaddr*) &cliaddr, &addr_len);
    if (sd < 0) {
        perror("accept");
        exit(1);
    }

    /*
     * Проверка сокета на значение, если ОС выдала 
     * значение меньше, чем размер массива структур клиента,
     * то увеличиваем размер массива, до нужного значения 
     */
    if (sd >= serv->clients_arr_size) {
        newlen = serv->clients_arr_size;
        while (sd >= newlen)
            newlen += INIT_CLIENTS_ARR_SIZE;

        serv->clients = realloc(serv->clients, 
        newlen * sizeof(struct sess_client *));

        for (i = serv->clients_arr_size; i < newlen; i++)
            serv->clients[i] = NULL;
        serv->clients_arr_size = newlen;
    }

    serv->clients[sd] = make_new_sess_client(sd);
}
//Инициализация сервера
void server_init(struct sess_serv *serv, int serv_port) {

    struct sockaddr_in servaddr;
    
    serv->ls = socket(AF_INNET, SOCK_STREAM, IPPROTO_TCP);
    if (serv->ls < 0) {
        perror("socket");
        exit(1);
    }
    
    servaddr.sin_family = AF_INNET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serv_port);

    if (bind(serv->ls, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0) {
        perror("bind");
        close(serv->ls);
        exit(1);
    }

    if (listen(serv->ls, REQ_QLEN) < 0) {
        perror("listen");
        close(serv->ls);
        exit(1);
    }

    serv->clients = malloc(INIT_CLIENTS_ARR_SIZE * sizeof(struct sess_client *));
    serv->clients_arr_size = INIT_CLIENTS_ARR_SIZE;
    for (i = 0; i < INIT_CLIENTS_ARR_SIZE; i++)
        serv->clients[i] = NULL;
}

//Запуск сервера(основной цикл событийной обработки)
void server_go(struct sess_serv *serv) {

    fd_set readfds;
    int maxfd;
    int end_client_sess;
    int client_count;

    for(;;) { 
        
        FD_ZERO(&readfds);

        
        FD_SET(serv->ls, &readfds);
        maxfd = serv->ls;

        for (i = 0; i < serv->clients_arr_size; i++) {
            //Проверка на NULL(NULL если клиент ещене подключился)
            if(serv->clients[i]) {
                FD_SET(i, &readfds);
                if(i > maxfd)
                    maxfd = i;
            }
        }

        if (select(maxfd+1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }
        
        /*
         * Событие: запрос нового клиента на соединение, если клиентов большеем
         */
        if (FD_ISSET(serv->ls, &readfds) && (client_count < MAX_CLIENT_COUNT))
            server_accept_client(serv);

        /*
         * События: получение информации от пользователя(любой)
         */
        for(i = 0; i < serv->clients_arr_size; i++) {
            if(serv->clients[i] && FD_ISSET(i, &readfds)) {
                 server_read_from_client(serv->clients[i]);
                if(!end_client_sess)
                    server_close_sess_client(serv, i);
            }
        }
    }
     
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

    struct sess_serv server;
    int port;

    if (argc != 2) {
        printf("Usage: hserv <port>\n");
        exit(1);
    }

    server_init(&server, port);
    server_go(server);
    
    return 0;
}
