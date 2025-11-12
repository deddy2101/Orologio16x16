#include "PingPongGame.h"

PingPongGame::PingPongGame(DisplayManager* disp, WebSocketManager* ws) 
    : display(disp), wsManager(ws), 
      paddle1Y(GRID_HEIGHT / 2), paddle2Y(GRID_HEIGHT / 2),
      score1(0), score2(0), state(PONG_READY), 
      lastUpdateTime(0), updateInterval(16) {  // ~60 FPS
}

void PingPongGame::init() {
    reset();
}

void PingPongGame::reset() {
    // Reset posizioni racchette al centro
    paddle1Y = GRID_HEIGHT / 2;
    paddle2Y = GRID_HEIGHT / 2;
    
    // Reset punteggi
    score1 = 0;
    score2 = 0;
    
    // Reset stato
    state = PONG_READY;
    
    // Reset pallina
    resetBall(true);
    
    Serial.println("PingPong game reset");
}

void PingPongGame::resetBall(bool player1Scored) {
    // Posiziona la pallina al centro
    ballPosX = GRID_WIDTH / 2.0f;
    ballPosY = GRID_HEIGHT / 2.0f;
    ball.x = (int)ballPosX;
    ball.y = (int)ballPosY;
    
    // Direzione iniziale: verso il giocatore che ha perso il punto
    // Velocità base: 0.15 pixel per frame (~9 pixel al secondo a 60fps)
    ballVelX = player1Scored ? -0.15f : 0.15f;
    
    // Angolo casuale verticale
    ballVelY = (random(-100, 100) / 100.0f) * 0.1f;
    
    Serial.printf("Ball reset. Vel: %.2f, %.2f\n", ballVelX, ballVelY);
}

void PingPongGame::movePaddle(int& paddleY, bool up) {
    if (up) {
        // Muovi su (assicurati che la parte superiore della racchetta non esca)
        if (paddleY > PADDLE_SIZE / 2) {
            paddleY--;
        }
    } else {
        // Muovi giù (assicurati che la parte inferiore non esca)
        if (paddleY < GRID_HEIGHT - 1 - PADDLE_SIZE / 2) {
            paddleY++;
        }
    }
}

bool PingPongGame::checkPaddleCollision() {
    int ballIntX = (int)ballPosX;
    int ballIntY = (int)ballPosY;
    
    // Controlla collisione con racchetta sinistra (giocatore 1)
    if (ballIntX == 1 && ballVelX < 0) {
        int paddle1Top = paddle1Y - PADDLE_SIZE / 2;
        int paddle1Bottom = paddle1Y + PADDLE_SIZE / 2;
        
        if (ballIntY >= paddle1Top && ballIntY <= paddle1Bottom) {
            // Collisione! Inverti direzione X
            ballVelX = -ballVelX * 1.05f;  // Aumenta leggermente la velocità
            
            // Calcola angolo di rimbalzo basato su dove ha colpito la racchetta
            float hitPos = (ballIntY - paddle1Y) / (float)(PADDLE_SIZE / 2);
            ballVelY += hitPos * 0.08f;
            
            // Limita la velocità verticale
            if (ballVelY > 0.2f) ballVelY = 0.2f;
            if (ballVelY < -0.2f) ballVelY = -0.2f;
            
            return true;
        }
    }
    
    // Controlla collisione con racchetta destra (giocatore 2)
    if (ballIntX == GRID_WIDTH - 2 && ballVelX > 0) {
        int paddle2Top = paddle2Y - PADDLE_SIZE / 2;
        int paddle2Bottom = paddle2Y + PADDLE_SIZE / 2;
        
        if (ballIntY >= paddle2Top && ballIntY <= paddle2Bottom) {
            // Collisione! Inverti direzione X
            ballVelX = -ballVelX * 1.05f;
            
            // Calcola angolo di rimbalzo
            float hitPos = (ballIntY - paddle2Y) / (float)(PADDLE_SIZE / 2);
            ballVelY += hitPos * 0.08f;
            
            // Limita la velocità verticale
            if (ballVelY > 0.2f) ballVelY = 0.2f;
            if (ballVelY < -0.2f) ballVelY = -0.2f;
            
            return true;
        }
    }
    
    return false;
}

void PingPongGame::updateBall() {
    // Aggiorna posizione
    ballPosX += ballVelX;
    ballPosY += ballVelY;
    
    // Rimbalzo sui bordi superiore e inferiore
    if (ballPosY <= 0) {
        ballPosY = 0;
        ballVelY = -ballVelY;
    }
    if (ballPosY >= GRID_HEIGHT - 1) {
        ballPosY = GRID_HEIGHT - 1;
        ballVelY = -ballVelY;
    }
    
    // Controlla collisione con le racchette
    checkPaddleCollision();
    
    // Controlla se la pallina è uscita dai lati (punto segnato)
    if (ballPosX < 0) {
        // Giocatore 2 ha segnato
        score2++;
        wsManager->sendMessage("SCORE:P2:" + String(score2));
        Serial.printf("Player 2 scores! Score: %d - %d\n", score1, score2);
        resetBall(false);
        checkWin();
    } else if (ballPosX >= GRID_WIDTH) {
        // Giocatore 1 ha segnato
        score1++;
        wsManager->sendMessage("SCORE:P1:" + String(score1));
        Serial.printf("Player 1 scores! Score: %d - %d\n", score1, score2);
        resetBall(true);
        checkWin();
    }
    
    // Aggiorna la posizione intera
    ball.x = (int)ballPosX;
    ball.y = (int)ballPosY;
}

