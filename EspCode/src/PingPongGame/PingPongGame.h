#ifndef PINGPONGGAME_H
#define PINGPONGGAME_H

#include "commonTypes.h"
#include "DisplayManager/DisplayManager.h"
#include "WebSocketManager/WebSocketManager.h"

enum PongGameState {
    PONG_READY,
    PONG_RUNNING,
    PONG_PAUSED,
    PONG_GAME_OVER
};

class PingPongGame {
private:
    static const int GRID_WIDTH = 16;
    static const int GRID_HEIGHT = 16;
    static const int PADDLE_SIZE = 3;
    static const int WINNING_SCORE = 5;
    
    DisplayManager* display;
    WebSocketManager* wsManager;
    
    // Posizioni delle racchette (coordinate Y del pixel centrale)
    int paddle1Y;  // Giocatore 1 (sinistra)
    int paddle2Y;  // Giocatore 2 (destra)
    
    // Posizione e velocità della pallina
    Position ball;
    float ballVelX;
    float ballVelY;
    float ballPosX;  // Posizione float per movimento fluido
    float ballPosY;
    
    // Punteggi
    int score1;
    int score2;
    
    // Stato del gioco
    PongGameState state;
    
    // Timing
    unsigned long lastUpdateTime;
    int updateInterval;
    
    // Metodi privati per la logica del gioco
    void resetBall(bool player1Scored);
    void movePaddle(int& paddleY, bool up);
    bool checkPaddleCollision();
    void updateBall();
    void checkWin();
    
public:
    PingPongGame(DisplayManager* disp, WebSocketManager* ws);
    
    // Inizializza/resetta il gioco
    void init();
    void reset();
    
    // Aggiorna il gioco (da chiamare nel loop)
    void update();
    
    // Gestisce i comandi dal WebSocket
    void handleCommand(GameCommand cmd, int player);
    
    // Getters
    int getScore1() { return score1; }
    int getScore2() { return score2; }
    PongGameState getState() { return state; }
    bool isRunning() { return state == PONG_RUNNING; }
};

#endif