#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>

// Enum per i comandi ricevuti dal WebSocket
enum GameCommand {
    CMD_NONE,
    CMD_START_SNAKE,
    CMD_STOP_GAME,
    CMD_UP,
    CMD_DOWN,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_PAUSE
};

class WebSocketManager {
private:
    AsyncWebSocket ws;
    GameCommand lastCommand;
    bool commandAvailable;
    
    // Callback per gestire gli eventi WebSocket
    static void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                  AwsEventType type, void *arg, uint8_t *data, size_t len);
    
    // Riferimento statico per accedere all'istanza nel callback statico
    static WebSocketManager* instance;
    
    // Metodo per processare i messaggi ricevuti
    void handleMessage(String message);

public:
    WebSocketManager();
    
    // Inizializza il WebSocket server e lo collega al server HTTP
    void init(AsyncWebServer* server);
    
    // Ottiene l'ultimo comando ricevuto e lo resetta
    GameCommand getCommand();
    
    // Verifica se c'è un comando disponibile
    bool hasCommand();
    
    // Invia un messaggio a tutti i client connessi
    void sendMessage(String message);
    
    // Invia il punteggio ai client
    void sendScore(int score);
    
    // Invia lo stato del gioco (game over, win, etc)
    void sendGameState(String state);
};

#endif