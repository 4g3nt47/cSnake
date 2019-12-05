#ifndef _SNAKE_H
#define _SNAKE_H

#define max_snake_len 300
#define starting_snake_len 6

const int delay = 65000; // Controls gameplay speed. Lower value may cause flickering.
bool pass_thru_walls = true; // Allow/disallow passage through walls.

typedef struct _point{
  int x; int y;
} point;

typedef enum {
  right, left, up, down
} direction;

int snake_len, score, moves;
int maxX, maxY, height, width;
bool game_over = false;
direction curr_dir;

point snake[max_snake_len] = {};
point food;

void makeFood();

void init(){
  
  // Called when a game starts.
  curr_dir = right;
  getmaxyx(stdscr, maxY, maxX);
  maxX-=4;
  maxY-=2;
  // create default snake coordinates.
  snake_len = starting_snake_len;
  score = 0;
  moves = 0;
  for (int i = 0; i < snake_len; i++){
  	snake[i].y = maxY/2;
  	snake[i].x = (snake_len-i) + 2;
  }
  makeFood();
}

void makeFood(){
  
  // Generate food coordinate.
  srand(time(NULL));
  food.x = (rand() % (maxX-1));
  food.y = (rand() % (maxY-1));
  if (food.x == 0) food.x += 1;
  if (food.y == 0) food.y += 1;
}

void moveSnake(){

  // Updates snake coordinates.
  int nextX = 0, nextY = 0;
  if (curr_dir == right) nextX = 1;
  else if (curr_dir == left) nextX = -1;
  else if (curr_dir == up) nextY = -1;
  else nextY= 1;
  point tmp = snake[0];
  for (int i = snake_len-1; i >= 0; i--){
  	if (i == 0){ // head.
      snake[0].x += nextX;
      snake[0].y += nextY;
  	}else{
  	  snake[i] = snake[i-1]; 
    } 
  }
  // check if snake crosses itself.
  for (int i = 1; i < snake_len; i++){
    if (snake[i].x == snake[0].x && snake[i].y == snake[0].y){
      game_over = true;
      break;
    }
  }
  // Allow passing through walls and appearing in the opposite end.
  if(pass_thru_walls){
    if (snake[0].x < 1 && curr_dir == left) snake[0].x = maxX-2;
    else if (snake[0].x >= (maxX-1) && curr_dir == right) snake[0].x = 1;
    else if (snake[0].y < 1 && curr_dir == up) snake[0].y = maxY-2;
    else if (snake[0].y >= (maxY-1) && curr_dir == down) snake[0].y = 1;
  }else{
  	if ((snake[0].x < 1 && curr_dir == left) || (snake[0].x > (maxX-1) && curr_dir == right) || (snake[0].y < 1 && curr_dir == up) || (snake[0].y >= (maxY-1))){
  	  game_over = true;
  	  return;
  	}
  }
  // check if food is at same coordinate as head.
  if (snake[0].x == food.x && snake[0].y == food.y){
  	score += 5;
  	makeFood();
  	// increase length after every food eaten.
  	if (snake_len < max_snake_len){
      point lcoord = snake[snake_len-1];
      snake_len++;
      snake[snake_len-1].x = lcoord.x - nextX;
      snake[snake_len-1].y = lcoord.y - nextY;
  	}
  }
}

void render(WINDOW *win){
  
  // Render contents to screen.
  wclear(win);
  clear();
  box(win, 0, 0);
  wmove(win, 0, 2);
  wprintw(win, "[ Score: %d ]", score);
  wmove(win, maxY-1, 2);
  wprintw(win, "[ Moves: %d ]", moves);
  wmove(win, 0, maxX-17);
  if (pass_thru_walls) wprintw(win, "[ Walls: soft ]");
  else wprintw(win, "[ Walls: hard ]");
  wmove(win, maxY-1, maxX-16);
  wprintw(win, "[ Length: %d ]", snake_len);
  for (int i = 0; i < snake_len; i++){
  	wmove(win, snake[i].y, snake[i].x);
  	if (i > 2){
      waddch(win, 'x'); // tail marker.
    }else if (i != 0){
      waddch(win, '+'); // neck marker.
    }else{ // mark head.
      char c;
      if (curr_dir == up) c = '^';
      else if (curr_dir == down) c = 'v';
      else if (curr_dir == right) c = '>';
      else c = '<';
      waddch(win, c);
    }
  }
  wmove(win, food.y, food.x);
  move(0, ((maxX+2)/2) - 15);
  printw("Use 'E' to exit, 'P' to pause.");
  wprintw(win, "o");
  refresh();
  wrefresh(win);
  usleep(delay);
}

#endif
