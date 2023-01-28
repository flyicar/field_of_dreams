#ifndef STATES_H
#define STATES_H

//Множество состояний fsm хоста-сервера
enum fsm_states {
    fsm_wait_client, //Ожидание подключения клиентов
    fsm_start_game, //Начало игры
    fsm_wait_word_hint, //Ожидание ввода слова и подсказки от хоста-сервера 
    fsm_select_client_move, //Выбор хода следующего игрока
    fsm_wait_client_move_result, //Ожидание результатов хода 
    fsm_handle_client_inf, //Обработка информации от клиента
    fsm_guess_letter, //Угадана буква
    fsm_guess_word, //Угадано слово 
    fsm_guess_nothing, //Ничего не угадано
    fsm_client_win, //Победа игрока
    fsm_wait_new_game, //Ожидание начала новой игры
    fsm_disconnect_other_player_move, //Отключение клиента в чужой ход 
    fsm_end_prog, //Завершение программы
    fsm_error //Ошибка выполнения программы
};

#endif /*STATES_H*/
