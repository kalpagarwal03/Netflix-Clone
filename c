#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct Node {
    int x, y;
    struct Node* next;
} Node;

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Direction;

typedef enum {
    RED, PURPLE
} FoodType;

typedef struct {
    int x, y;
    FoodType type;
} Food;

typedef struct {
    Node* head;
    Direction direction;
    int length;
    int score;
} Snake;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

Node* createNode(int x, int y) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new node\n");
        exit(1);
    }
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, int x, int y) {
    Node* newNode = createNode(x, y);
    newNode->next = *head;
    *head = newNode;
}

void deleteNode(Node** head) {
    if (*head == NULL) {
        return;
    }
    Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

void displayBoard(Snake* snake, Food* food) {
    system("cls");
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf("#");
            } else if (i == food->y && j == food->x) {
                if (food->type == RED) {
                    printf("R");
                } else {
                    printf("P");
                }
            } else {
                Node* current = snake->head;
                int isBody = 0;
                while (current != NULL) {
                    if (current->x == j && current->y == i) {
                        printf("O");
                        isBody = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!isBody) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", snake->score);
}

void updateSnake(Snake* snake, int dx, int dy) {
    int newX = snake->head->x + dx;
    int newY = snake->head->y + dy;

    insertNode(&(snake->head), newX, newY);

    if (snake->length < snake->score) {
        snake->length++;
    } else {
        deleteNode(&(snake->head));
    }
}

int isCollision(Snake* snake) {
    Node* current = snake->head->next;
    while (current != NULL) {
        if (current->x == snake->head->x && current->y == snake->head->y) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int main() {
    Snake snake;
    snake.head = createNode(WIDTH / 2, HEIGHT / 2);
    snake.direction = RIGHT;
    snake.length = 1;
    snake.score = 0;

    Food food;
    food.type = RED;
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;

    int dx = 0, dy = 0;
    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 'w':
                    dx = 0;
                    dy = -1;
                    snake.direction = UP;
                    break;
                case 's':
                    dx = 0;
                    dy = 1;
                    snake.direction = DOWN;
                    break;
                case 'a':
                    dx = -1;
                    dy = 0;
                    snake.direction = LEFT;
                    break;
                case 'd':
                    dx = 1;
                    dy = 0;
                    snake.direction = RIGHT;
                    break;
                case 'q':
                    exit(0);
            }
        }

        updateSnake(&snake, dx, dy);

        if (snake.head->x == food.x && snake.head->y == food.y) {
            snake.score++;
            if (food.type == PURPLE) {
                Node* current = snake.head;
                Node* prev = NULL;
                while (current != NULL) {
                    Node* next = current->next;
                    current->next = prev;
                    prev = current;
                    current = next;
                }
                snake.head = prev;
            }

            food.type = rand() % 2 == 0 ? RED : PURPLE;
            food.x = rand() % (WIDTH - 2) + 1;
            food.y = rand() % (HEIGHT - 2) + 1;
        }

        if (snake.head->x <= 0 || snake.head->x >= WIDTH - 1 ||
            snake.head->y <= 0 || snake.head->y >= HEIGHT - 1 ||
            isCollision(&snake)) {
            printf("Game Over!\n");
            break;
        }

        displayBoard(&snake, &food);
        Sleep(100);
    }

    return 0;
}