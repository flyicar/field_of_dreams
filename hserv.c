#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "serv_structs.h"
#include "serv.h"
#include "prot.h"
#include "consts.h"
#include "states.h"

//Описание шагов fsm  
void server_fsm_step(struct sess_serv *serv, int sd) {
    switch (serv->clients[sd]->state) {
        case fsm_wait_client:
            server_check_start_game(serv, sd);
            break;
        case fsm_start_game:
            server_start_game(serv, sd);
            break;
        case fsm_wait_word_hint:
            server_get_host_word(serv);  
            server_get_host_hint(serv);
            break;
        case fsm_select_client_move:
            server_select_next_move(serv, sd);
            server_send_notif(serv, sd);
            break;
        case fsm_wait_client_move_result:
            server_check_valid_inf(serv, sd);
            break;
        case fsm_handle_client_inf:
            server_handle_client_inf(serv, sd);
            break;
        case fsm_guess_word:
            server_send_msg_res(serv, sd);
            break;
        case fsm_guess_letter:
            server_check_for_word(serv, sd);
            break;
        case fsm_guess_nothing:
            server_send_msg_res(serv, sd);
            break;
        case fsm_client_win:
            server_send_win_req_msg(serv, sd);
            break;
        case fsm_wait_new_game:
            server_check_valid_inf(serv, sd);
            server_get_ngame_reply(serv, sd);
            server_handle_ngame_reply(serv, sd);
            break;
        case fsm_disconnect_other_player_move:
            server_handle_client_disconnect(serv, sd);
            break;
        case fsm_end_prog:
            break;
    }
}

//sd - дескриптор обрабатываемого клиента
void server_read_from_client(struct sess_serv *serv, int sd) {

    size_t size;
    char from_client[MAX_BUFFER_SIZE];

    size = read(serv->clients[sd]->sd, from_client, sizeof(from_client));
    if (size <= 0) {
        perror("read");
        exit(1);
    }
    
    /*
     * Протокол: преобразование информации от клиента 
     * дешифровка команды (возможны опции и параметры), 
     * заполнение структуры сервера и клиента
     */
    prot_get_from_cmd(serv, sd, size);
}

//sd - дескриптор обрабатываемого клиента
void server_write_to_client(struct sess_serv *serv, int sd) {
    
    size_t size;
    char to_client[MAX_BUFFER_SIZE];

    /*
     * Протокол: формирование ответа клиенту 
     * взятие данных из структур сервера и клиента
     * создание команды (возможны опции и параметры), 
     */
    strcpy(to_client, prot_put_into_cmd(serv, 
    sd, MAX_BUFFER_SIZE));

    size = write(serv->clients[sd]->sd, 
    to_client, sizeof(to_client));
    if (size != sizeof(to_client)) {
        perror("write");
        exit(1);
    }
}

struct sess_client *make_new_sess_client(int sd) {

    struct sess_client *client = malloc(sizeof(struct sess_client));

    client->sd = sd;
    memset(client->nick, 0, sizeof(client->nick));
    memset(client->word, 0, sizeof(client->word));
    client->letter = 0;
    client->state = fsm_wait_client;

    return client;
}

void server_accept_client(struct sess_serv *serv)
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
    int i;
    
    serv->ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv->ls < 0) {
        perror("socket");
        exit(1);
    }
    
    servaddr.sin_family = AF_INET;
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
    int i, client_count;

    client_count = 0;
    for(;;) { 
        
        FD_ZERO(&readfds);

        FD_SET(serv->ls, &readfds);
        maxfd = serv->ls;

        for (i = 0; i < serv->clients_arr_size; i++) {
            //Проверка на NULL(NULL, если клиент еще не подключился)
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
         * Событие: запрос нового клиента на соединение, если клиентов больше
         * чем максимальное возможное число, то запускаем отказ
         */
        if (FD_ISSET(serv->ls, &readfds) && (client_count < MAX_CLIENT_COUNT)) {
            client_count++;
            server_accept_client(serv);
        }
        else {
            //Отказ новому клиенту
        }

        /*
         * События: получение информации от пользователя(любой)
         */
        for (i = 0; i < serv->clients_arr_size; i++) {
            if (serv->clients[i] && FD_ISSET(i, &readfds)) {
                server_read_from_client(serv, i);
            }
        }
    }
     
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
    server_go(&server);
    
    return 0;
}
