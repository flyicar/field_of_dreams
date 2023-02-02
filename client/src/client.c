#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "cl_str.h"
#include "state_machine.h"

//Структура информации об игроке.
struct client_info client_data;

//Пока дефолтная структура для сервера. Можно засунуть ее во что-то.
struct sockaddr_in server_address;

//Предполагаемые аргументы (Если не задаются явно, то должны браться значения из конфига).
//-n отвечает за никнейм, используемый клиентом
//-i/s отвечают за ip-адрес/порт сервера, к которому подключается клиент

int main(int argc, char* argv[]) {
  enum client_states fsm_step = 1; //Номер состояния FSM

  int opt, network_socket;
  char *opts = "n:i:p:h:";
  bool connection_success = false;
  //Надо обернуть в структуру

  //Обработка всех предполагаемых аргументов.
  //Вызов getopt до тех пор, пока не будут обработаны
  server_address.sin_family = AF_INET;

  while ((opt = getopt(argc, argv, opts)) != -1) {
    switch(opt) {
      case 'n': {
        strncpy(client_data.nickname, optarg, sizeof(client_data.nickname));
        /* конфиг здесь*/
        break;
      }
      case 'i': {
        //преобразовывает обычный вид IP-адреса (из номеров и точек)
        //в двоичный код и сохраняет его в структуре
        if (inet_aton(optarg, &server_address.sin_addr) != 1) {
          	server_address.sin_addr.s_addr = INADDR_ANY;
          printf("IP-адрес сервера некорректен. Используется config...\n");
          /* конфиг здесь*/
        }
        printf("IP: %s \n", optarg);
        break;
      }
      case 'p': {
        if (atoi(optarg) < 65536 && atoi(optarg) > 0) { //Проверка корректности введенного порта.
          server_address.sin_port = htons(atoi(optarg)); //htons для сетевого порядка
          printf("PORT: %s\n", optarg);
        }
        else {
          printf("Порт некорректен. Используется config...\n");
        }
        break;
      }
      case 'h': {
        printf("Аргументы: \n");
        printf("-n [string] - позволяет передавать никнейм как аргумент.\n");
        printf("-i [string] - позволяет передавать ip-адрес как аргумент.\n");
        printf("-p [short int] - позволяет передавать порт как аргумент.\n");
        printf("-h - вывод этой подсказки. \n");
        break;
      }
    }
  }


  //Создание сокета со стороны клиента для TCP-соединения.
  network_socket = socket(AF_INET, SOCK_STREAM, 0);

  for (;;) {
    //printf("%d \n", fsm_launch_waiting);
    switch (fsm_step) {
      case fsm_launch_waiting:
        //game_waiting_frame(...);

        for (int connect_try = 5; connect_try > 0; connect_try--) {
          printf("Подключение... ");
          if (connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == 0) {
            printf("Успешно!\n");
            connection_success = true;
            break;
          }
          sleep(3);
          printf("Ошибка...\n");
        }

        if (!connection_success) {
          printf("Подключение не удалось.\n");
          fsm_step = fsm_connection_break; //Переходим к обработке разрыва соединения.
        }

        /*
          Здесь должна быть отправка никнейма для сервер и каких-то еще данных (если придумается).
          По идее сервер в ответ должен послать какие-то команды.. Как пример могу предложить "sleep n" и "start"
          где start заставляет перейти клиента в состояние fsm_gamea_start, а sleep заставляет ждать n секунд, пока сервер не решит начать игру (например, не все игроки подключились или хост уснул).

          Здесь должно быть ожидание сообщения от сервера. Реализуется протоколами(?)
        */

        fsm_step = fsm_game_start;
        break;

      case fsm_game_start:

        /* Обработка текущей игры... */
        /* Отрисовка и обновление получаемых данных*/
        break;

      case fsm_move_waiting:

        /* */
        break;

      case fsm_move_input:
        /* */
        break;

      case fsm_guess_failure:
        /* */
        break;

      case fsm_guess_word_success:
        /* */
        break;

      case fsm_guess_letter_success:
        /* */
        break;

      case fsm_guess_timeout:
        /* */
        break;

      case fsm_endgame_output:
        /* */
        break;

      case fsm_exit:
        /* */
        break;

      case fsm_new_game_waiting:
        /* */
        break;

      case fsm_connection_break:
        //обработка графическая?
        //Уничтожение сокетов и, возможно, каких-то еще ресурсов.
        exit(EXIT_FAILURE);
    }
  }

  return 0;
}
