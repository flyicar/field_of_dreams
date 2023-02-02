#ifndef PROT_H
#define PROT_H

#include "serv_structs.h"

void prot_string_pars(struct prot_struct *msg, char buffer[MAX_BUFFER_SIZE]);
    
void prot_make_command(struct prot_struct *msg, char buffer[MAX_BUFFER_SIZE]);

void prot_fill_prot_msg(struct prot_struct *msg, char *cmd, 
char *dev_type, char *data);

enum commands prot_get_packet_type(char* str);

#endif /*PROT_H*/
