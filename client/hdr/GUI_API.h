#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>

/*
    Здесь пишем свои пожелания, нужные правки и т.д....
    1. Доработать функцию выхода из программы


    Тестовая программы в конце, просто копируем в другой файл и тестим
*/

#define top_bottom_pair 1//цветовая палитра для нижней и верхней рамки 
#define wind_pair 2//цветовая палитра для окон
#define but_pair 3//Для кнопки
int termx, termy, winx, winy,centery,centerx;

WINDOW  * start_wind, *mgwin, *sgwin, *sgwins, *pgwin, *pgwins; //Объявление переменных окон

void terminal_start();
int start_win();
int game_wins();
int end_win();
int window_players();
void hascolors();
void enablecolors();
void terminal_stop();
void get_window_dimensions();
void resizehandler();

//void exit_size();

// Старт программы

void terminal_start() {

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();
}
// Очистка терминала
void terminal_stop() {

	endwin();
}
//Получение размеров экрана и запись данных в глобальные переменные
void get_window_dimensions() {

	getmaxyx(stdscr, termy, termx);
    getmaxyx(mgwin, winy, winx);
    centery = termy/2;
    centerx = termx/2;

    start_wind = newwin(termy-2,termx,1,0);//задание размеров для окна в стартовом фрейме
    mgwin = newwin(termy-2,termx - (termx/3),1,0);//задание размеров для главного окна в игоровом фрейме
    sgwin = newwin((termy/6)-1,(termx/3),1,termx-(termx/3));//задание размеров для окна "ИГРОКИ" в игровом фрейме
    sgwins = newwin((termy/6)-1,(termx/3),0,termx-(termx/1.45));
    pgwin = newwin((termy-(termy/6))-1,termx/3,termy/6,termx-(termx/3));//задание размеров для окна вывода никоа игроков в игровом фрейме
    pgwins = newwin((termy-(termy/6))-1,termx/3,termy/6,termx-(termx/1.45));
}
// проверка на поддержу цветов в терминале
void hascolors()
{
    if(has_colors() == false)
    {
        endwin();
        printf("your terminal does not support colors!");
        exit(1);
    }
}
//Инициализация цветов
void enablecolors()
{
    start_color();
    init_pair(top_bottom_pair,COLOR_BLACK, COLOR_CYAN);
    init_pair(wind_pair,COLOR_WHITE,COLOR_BLUE);
    init_pair(but_pair,COLOR_RED,COLOR_CYAN);
}
//Стартовый фрейм программы
int start_win(WINDOW* win)
{
    int gg=1;
    hascolors();
    enablecolors();
    wbkgd(stdscr,COLOR_PAIR(top_bottom_pair));
    box(win,0,0);
	wbkgd(win,COLOR_PAIR(wind_pair));
    mvwprintw(win,centery-2,centerx-5,"ПОЛЕ ЧУДЕС");
    mvwprintw(win,centery+3,centerx-17,"Нажмите ENTER, чтобы продолжить...");
    mvprintw(termy-1,0,"ВЫХОД");
    attron(COLOR_PAIR(but_pair));
    mvprintw(termy-1,6,"'q'");
    attroff(COLOR_PAIR(but_pair));
	wrefresh(stdscr);   
    wrefresh(win);
    return gg;
}

//Конечный фрейм программы
int end_win(WINDOW* win)
{
    int gg=2;
    hascolors();
    enablecolors();
    wbkgd(stdscr,COLOR_PAIR(top_bottom_pair));
    box(win,0,0);
	wbkgd(win,COLOR_PAIR(wind_pair));
    mvwprintw(win,centery-2,centerx-5,"Игра окончена");
    mvwprintw(win,centery+1,centerx-8,"Спасибо за игру");
    mvprintw(termy-1,0,"ВЫХОД");
    attron(COLOR_PAIR(but_pair));
    mvprintw(termy-1,6,"'q'");
    attroff(COLOR_PAIR(but_pair));
	wrefresh(stdscr);   
    wrefresh(win);
    return gg;
}


