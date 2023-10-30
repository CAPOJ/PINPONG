#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void field(int WIDTH, int HEIGHT, int x_ball, int y_Ball, int x_left_platf, int y_left_platf,
           int x_right_platf, int y_right_platf, int score_left, int score_right);
int start_game();
int change_x_ball(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                  int max_y);
int change_y_ball(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                  int max_y);
int change_direction(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                     int max_y);

int main() {
    int winner;
    initscr();
    noecho();
    nodelay(stdscr, 1);
    endwin();
    winner = start_game();
    if (winner == -1) {
        printf("Congratulations, Left Player!");
    } else {
        printf("Congratulations, Right Player!");
    }
    return 0;
}

void field(int WIDTH, int HEIGHT, int x_ball, int y_Ball, int x_left_platf, int y_left_platf,
           int x_right_platf, int y_right_platf, int score_left, int score_right) {
    for (int i = 0; i < WIDTH; i++) {
        printw("-");
    }
    printw("\n");
    for (int i = HEIGHT; i > 0; i--) {
        for (int j = 0; j < WIDTH + 1; j++) {
            if (j == 0 || j == WIDTH) {
                printw("|");
            } else if (j == x_ball && i == y_Ball) {
                printw("O");
            } else if ((j == x_left_platf &&
                        (i == y_left_platf || i == y_left_platf - 1 || i == y_left_platf - 2)) ||
                       (j == x_right_platf &&
                        (i == y_right_platf || i == y_right_platf - 1 || i == y_right_platf - 2))) {
                printw("#");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    for (int i = 0; i < WIDTH; i++) {
        printw("-");
    }
    printw("\n");
    for (int i = 0; i < WIDTH; i++) {
        if (i == HEIGHT) {
            printw("%d", score_left);
        } else if (i == 60) {
            printw("%d", score_right);
        } else {
            printw(" ");
        }
    }
    printw("\n");
}

int is_goal(int y_platf, int y_ball) {
    if (y_ball == y_platf || y_ball == y_platf - 1 || y_ball == y_platf - 2) {
        return 0;
    }
    return 1;
}

int rand_func(unsigned int next) {
    next = next * 1103515245;
    return ((unsigned int)(next / 65536) * 2768);
}

int start_game() {
    char letter;
    int WIDTH = 82, HEIGHT = 25, score_left = 0, score_right = 0, x_left_platf = 1, x_right_platf = 81,
        min_y = 0, max_y = 26, direction = 1, y_left_platf = 10, y_right_platf = 10, x_ball, y_ball,
        x_ball_prev, y_ball_prev;
    unsigned int next = 1;
    x_ball = (WIDTH / 2);
    y_ball = rand_func(next) % (24 - 0 + 1) + 0;
    field(WIDTH, HEIGHT, x_ball, y_ball, x_left_platf, y_left_platf, x_right_platf, y_right_platf, score_left,
          score_right);
    while (score_left < 21 && score_right < 21) {
        letter = getch();
        next++;
        if ((letter == 'Z' || letter == 'z') && (y_left_platf != 3)) {
            --y_left_platf;
        } else if ((letter == 'm' || letter == 'M') && (y_right_platf != 3)) {
            --y_right_platf;
        } else if ((letter == 'A' || letter == 'a') && (y_left_platf <= 24)) {
            ++y_left_platf;
        } else if ((letter == 'K' || letter == 'k') && (y_right_platf <= 24)) {
            ++y_right_platf;
        }
        x_ball_prev = x_ball;
        y_ball_prev = y_ball;
        x_ball = change_x_ball(direction, x_ball, y_ball, x_left_platf, x_right_platf, min_y, max_y);
        y_ball = change_y_ball(direction, x_ball_prev, y_ball, x_left_platf, x_right_platf, min_y, max_y);
        direction =
            change_direction(direction, x_ball_prev, y_ball_prev, x_left_platf, x_right_platf, min_y, max_y);
        clear();
        field(WIDTH, HEIGHT, x_ball, y_ball, x_left_platf, y_left_platf, x_right_platf, y_right_platf,
              score_left, score_right);
        if (x_ball == x_left_platf + 1)
            if (is_goal(y_left_platf, y_ball)) {
                score_right++;
                y_ball = rand_func(next) % (24 - 0 + 1) + 0;
                x_ball = (WIDTH / 2) - 1;
                direction = -1 * (rand_func(next) % (3) + 1);
            }
        if (x_ball == x_right_platf - 1)
            if (is_goal(y_right_platf, y_ball)) {
                score_left++;
                y_ball = rand_func(next) % (24 - 0 + 1) + 0;
                x_ball = (WIDTH / 2);
                direction = 1 * (rand_func(next) % (3) + 1);
            }
        usleep(100000);
    }
    if (score_left > score_right) return -1;
    return 1;
}

int change_sign(int x) {
    if (x > 0) return -1;
    return 1;
}

int sign(int x) {
    if (x > 0) return 1;
    return -1;
}

int abs(int x) {
    if (x > 0) return x;
    return -1 * x;
}

int change_x_ball(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                  int max_y) {
    if (((x_ball == x_right_wall - 1) && direction > 0) || ((x_ball == x_left_wall + 1) && direction < 0)) {
        ;
    } else {
        x_ball += 1 * sign(direction);
        if (abs(direction) == 1) {
            if (y_ball == max_y - 1) {
                ;
            }
        } else if (abs(direction) == 3) {
        }
        if (y_ball == min_y + 1) {
            ;
        } else {
            ;
        }
    }
    return x_ball;
}

int change_y_ball(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                  int max_y) {
    if (((x_ball == x_right_wall - 1) && direction > 0) || ((x_ball == x_left_wall + 1) && direction < 0)) {
        ;
    } else {
        if (abs(direction) == 1) {
            if (y_ball == max_y - 1) {
                y_ball -= 1;
            } else {
                y_ball += 1;
            }
        } else if (abs(direction) == 3) {
            if (y_ball == min_y + 1) {
                y_ball += 1;
            } else {
                y_ball -= 1;
            }
        }
    }
    return y_ball;
}

int change_direction(int direction, int x_ball, int y_ball, int x_left_wall, int x_right_wall, int min_y,
                     int max_y) {
    if (((x_ball == x_right_wall - 1) && direction > 0) || ((x_ball == x_left_wall + 1) && direction < 0)) {
        if (y_ball == min_y + 1 || y_ball == max_y - 1) {
            direction = (4 - abs(direction)) * change_sign(direction);
        } else if (((x_ball == x_left_wall + 1) && sign(direction) < 0) ||
                   (x_ball == x_right_wall - 1 && sign(direction) > 0)) {
            direction = abs(direction) * change_sign(direction);
        }
    } else {
        if (abs(direction) == 1) {
            if (y_ball == max_y - 1) {
                direction = 3 * sign(direction);
            }
        } else if (abs(direction) == 3) {
            if (y_ball == min_y + 1) {
                direction = 1 * sign(direction);
            }
        }
    }
    return direction;
}