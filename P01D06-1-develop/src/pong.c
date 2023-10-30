#include <stdio.h>

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
        printf("-");  //печать потолка
    }
    printf("\n");
    for (int i = HEIGHT; i > 0; i--) {
        for (int j = 0; j < WIDTH + 1; j++) {
            if (j == 0 || j == WIDTH) {
                printf("|");  //печать стен
            } else if (j == x_ball && i == y_Ball) {
                printf("O");  //печать шарика
            } else if ((j == x_left_platf &&
                        (i == y_left_platf || i == y_left_platf - 1 || i == y_left_platf - 2)) ||
                       (j == x_right_platf &&
                        (i == y_right_platf || i == y_right_platf - 1 || i == y_right_platf - 2))) {
                printf("#");  // печать платформ
            } else {
                printf(" ");  // печать пустоты
            }
        }
        printf("\n");
    }
    for (int i = 0; i < WIDTH; i++) {
        printf("-");  // печать пола
    }
    printf("\n");
    for (int i = 0; i < WIDTH; i++) {  // вывод счета игроков
        if (i == HEIGHT) {
            printf("%d", score_left);
        } else if (i == 60) {
            printf("%d", score_right);
        } else {
            printf(" ");
        }
    }
    printf("\n");
}

int is_goal(int y_platf, int y_ball) {  //проверка на гол
    if (y_ball == y_platf || y_ball == y_platf - 1 || y_ball == y_platf - 2) {
        return 0;
    }
    return 1;
}

int rand(unsigned int next) {  // рандомизация
    next = next * 1103515245;
    return ((unsigned int)(next / 65536) * 2768);
}

int right_input(char letter) {  // считывание ввода
    if (letter != ' ' && letter != 'z' && letter != 'Z' && letter != 'm' && letter != 'M' && letter != 'A' &&
        letter != 'a' && letter != 'k' && letter != 'K') {
        return 0;
    }
    return 1;
}

int start_game() {
    char letter;
    int WIDTH = 82, HEIGHT = 25, score_left = 0, score_right = 0, x_left_platf = 1, x_right_platf = 81,
        min_y = 0, max_y = 26, direction = 1, y_left_platf = 10, y_right_platf = 10, x_ball, y_ball,
        x_ball_prev, y_ball_prev;
    unsigned int next = 1;
    x_ball = (WIDTH / 2);
    y_ball = rand(next) % (24 - 0 + 1) + 0;
    field(WIDTH, HEIGHT, x_ball, y_ball, x_left_platf, y_left_platf, x_right_platf - 2, y_right_platf,
          score_left, score_right);
    while (score_left < 21 && score_right < 21) {
        letter = getchar();
        if (!right_input(letter)) continue;
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
        field(WIDTH, HEIGHT, x_ball, y_ball, x_left_platf, y_left_platf, x_right_platf, y_right_platf,
              score_left, score_right);

        if (x_ball == x_left_platf + 1) {
            if (is_goal(y_left_platf, y_ball) == 1) {
                score_right++;
                y_ball = rand(next) % (24 - 0 + 1) + 0;
                x_ball = (WIDTH / 2) - 1;
                direction = -1 * (rand(next) % (3) + 1);
            }
        }
        if (x_ball == x_right_platf - 1) {
            int n = is_goal(y_right_platf, y_ball);
            if (n == 1) {
                score_left++;
                y_ball = rand(next) % (24 - 0 + 1) + 0;
                x_ball = (WIDTH / 2);
                direction = 1 * (rand(next) % (3) + 1);
            }
        }
    }
    if (score_left > score_right) {
        return -1;
    } else {
        return 1;
    }
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