// отрисовка окон в игровом фрейме
int game_wins(WINDOW* win,WINDOW* win1,WINDOW* win2)
{
    
    int gg = 3;//флаг фрейма для обработчика
    hascolors();
    enablecolors();
    wbkgd(stdscr,COLOR_PAIR(top_bottom_pair));
    box(win,0,0);
    box(win1,0,0);
    box(win2,0,0);
    getmaxyx(win, winy, winx);
    wbkgd(win,COLOR_PAIR(wind_pair));
    wbkgd(win1,COLOR_PAIR(wind_pair));
    wbkgd(win2,COLOR_PAIR(wind_pair));
    mvwprintw(win,0,(winx/2)-5,"ПОЛЕ ЧУДЕС");//вот эта строка работает криво(артефакты)
    mvprintw(termy-1,0,"ВЫХОД");
    attron(COLOR_PAIR(but_pair));
    mvprintw(termy-1,6,"'q'");
    attroff(COLOR_PAIR(but_pair));
    mvprintw(termy-1,10,"НАЗВАТЬ СЛОВО");
    attron(COLOR_PAIR(but_pair));
    mvprintw(termy-1,24,"F2");
    attroff(COLOR_PAIR(but_pair));
    wrefresh(stdscr);   
    wrefresh(win);
    wrefresh(win1);
    wrefresh(win2);
    return gg;
}

// i stole this idea of a resize handler which just calls start and stop from stone soup - http://crawl.develz.org/
//Обработчик изменнения размеров терминала(Работает с артефактами, но вроде пока пойдет)
void resizehandler(int c) {

	terminal_stop();
	terminal_start();

	get_window_dimensions();

    if (c == 1)
    {
    start_win(start_wind);
    }
    if (c == 2)
    {
    end_win(start_wind);
    }
    if (c == 3)
    {
    game_wins(mgwin,sgwin,pgwin);
    }
    if (c == 4)
    {
    window_players(mgwin,sgwins,pgwins);  
    }
   
}

//Отрисовка вывода списка игроков
int window_players(WINDOW* win,WINDOW* win1,WINDOW* win2)
{
    int gg=4;
    hascolors();
    enablecolors();
    wbkgd(stdscr,COLOR_PAIR(top_bottom_pair));
    box(win,0,0);
    box(win1,0,0);
    box(win2,0,0);
    getmaxyx(win, winy, winx);
    wbkgd(win,COLOR_PAIR(wind_pair));
    wbkgd(win1,COLOR_PAIR(wind_pair));
    wbkgd(win2,COLOR_PAIR(wind_pair));
    mvprintw(termy-1,0,"ВЫХОД");
    attron(COLOR_PAIR(but_pair));
    mvprintw(termy-1,6,"'q'");
    attroff(COLOR_PAIR(but_pair));
    wrefresh(stdscr); 
    wrefresh(win1);
    wrefresh(win2);
    return gg;
}



//Выход из программы и ловля сигнала SIGWINCH(под вопросом(работает криво))
/*void exit_size(int c)
{
        
		c = getch();

		if (c == 'q' || c == 27 )
        {
		    exit(1);
        }
        else if  (wgetch(stdscr) == KEY_RESIZE) resizehandler();
}*/
//тестовая программа
/*int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
	terminal_start();

	termx = 0; termy = 0; mapwinx = 0; mapwiny = 0; sidebarx = 25; sidebary = 0;

	int c;
	get_window_dimensions();

	// game loop here
	while (true) {
        start_win(start_wind);

		// handle input
		c = 0;
		c = getch();

		if (c == 'q' || c == 27 ESC)
        {
		    break;
        }
        else if  (wgetch(stdscr) == KEY_RESIZE) resizehandler();
	}
	terminal_stop();

	return 0;
}*/
