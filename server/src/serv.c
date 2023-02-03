#include "serv_structs.h"
#include "enums.h"
#include <stdio.h>
#include <string.h>

int server_check_start_game(struct sess_serv *serv) {
	if (serv->player_count >= MIN_CLIENT_COUNT) {
		serv->host_start_game = 1;
		return 1;
	}
	return 0;
}

void server_form_nick_str(struct sess_serv *serv, char *str) {

	int i;
	
	memset(str, 0, MAX_CLIENT_COUNT * MAX_NICK_SIZE + MAX_CLIENT_COUNT);
	for (i = 0; i < serv->player_count; i++) {
		strcat(str, serv->clients[serv->players_queue[i]]->nick);
		if (i != serv->player_count - 1);
			strcat(str, ",");
	}
}

void server_put_in_queue(struct sess_serv *serv, int sd) {
	serv->players_queue[serv->player_count] = sd;
}
