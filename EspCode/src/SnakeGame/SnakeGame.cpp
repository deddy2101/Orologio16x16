#include "SnakeGame.h"

SnakeGame::SnakeGame(DisplayManager* disp, WebSocketManager* ws) 
    : display(disp), wsManager(ws), snakeLength(3), score(0), 
      state(GAME_READY), lastMoveTime(0), moveInterval(300) {
    
    currentDirection = DIR_RIGHT;
    nextDirection = DIR_RIGHT;
}

void SnakeGame::init() {
    reset();
}

void SnakeGame::reset() {
    // Inizializza il serpente al centro, lunghezza 3
    snakeLength = 3;
    snake[0] = {GRID_WIDTH / 2, GRID_HEIGHT / 2};
    snake[1] = {GRID_WIDTH / 2 - 1, GRID_HEIGHT / 2};
    snake[2] = {GRID_WIDTH / 2 - 2, GRID_HEIGHT / 2};
    
    currentDirection = DIR_RIGHT;
    nextDirection = DIR_RIGHT;
    score = 0;
    state = GAME_READY;
    moveInterval = 300; // Reset velocità
    
    generateFood();
    
    Serial.println("Snake game reset");
}

void SnakeGame::generateFood() {
    bool validPosition = false;
    
    while (!validPosition) {
        food.x = random(0, GRID_WIDTH);
        food.y = random(0, GRID_HEIGHT);
        
        // Verifica che il cibo non sia sul serpente
        validPosition = true;
        for (int i = 0; i < snakeLength; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                validPosition = false;
                break;
            }
        }
    }
    
    Serial.printf("Food generated at: %d, %d\n", food.x, food.y);
}

bool SnakeGame::checkCollision(Position pos) {
    // Collisione con i muri
    if (pos.x < 0 || pos.x >= GRID_WIDTH || pos.y < 0 || pos.y >= GRID_HEIGHT) {
        return true;
    }
    
    // Collisione con se stesso (escludi la coda perché si muoverà)
    for (int i = 0; i < snakeLength - 1; i++) {
        if (snake[i].x == pos.x && snake[i].y == pos.y) {
            return true;
        }
    }
    
    return false;
}

bool SnakeGame::isFoodPosition(Position pos) {
    return (pos.x == food.x && pos.y == food.y);
}

void SnakeGame::moveSnake() {
    // Aggiorna la direzione corrente
    currentDirection = nextDirection;
    
    // Calcola la nuova posizione della testa
    Position newHead = snake[0];
    
    switch (currentDirection) {
        case DIR_UP:
            newHead.y--;
            break;
        case DIR_DOWN:
            newHead.y++;
            break;
        case DIR_LEFT:
            newHead.x--;
            break;
        case DIR_RIGHT:
            newHead.x++;
            break;
    }
    
    // Controlla collisioni
    if (checkCollision(newHead)) {
        state = GAME_OVER;
        wsManager->sendGameState("GAME_OVER");
        wsManager->sendScore(score);
        Serial.println("Game Over!");
        
        // Mostra animazione game over usando DisplayManager
        display->scrollTextFull("GAME OVER", CRGB::Red);
        delay(2000);
        display->scrollTextFull("Score: " + String(score), CRGB::Yellow);
        delay(2000);
        
        return;
    }
    
    // Controlla se ha mangiato il cibo
    bool ateFood = isFoodPosition(newHead);
    
    if (ateFood) {
        score++;
        wsManager->sendScore(score);
        Serial.printf("Score: %d\n", score);
        
        // Aumenta la velocità ogni 5 punti
        if (score % 5 == 0 && moveInterval > 100) {
            moveInterval -= 20;
            Serial.printf("Speed increased! Interval: %d\n", moveInterval);
        }
        
        generateFood();
    }
    
    // Sposta il corpo del serpente
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    
    // Muovi la testa
    snake[0] = newHead;
    
    // Se ha mangiato, aumenta la lunghezza
    if (ateFood) {
        if (snakeLength < MAX_SNAKE_LENGTH) {
            snakeLength++;
        }
        
        // Controlla vittoria (serpente riempie tutta la griglia)
        if (snakeLength >= MAX_SNAKE_LENGTH) {
            state = GAME_OVER;
            wsManager->sendGameState("WIN");
            wsManager->sendScore(score);
            Serial.println("You Win!");
            
            display->scrollTextFull("YOU WIN!", CRGB::Green);
            delay(2000);
        }
    }
}

void SnakeGame::update() {
    if (state != GAME_RUNNING) {
        return;
    }
    
    unsigned long currentTime = millis();
    
    // Aggiorna il movimento del serpente in base all'intervallo
    if (currentTime - lastMoveTime >= moveInterval) {
        lastMoveTime = currentTime;
        moveSnake();
    }
    
    // Disegna il gioco usando DisplayManager (viene chiamato sempre per il lampeggio del cibo)
    display->drawSnakeGame(snake, snakeLength, food);
}

void SnakeGame::handleCommand(GameCommand cmd) {
    switch (cmd) {
        case CMD_START_SNAKE:
            if (state == GAME_READY || state == GAME_OVER) {
                reset();
                state = GAME_RUNNING;
                lastMoveTime = millis();
                wsManager->sendMessage("Game Started!");
                wsManager->sendScore(0);
                Serial.println("Game started!");
            }
            break;
            
        case CMD_STOP_GAME:
            if (state == GAME_RUNNING || state == GAME_PAUSED) {
                state = GAME_OVER;
                wsManager->sendGameState("STOPPED");
                Serial.println("Game stopped");
            }
            break;
            
        case CMD_PAUSE:
            if (state == GAME_RUNNING) {
                state = GAME_PAUSED;
                wsManager->sendGameState("PAUSED");
                Serial.println("Game paused");
            } else if (state == GAME_PAUSED) {
                state = GAME_RUNNING;
                lastMoveTime = millis();
                wsManager->sendGameState("RUNNING");
                Serial.println("Game resumed");
            }
            break;
            
        case CMD_UP:
            // Previeni inversione immediata (non può andare giù se sta andando su)
            if (currentDirection != DIR_DOWN) {
                nextDirection = DIR_UP;
            }
            break;
            
        case CMD_DOWN:
            if (currentDirection != DIR_UP) {
                nextDirection = DIR_DOWN;
            }
            break;
            
        case CMD_LEFT:
            if (currentDirection != DIR_RIGHT) {
                nextDirection = DIR_LEFT;
            }
            break;
            
        case CMD_RIGHT:
            if (currentDirection != DIR_LEFT) {
                nextDirection = DIR_RIGHT;
            }
            break;
            
        case CMD_NONE:
            break;
    }
}