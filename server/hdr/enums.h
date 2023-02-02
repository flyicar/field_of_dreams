#ifndef ENUMS_H
#define ENUMS_H

//Множество состояний fsm хоста-сервера
enum fsm_states {
	fsm_start_prog, //Запуск программы
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

enum commands {
	DENIED, //Отказ сервера
	HELLO, //Отправка подтверждения на начало игры
	START, //Отправка информирования о начале игры
	USERS, //Отправка данных об игроках
	WORD, //Отправка слова 
	HINT, //Отправка подсказки
	DATA,
	MOVE,
	SUCCESS,
	INCORRECT,
	TIMEOUT,
	ENDGAME,
	ERROR,
};

#endif /*ENUMS_H*/
