#include <stdio.h>
#include <string.h>

#include "serv_structs.h"
#include "serv.h"
#include "enums.h"
#include "prot.h"

void server_handle_cmd(struct sess_serv *serv, int sd) {
	
	char *str;
	int with_host;

	with_host = 0;
	switch (prot_get_packet_type(serv->msg->command)) {

		case HELLO: 
			if (strcmp(serv->msg->device_type, "CL") == 0) {
				if (!serv->host_start_game) {
					strcpy(serv->clients[sd]->nick, serv->msg->data);
					prot_fill_prot_msg(serv->msg, "HELLO", "SV", NULL);
					server_write_to_stream(serv, sd);
					//server_fsm_step(serv, sd);
				}
				else {
					prot_fill_prot_msg(serv->msg, "DENIED", "SV", NULL);
					server_write_to_stream(serv, sd);
				}
			} 
			break;

		case START:
			if (strcmp(serv->msg->device_type, "HS") == 0) {
				if (server_check_start_game(serv));
				prot_fill_prot_msg(serv->msg, "START", "SV", "Здесь должны быть ники");
				with_host = 1;
				server_broadcast(serv, with_host);
			} 
			if (strcmp(serv->msg->device_type, "CL") == 0) {
			}
			break;

		case HINT:
			if (strcmp(serv->msg->device_type, "HS") == 0) {
				strcpy(serv->hint, serv->msg->data);
				prot_fill_prot_msg(serv->msg, "HINT", "SV", NULL);
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
