#ifndef SERV_H
#define SERV_H

#include "serv_structs.h"

//Проверка на запуск игры
int server_check_start_game(struct sess_serv *serv);

void server_write_to_stream(struct sess_serv *serv, int sd);

void server_read_from_stream(struct sess_serv *serv, int sd);

void server_broadcast(struct sess_serv *serv, int with_host);

void server_handle_cmd(struct sess_serv *serv, int sd);

void server_accept_client(struct sess_serv *serv);

void server_init(struct sess_serv *serv, int serv_port);

void server_go(struct sess_serv *serv);

void server_form_nick_str(struct sess_serv *serv, char str[MAX_CLIENT_COUNT * MAX_NICK_SIZE + MAX_CLIENT_COUNT]);

void server_put_in_queue(struct sess_serv *serv, int sd);

#endif /*SERV_H*/
