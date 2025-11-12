#include "WebSocketManager.h"

// Inizializza il puntatore statico
WebSocketManager* WebSocketManager::instance = nullptr;

WebSocketManager::WebSocketManager() : ws("/ws"), lastCommand(CMD_NONE), commandAvailable(false) {
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
        client->text("Connected to ESP32 Snake Game");
        
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
    
    // Mappa i messaggi ai comandi
    if (message == "START" || message == "START_SNAKE") {
        lastCommand = CMD_START_SNAKE;
        commandAvailable = true;
    } 
    else if (message == "STOP" || message == "STOP_GAME") {
        lastCommand = CMD_STOP_GAME;
        commandAvailable = true;
    }
    else if (message == "UP" || message == "W") {
        lastCommand = CMD_UP;
        commandAvailable = true;
    }
    else if (message == "DOWN" || message == "S") {
        lastCommand = CMD_DOWN;
        commandAvailable = true;
    }
    else if (message == "LEFT" || message == "A") {
        lastCommand = CMD_LEFT;
        commandAvailable = true;
    }
    else if (message == "RIGHT" || message == "D") {
        lastCommand = CMD_RIGHT;
        commandAvailable = true;
    }
    else if (message == "PAUSE") {
        lastCommand = CMD_PAUSE;
        commandAvailable = true;
    }
}

GameCommand WebSocketManager::getCommand() {
    if (commandAvailable) {
        commandAvailable = false;
        GameCommand cmd = lastCommand;
        lastCommand = CMD_NONE;
        return cmd;
    }
    return CMD_NONE;
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