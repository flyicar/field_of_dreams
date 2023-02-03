#include <stdio.h>
#include <string.h>

#include "serv_structs.h"
#include "serv.h"
#include "enums.h"
#include "prot.h"

void server_handle_cmd(struct sess_serv *serv, int sd) {
	
	char *str, nick_str[MAX_CLIENT_COUNT * MAX_NICK_SIZE + MAX_CLIENT_COUNT];
	int with_host;
	int i;

	switch (prot_get_packet_type(serv->msg->command)) {

		case HELLO: 
			if (strcmp(serv->msg->device_type, "CL") == 0) {
				if (!serv->host_start_game) {

					serv->clients[sd]->state = fsm_wait_start_game;

					server_put_in_queue(serv, sd);
					serv->player_count++;
					strcpy(serv->clients[sd]->nick, serv->msg->data);
					prot_fill_prot_msg(serv->msg, "HELLO", "SV", NULL);
					server_write_to_stream(serv, sd);
				}
				else {

					serv->clients[sd]->state = fsm_wait_new_game;

					prot_fill_prot_msg(serv->msg, "DENIED", "SV", NULL);
					server_write_to_stream(serv, sd);
				}
			} 
			break;

		case START:
			if (strcmp(serv->msg->device_type, "HS") == 0) {
				if (server_check_start_game(serv)) {

					//Переводим всех игроков в состояния начала игры
					for (i = 0; i < serv->player_count; i++)
						serv->clients[serv->players_queue[i]]->state 
						= fsm_start_game;
					
					//Отправка никнэймов с уведомлением о начале игры 
					server_form_nick_str(serv, nick_str);
					prot_fill_prot_msg(serv->msg, "START", "SV", nick_str);
					with_host = 1;
					server_broadcast(serv, with_host);	
				}
				else {
					//Недостаточное число игроков, отказ хосту
					prot_fill_prot_msg(serv->msg, "DENIED", "SV", NULL);
					server_write_to_stream(serv, sd);
				}
			} 
			break;

		case HINT:
			if (strcmp(serv->msg->device_type, "HS") == 0) {
				strcpy(serv->hint, serv->msg->data);
				prot_fill_prot_msg(serv->msg, "HINT", "SV", serv->hint);
				with_host = 0;
				server_broadcast(serv, with_host);
			}
			break;

		case WORD:
			if (strcmp(serv->msg->device_type, "HS") == 0) {
				strcpy(serv->word, serv->msg->data);
				sprintf(str, "%ld", strlen(serv->word));
				prot_fill_prot_msg(serv->msg, "WORD", "SV", str);  
				with_host = 0;
				server_broadcast(serv, with_host);
			}
			break;

		default: 
			prot_fill_prot_msg(serv->msg, "ERROR", "SV", NULL);
			server_write_to_stream(serv, sd);
	}
}
