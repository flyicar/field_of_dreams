#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "serv_structs.h"
#include "serv.h"
#include "prot.h"

//sd - дескриптор обрабатываемого клиента
void server_read_from_stream(struct sess_serv *serv, int sd) {

    size_t size;
    char from_stream[MAX_BUFFER_SIZE];

    size = read(sd, from_stream, sizeof(from_stream));
    if (size < 0) {
        perror("read");
        exit(1);
    }
	//Если не конец файла, и read не вернул ошибку
	else if (size > 0) {
		if (from_stream[strlen(from_stream) - 1] == '\n')
			from_stream[strlen(from_stream) - 1] = '\0';
		prot_string_pars(serv->msg, from_stream);
		server_handle_cmd(serv, sd);
	}
}

//Рассылка сообщения всем клиентам, в том числе хосту
void server_broadcast(struct sess_serv *serv, int host) {
	
	int i;
	
	for (i = 0; i < serv->clients_arr_size; i++)
		if (serv->clients[i])
			server_write_to_stream(serv, i);	
	if (host)
		server_write_to_stream(serv, serv->fd[1]);
}

//sd - дескриптор обрабатываемого клиента
void server_write_to_stream(struct sess_serv *serv, int sd) {
    
    size_t size;
    char to_stream[MAX_BUFFER_SIZE];

	prot_make_command(serv->msg, to_stream);

    size = write(sd, to_stream, 
	sizeof(to_stream));
    if (size != sizeof(to_stream)) {
        perror("write");
        exit(1);
    }
}
