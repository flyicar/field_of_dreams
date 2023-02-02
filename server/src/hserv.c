#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include "serv_structs.h"
#include "serv.h"
#include "consts.h"
#include "prot.h"
#include "enums.h"

/*//Описание шагов fsm  
void server_fsm_step(struct sess_serv *serv, int sd) {
    switch (serv->clients[sd]->state) {
		case fsm_start_prog:
			serv->clients[sd]->state = fsm_wait_client;
        case fsm_wait_client:
			serv->clients[sd]->state = fsm_start_game;
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
}*/

struct sess_client *server_make_new_sess_client(int sd) {

    struct sess_client *client = malloc(sizeof(struct sess_client));

    client->sd = sd;
    memset(client->nick, 0, sizeof(client->nick));
    memset(client->word, 0, sizeof(client->word));
    client->letter = 0;
    client->state = fsm_start_prog;

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

    serv->clients[sd] = server_make_new_sess_client(sd);
}

//Запуск сервера(основной цикл событийной обработки)
void server_go(struct sess_serv *serv) {

    fd_set readfds;
    int maxfd;
    int i;
	char command[512];
	size_t size;

	printf("Enter any command:\n");
	fgets(command, sizeof(command), stdin);

    size = write(serv->fd[1], command, 
	sizeof(command));
    if (size != sizeof(command)) {
        perror("write");
        exit(1);
    }

    for(;;) { 
        
        FD_ZERO(&readfds);
		
		//Записываем прослушивающий сокет в readfds
        FD_SET(serv->ls, &readfds);
        maxfd = serv->ls;

		//Записываем дескриптор пайпа readfds(для хоста)
		FD_SET(serv->fd[0], &readfds);

		if (serv->fd[0] > maxfd)
			maxfd = serv->fd[0];

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
        if (FD_ISSET(serv->ls, &readfds) && (serv->client_count < MAX_CLIENT_COUNT)) {
            serv->client_count++;
            server_accept_client(serv);
        }
        else {
            //Отказ новому клиенту
        }

		//Событие: получение информации от хоста 
		if (FD_ISSET(serv->fd[0], &readfds)) {
			server_read_from_stream(serv, serv->fd[0]);
		}

        /*
         * События: получение информации от пользователя(любой)
         */
        for (i = 0; i < serv->clients_arr_size; i++) {
            if (serv->clients[i] && FD_ISSET(i, &readfds)) {
                server_read_from_stream(serv, i);
            }
        }

		size = read(serv->fd[0], command, sizeof(command));
		if (size < 0) {
			perror("read");
			exit(1);
    	}
		printf("%s", command);
		fflush(stdout);
    }
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
	
	//Установка прослушивающего сокета
    if (listen(serv->ls, REQ_QLEN) < 0) {
        perror("listen");
        close(serv->ls);
        exit(1);
    }
	
	//Выделение памяти для указателей на структуры клиентов 
    serv->clients = malloc(INIT_CLIENTS_ARR_SIZE * sizeof(struct sess_client *));
    serv->clients_arr_size = INIT_CLIENTS_ARR_SIZE;
    for (i = 0; i < INIT_CLIENTS_ARR_SIZE; i++)
        serv->clients[i] = NULL;
	
	serv->msg = malloc(sizeof(struct prot_struct));
	
	//Инициализация pipe для хоста
	if (pipe(serv->fd) < 0) {
		perror("pipe");
		exit(1);
	}

	//Инициализация базовых переменных сервера
	serv->client_count = 0;
	serv->host_start_game = 0;

}

//MAIN
int main(int argc, char *argv[]) {

    struct sess_serv server;
    int port;

    if (argc != 2) {
        printf("Usage: hserv <port>\n");
        exit(1);
    }
	port = atoi(argv[1]);

    server_init(&server, port);
    server_go(&server);
    
    return 0;
}
