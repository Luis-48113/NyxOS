#include "../shell/commands.h"
#include "../kernel/drivers/graphics.h"
#include "../kernel/drivers/keyboard.h"
#include "../kernel/drivers/timer.h"
#include "../lib/string.h"

#define SNAKE_MAX_LENGTH 100
#define GRID_SIZE 20
#define GAME_SPEED 200  // milliseconds

typedef struct {
    int x, y;
} point_t;

typedef struct {
    point_t body[SNAKE_MAX_LENGTH];
    int length;
    int direction;  // 0: up, 1: right, 2: down, 3: left
    point_t food;
} snake_game_t;

static snake_game_t game;
static int game_running = 0;

static void snake_init(void) {
    game.length = 3;
    game.direction = 1;  // Start moving right
    
    // Initialize snake body
    for (int i = 0; i < game.length; i++) {
        game.body[i].x = 5 - i;
        game.body[i].y = 5;
    }
    
    // Place initial food
    snake_place_food();
}

static void snake_place_food(void) {
    uint32_t width = graphics_get_width();
    uint32_t height = graphics_get_height();
    
    int grid_width = width / GRID_SIZE;
    int grid_height = height / GRID_SIZE;
    
    do {
        game.food.x = rand() % grid_width;
        game.food.y = rand() % grid_height;
    } while (snake_check_collision(game.food.x, game.food.y));
}

static int snake_check_collision(int x, int y) {
    for (int i = 0; i < game.length; i++) {
        if (game.body[i].x == x && game.body[i].y == y) {
            return 1;
        }
    }
    return 0;
}

static void snake_update(void) {
    // Move snake head
    point_t head = game.body[0];
    
    switch (game.direction) {
        case 0: head.y--; break;  // Up
        case 1: head.x++; break;  // Right
        case 2: head.y++; break;  // Down
        case 3: head.x--; break;  // Left
    }
    
    // Check wall collision
    uint32_t width = graphics_get_width();
    uint32_t height = graphics_get_height();
    int grid_width = width / GRID_SIZE;
    int grid_height = height / GRID_SIZE;
    
    if (head.x < 0 || head.x >= grid_width || head.y < 0 || head.y >= grid_height) {
        game_running = 0;
        return;
    }
    
    // Check self collision
    if (snake_check_collision(head.x, head.y)) {
        game_running = 0;
        return;
    }
    
    // Move body
    for (int i = game.length - 1; i > 0; i--) {
        game.body[i] = game.body[i - 1];
    }
    game.body[0] = head;
    
    // Check food collision
    if (head.x == game.food.x && head.y == game.food.y) {
        if (game.length < SNAKE_MAX_LENGTH) {
            game.length++;
        }
        snake_place_food();
    }
}

static void snake_draw(void) {
    graphics_clear();
    
    uint32_t width = graphics_get_width();
    uint32_t height = graphics_get_height();
    int grid_width = width / GRID_SIZE;
    int grid_height = height / GRID_SIZE;
    
    // Draw snake
    for (int i = 0; i < game.length; i++) {
        uint32_t x = game.body[i].x * GRID_SIZE;
        uint32_t y = game.body[i].y * GRID_SIZE;
        graphics_draw_rect(x, y, GRID_SIZE - 1, GRID_SIZE - 1, COLOR_GREEN);
    }
    
    // Draw food
    uint32_t food_x = game.food.x * GRID_SIZE;
    uint32_t food_y = game.food.y * GRID_SIZE;
    graphics_draw_rect(food_x, food_y, GRID_SIZE - 1, GRID_SIZE - 1, COLOR_RED);
    
    // Draw score
    char score_buf[32];
    sprintf(score_buf, "Score: %d", game.length - 3);
    graphics_draw_string(10, 10, score_buf, COLOR_WHITE, COLOR_BLACK);
}

static void snake_handle_input(void) {
    char key = keyboard_get_last_key();
    
    if (key) {
        switch (key) {
            case 'w': case 'W': if (game.direction != 2) game.direction = 0; break;
            case 'd': case 'D': if (game.direction != 3) game.direction = 1; break;
            case 's': case 'S': if (game.direction != 0) game.direction = 2; break;
            case 'a': case 'A': if (game.direction != 1) game.direction = 3; break;
            case 27: game_running = 0; break;  // ESC to quit
        }
    }
}

int cmd_snake(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    if (!graphics_has_framebuffer()) {
        screen_write_string("Snake requires framebuffer graphics!\n");
        return 1;
    }
    
    // Seed random number generator
    srand(timer_get_ticks());
    
    snake_init();
    game_running = 1;
    
    screen_write_string("Snake Game Started!\n");
    screen_write_string("Controls: WASD to move, ESC to quit\n");
    
    while (game_running) {
        snake_handle_input();
        snake_update();
        snake_draw();
        timer_wait(GAME_SPEED);
    }
    
    graphics_clear();
    screen_write_string("Game Over! Final Score: ");
    char score_buf[10];
    sprintf(score_buf, "%d", game.length - 3);
    screen_write_string(score_buf);
    screen_write_string("\n");
    
    return 0;
}