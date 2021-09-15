#ifndef CPP_CHAT_CLIENT_CLIENT_H
#define CPP_CHAT_CLIENT_CLIENT_H

#include <thread>
#include <string.h>
#include <pthread.h>
#include "vusocket.h"
#include "CircularLineBuffer.h"

class Client {
private:
  
    int WhoC = -1;
   
    std::thread socketThread, stdinThread;

    SOCKET sock;

   
    CircularLineBuffer socketBuffer, stdinBuffer;
    /**
     *
     * This method reads data from the standard input and writes it into the 'stdinBuffer'.
     * This method is called repeatedly, which means you don't need to add a loop yourself.
     * After implementation, it should look something like this:
     *
     * @return Returns -1 if the user writes '!exit', returns the number of characters read otherwise.
     */
    int readFromStdin();

    /**
     *
     * This method reads data from 'sock' and writes it into 'socketBuffer'.
     * This method is called repeatedly, which means you don't need to add a loop yourself.
     * After implementation, it should look something like this:
     *
     * #return Return the return value of 'recv'.
     */
    int readFromSocket();

    inline void threadReadFromStdin() {
        while (!stop) {
            auto res = readFromStdin();
            if (res < 0) {
                stop = true;
            }
        }
    }

    inline void threadReadFromSocket() {
        while (!stop) {
            auto res = readFromSocket();
            if (res < 0) {
                stop = true;
            }
        }
    }

    void createSocketAndLogIn();

 
    void closeSocket();

    inline void startThreads() {
        socketThread = std::thread(&Client::threadReadFromSocket, this);
        stdinThread = std::thread(&Client::threadReadFromStdin, this);
    }

    inline void stopThreads() {
        socketThread.join();
        stdinThread.join();
    }

public:

    bool stop = false;


    inline ~Client() {
        closeSocket();
        stopThreads();
    }

    inline void setup() {
        createSocketAndLogIn();
        startThreads();
    }
    struct addrinfo *result;

    int tick();
};


#endif //CPP_CHAT_CLIENT_CLIENT_H