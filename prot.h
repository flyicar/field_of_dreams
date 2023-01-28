#ifndef PROT_H
#define PROT_H

#include "serv_structs.h"

/*
 * Протокол: преобразование информации от клиента 
 * дешифровка команды (возможны опции и параметры), 
 * заполнение структуры сервера и клиента
 */
void prot_get_from_cmd(struct sess_serv *serv, int sd, int cmd_size);  

/*
 * Протокол: формирование ответа клиенту 
 * взятие данных из структур сервера и клиента,
 * создание команды (возможны опции и параметры) 
 */
char *prot_put_into_cmd(struct sess_serv *serv, int sd, int cmd_size);  

#endif /*PROT_H*/
