#include "serv_structs.h"
#include "enums.h"
#include <stdio.h>

int server_check_start_game(struct sess_serv *serv) {
	if (serv->client_count >= MIN_CLIENT_COUNT) {
		serv->host_start_game = 1;
		return 1;
	}
	return 0;
}
