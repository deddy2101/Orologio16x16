#include "WebSocketManager.h"

// Inizializza il puntatore statico
WebSocketManager* WebSocketManager::instance = nullptr;

WebSocketManager::WebSocketManager() : ws("/ws"), commandAvailable(false) {
    lastInput.command = CMD_NONE;
    lastInput.player = 0;
    instance = this;
}

void WebSocketManager::init(AsyncWebServer* server) {
    // Registra il callback per gli eventi WebSocket
    ws.onEvent(onWebSocketEvent);
    
    // Aggiungi il WebSocket al server
    server->addHandler(&ws);
    
    Serial.println("WebSocket server initialized on /ws");
}

void WebSocketManager::onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                         AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        client->text("Connected to ESP32 Game Server");
        
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0; // Null-terminate
            String message = String((char*)data);
            
            // Processa il messaggio ricevuto
            if (instance) {
                instance->handleMessage(message);
            }
        }
    }
}

void WebSocketManager::handleMessage(String message) {
    message.trim();
    message.toUpperCase();
    
    Serial.print("WebSocket received: ");
    Serial.println(message);
    
    // Formato dei messaggi per PingPong: "P1:UP", "P2:DOWN", ecc.
    // Per Snake: "UP", "DOWN", "START", ecc.
    
    int player = 0;
    GameCommand cmd = CMD_NONE;
    
    // Controlla se il messaggio contiene un identificatore di giocatore
    if (message.startsWith("P1:")) {
        player = 1;
        message = message.substring(3); // Rimuovi "P1:"
    } else if (message.startsWith("P2:")) {
        player = 2;
        message = message.substring(3); // Rimuovi "P2:"
    }
    
    // Mappa i messaggi ai comandi
    if (message == "START" || message == "START_SNAKE") {
        cmd = CMD_START_SNAKE;
    } 
    else if (message == "START_PONG" || message == "PONG") {
        cmd = CMD_START_PONG;
    }
    else if (message == "STOP" || message == "STOP_GAME") {
        cmd = CMD_STOP_GAME;
    }
    else if (message == "UP" || message == "W") {
        cmd = CMD_UP;
    }
    else if (message == "DOWN" || message == "S") {
        cmd = CMD_DOWN;
    }
    else if (message == "LEFT" || message == "A") {
        cmd = CMD_LEFT;
    }
    else if (message == "RIGHT" || message == "D") {
        cmd = CMD_RIGHT;
    }
    else if (message == "PAUSE") {
        cmd = CMD_PAUSE;
    }
    
    if (cmd != CMD_NONE) {
        lastInput.command = cmd;
        lastInput.player = player;
        commandAvailable = true;
    }
}

GameCommand WebSocketManager::getCommand() {
    if (commandAvailable) {
        commandAvailable = false;
        GameCommand cmd = lastInput.command;
        lastInput.command = CMD_NONE;
        lastInput.player = 0;
        return cmd;
    }
    return CMD_NONE;
}

GameInput WebSocketManager::getInput() {
    if (commandAvailable) {
        commandAvailable = false;
        GameInput input = lastInput;
        lastInput.command = CMD_NONE;
        lastInput.player = 0;
        return input;
    }
    GameInput emptyInput = {CMD_NONE, 0};
    return emptyInput;
}

bool WebSocketManager::hasCommand() {
    return commandAvailable;
}

void WebSocketManager::sendMessage(String message) {
    ws.textAll(message);
}

void WebSocketManager::sendScore(int score) {
    String msg = "SCORE:" + String(score);
    ws.textAll(msg);
}

void WebSocketManager::sendGameState(String state) {
    String msg = "STATE:" + state;
    ws.textAll(msg);
}