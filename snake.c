#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "snake.h" // The game's header file.

/*
  A snake game in C using the ncurses library.
             Author: Umar Abdul.
*/


int main(int argc, char *argv[]){
  
  if (argc > 1){  // parse arguments.
    for (int i = 1; i < argc; i++){
      if (strcmp(argv[i], "--soft-walls") == 0 || strcmp(argv[i], "--soft-wall") == 0) pass_thru_walls = true;
      else if (strcmp(argv[i], "--hard-walls") == 0 || strcmp(argv[i], "--hard-wall") == 0) pass_thru_walls = false;
    }
  }
  initscr(); // drop into curses mode
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  timeout(10); // input timeout.
  refresh();
  init();
  int ch;
  WINDOW *win;
  while (1){
  	getmaxyx(stdscr, maxY, maxX);
  	maxX-=4;
  	maxY-=2;
  	win = newwin(maxY, maxX, 1, 2);
  	if (food.y >= (maxY-1) || food.x >= (maxX-1)) makeFood(); // prevents food from going out of sight when window size changes.
    ch = getch();
    if (ch == KEY_RIGHT && curr_dir != left && curr_dir != right){
      curr_dir = right;
      moves++;
    }else if (ch == KEY_LEFT && curr_dir != right && curr_dir != left){
      curr_dir = left;
      moves++;
    }else if (ch == KEY_UP && curr_dir != down && curr_dir != up){
      curr_dir = up;
      moves++;
    }else if (ch == KEY_DOWN && curr_dir != up && curr_dir != down){
      curr_dir = down;
      moves++;
    }else if (ch == 'E' || ch == 'e'){
      break;
    }else if (ch == 'P' || ch == 'p'){
      wmove(win, maxY/2, (maxX/2) - 4);
      wprintw(win, "[PAUSED]");
      wrefresh(win);
      while ((ch = getch()) == ERR) continue;
    }
    moveSnake();
    if (game_over){
      wclear(win);
      wmove(win, maxY/2, (maxX/2) - 5);
      wattron(win, A_BOLD);
      wprintw(win, "Game Over.");
      wmove(win, (maxY/2)+1, (maxX/2) - 5);
      wprintw(win, "Score: %d", score);
      wmove(win, (maxY/2)+2, (maxX/2) - 5);
      wprintw(win, "Moves: %d", moves);
      wattroff(win, A_BOLD);
      wrefresh(win);
      timeout(50000);
      getch();
      timeout(10);
      game_over = false;
      init();
    }else{
      render(win);
    }
    delwin(win);
  }
  delwin(win);
  endwin(); // exit curses();
  
  return 0;
}