void PingPongGame::checkWin() {
    if (score1 >= WINNING_SCORE) {
        state = PONG_GAME_OVER;
        wsManager->sendGameState("PLAYER1_WIN");
        Serial.println("Player 1 wins!");
        
        display->scrollTextFull("P1 WINS!", CRGB::Green);
        delay(2000);
        
    } else if (score2 >= WINNING_SCORE) {
        state = PONG_GAME_OVER;
        wsManager->sendGameState("PLAYER2_WIN");
        Serial.println("Player 2 wins!");
        
        display->scrollTextFull("P2 WINS!", CRGB::Blue);
        delay(2000);
    }
}

void PingPongGame::update() {
    if (state != PONG_RUNNING) {
        return;
    }
    
    unsigned long currentTime = millis();
    
    // Aggiorna ad intervalli fissi per movimento fluido
    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime;
        updateBall();
    }
    
    // Disegna il gioco
    display->clearDisplay();
    
    // Disegna racchetta giocatore 1 (sinistra, blu)
    int paddle1Top = paddle1Y - PADDLE_SIZE / 2;
    for (int i = 0; i < PADDLE_SIZE; i++) {
        int y = paddle1Top + i;
        if (y >= 0 && y < GRID_HEIGHT) {
            int matrixIndex = y * GRID_WIDTH + 0;  // Colonna 0
            int ledIndex = display->ledMap[matrixIndex];
            if (ledIndex >= 0 && ledIndex < 256) {
                display->leds[ledIndex] = CRGB::Blue;
            }
        }
    }
    
    // Disegna racchetta giocatore 2 (destra, verde)
    int paddle2Top = paddle2Y - PADDLE_SIZE / 2;
    for (int i = 0; i < PADDLE_SIZE; i++) {
        int y = paddle2Top + i;
        if (y >= 0 && y < GRID_HEIGHT) {
            int matrixIndex = y * GRID_WIDTH + (GRID_WIDTH - 1);  // Ultima colonna
            int ledIndex = display->ledMap[matrixIndex];
            if (ledIndex >= 0 && ledIndex < 256) {
                display->leds[ledIndex] = CRGB::Green;
            }
        }
    }
    
    // Disegna la pallina (bianca lampeggiante)
    if (millis() % 200 < 100) {  // Lampeggia più velocemente
        if (ball.x >= 0 && ball.x < GRID_WIDTH && ball.y >= 0 && ball.y < GRID_HEIGHT) {
            int matrixIndex = ball.y * GRID_WIDTH + ball.x;
            int ledIndex = display->ledMap[matrixIndex];
            if (ledIndex >= 0 && ledIndex < 256) {
                display->leds[ledIndex] = CRGB::White;
            }
        }
    }
    
    // Disegna linea centrale tratteggiata (grigio scuro)
    for (int y = 0; y < GRID_HEIGHT; y += 2) {
        int matrixIndex = y * GRID_WIDTH + (GRID_WIDTH / 2);
        int ledIndex = display->ledMap[matrixIndex];
        if (ledIndex >= 0 && ledIndex < 256) {
            display->leds[ledIndex] = CRGB(30, 30, 30);
        }
    }
    
    FastLED.show();
}

void PingPongGame::handleCommand(GameCommand cmd, int player) {
    // Gestisce i comandi di controllo del gioco
    switch (cmd) {
        case CMD_START_PONG:
            if (state == PONG_READY || state == PONG_GAME_OVER) {
                reset();
                state = PONG_RUNNING;
                lastUpdateTime = millis();
                wsManager->sendMessage("PONG:Game Started!");
                wsManager->sendMessage("SCORE:P1:0");
                wsManager->sendMessage("SCORE:P2:0");
                Serial.println("PingPong started!");
            }
            break;
            
        case CMD_STOP_GAME:
            if (state == PONG_RUNNING || state == PONG_PAUSED) {
                state = PONG_GAME_OVER;
                wsManager->sendGameState("STOPPED");
                Serial.println("PingPong stopped");
            }
            break;
            
        case CMD_PAUSE:
            if (state == PONG_RUNNING) {
                state = PONG_PAUSED;
                wsManager->sendGameState("PAUSED");
                Serial.println("PingPong paused");
            } else if (state == PONG_PAUSED) {
                state = PONG_RUNNING;
                lastUpdateTime = millis();
                wsManager->sendGameState("RUNNING");
                Serial.println("PingPong resumed");
            }
            break;
            
        case CMD_UP:
            if (state == PONG_RUNNING) {
                if (player == 1) {
                    movePaddle(paddle1Y, true);
                } else if (player == 2) {
                    movePaddle(paddle2Y, true);
                }
            }
            break;
            
        case CMD_DOWN:
            if (state == PONG_RUNNING) {
                if (player == 1) {
                    movePaddle(paddle1Y, false);
                } else if (player == 2) {
                    movePaddle(paddle2Y, false);
                }
            }
            break;
            
        case CMD_LEFT:
        case CMD_RIGHT:
        case CMD_NONE:
            break;
    }
}