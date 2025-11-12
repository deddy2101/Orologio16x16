#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include "commonTypes.h"
#include "DisplayManager/DisplayManager.h"
#include "WebSocketManager/WebSocketManager.h"

enum Direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

enum GameState {
    GAME_READY,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_OVER
};

class SnakeGame {
private:
    static const int GRID_WIDTH = 16;
    static const int GRID_HEIGHT = 16;
    static const int MAX_SNAKE_LENGTH = GRID_WIDTH * GRID_HEIGHT;
    
    DisplayManager* display;
    WebSocketManager* wsManager;
    
    Position snake[MAX_SNAKE_LENGTH];
    int snakeLength;
    Direction currentDirection;
    Direction nextDirection; // Per evitare inversioni istantanee
    
    Position food;
    int score;
    GameState state;
    
    unsigned long lastMoveTime;
    int moveInterval; // Millisecondi tra un movimento e l'altro
    
    // Metodi privati per la logica del gioco
    void generateFood();
    bool checkCollision(Position pos);
    bool isFoodPosition(Position pos);
    void moveSnake();
    // drawGame() è stato spostato in DisplayManager
    
public:
    SnakeGame(DisplayManager* disp, WebSocketManager* ws);
    
    // Inizializza/resetta il gioco
    void init();
    void reset();
    
    // Aggiorna il gioco (da chiamare nel loop)
    void update();
    
    // Gestisce i comandi dal WebSocket
    void handleCommand(GameCommand cmd);
    
    // Getters
    int getScore() { return score; }
    GameState getState() { return state; }
    bool isRunning() { return state == GAME_RUNNING; }
    
    // Setters
    void setSpeed(int interval) { moveInterval = interval; }
};

#endif