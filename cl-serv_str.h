#define NICKNAME_SIZE 32
#define MAX_LETTER_SIZE 64 

#include <stdbool.h>

struct client_inf {
    int sockfd,
    char nickname[NICKNAME_SIZE],
    bool letter_arr[MAX_LETTER_SIZE],
}

struct session_serv {
    int lsockfd,
    struct client_inf *client_inf_arr,
    int client_arr_size,
    enum fsm_states state
}
