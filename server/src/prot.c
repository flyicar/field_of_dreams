#include "serv_structs.h"
#include "enums.h"
#include <string.h>

void prot_string_pars(struct prot_struct *msg, char buffer[MAX_BUFFER_SIZE]) {
 	
	char *istr;

    // Выделение последующих частей
    istr = strtok(buffer, "/");
	strcpy(msg->command, istr);

    istr = strtok(NULL, "/");
	strcpy(msg->device_type, istr);

	if (!istr) {
		istr = strtok(NULL, "/");
		strcpy(msg->data, istr);
	}
}

void prot_make_command(struct prot_struct *msg, char buffer[MAX_BUFFER_SIZE]) {
	
	memset(buffer, 0, MAX_BUFFER_SIZE);
	strcat(buffer, msg->command);
	strcat(buffer, "/");
	strcat(buffer, msg->device_type);
	strcat(buffer, "/");
	strcat(buffer, msg->data);
}

void prot_fill_prot_msg(struct prot_struct *msg, char *cmd, 
char *dev_type, char *data) {
	
	strcpy(msg->command, cmd);
	strcpy(msg->device_type, dev_type);

	if (data) 
		strcpy(msg->data, data);
}

enum commands prot_get_packet_type(char* str) {

    if (strcmp(str, "DENIED") == 0) return DENIED;
    if (strcmp(str, "HELLO") == 0) return HELLO;
    if (strcmp(str, "START") == 0) return START;
    if (strcmp(str, "USERS") == 0) return USERS; 
    if (strcmp(str, "WORD") == 0) return WORD;
    if (strcmp(str, "DATA") == 0) return DATA;
    if (strcmp(str, "MOVE") == 0) return MOVE;
    if (strcmp(str, "SUCCESS") == 0) return SUCCESS;
    if (strcmp(str, "INCORRECT") == 0) return INCORRECT;
    if (strcmp(str, "TIMEOUT") == 0) return TIMEOUT;
    if (strcmp(str, "ENDGAME") == 0) return ENDGAME;
    if (strcmp(str, "ERROR") == 0) return ERROR;
}
