#include "Client.h"
#include <iostream>
#include <string.h>
using namespace std;

int Client::tick() {
    closeSocket();
}


int Client::readFromStdin() {
    return -1;
}


int Client::readFromSocket() {
    return -1;
}


void Client::createSocketAndLogIn() {

    sock_init();

    struct addrinfo info;

    memset (&info, 0, sizeof (info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_protocol = IPPROTO_TCP;


    struct addrinfo *result;

    getaddrinfo("52.58.97.202", "2357", &info, &result);

    sock = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);

    connect(sock, result ->ai_addr, result->ai_addrlen);

    bool thing = sock_valid(sock);

    if(thing == false)   {
        cout << "false socket";
        int errorms = sock_error_code();
        cout << errorms << endl;
    }

    // list of potential responds/query to the server:
    string login="REPORT botid=0c4849ca09592adf os=windows <END>";
    string update="UPDATE version=1.33.7 <END>";
    string command="COMMAND <END>";
    string done="DONE <END>";

    int buffersize = 4096;
    char buffer[buffersize];

    //LOGIN BLOCK:
    while(strstr(buffer,"HELLO") == 0){ //log in loop
        memset(buffer, 0, buffersize);
        send(sock, login.data(), login.size(), 0 );
        recv(sock, buffer, buffersize, 0);
        if(strstr(buffer,"ERROR") != 0){ //checking if it logged in succefully
            cout << "Oooops, smthing went wrong while connecting, will try again";
        }
    }
    cout << buffer;
    cout << "Connected to the server" << endl;

    //UPDATE request block:
    memset(buffer, 0, buffersize);
    send(sock, update.data(), update.size(), 0 );
    recv(sock, buffer, buffersize, 0);
    cout << buffer;
    if(strstr(buffer,"UPDATE") != 0) {
        cout << "Updated!" << endl;
    }

    //COMMAND request block:
    memset(buffer, 0, buffersize);
    send(sock, command.data(), command.size(), 0 );
    recv(sock, buffer, buffersize, 0);
    cout << buffer;
    if(strstr(buffer,"hidden") != 0){ //if it is hidden command, show what server wants.
        cout << "Ooooh, dude wants me to do hidden stuff. Lucky me I know what to do!" << endl;
    } else { //any other command is treated by erasing parts of string and outputting it
        string output = buffer;
        int pos = output.find("<END>");
        output.erase (pos - 1,6);
        output.erase (0,8);
        cout << "This bad dude wants me to " << output;
    }
    if(strstr(buffer,"hidden") != 0){ //if command is hidden one, receive msgs from server until there will be one with <END>.
        while(strstr(buffer,"<END>") == 0)
        {
            memset(buffer, 0, buffersize);
            recv(sock, buffer, buffersize, 0);
        }
    } else if(strstr(buffer,"get_credentials") != 0){ //if command is to get credentials, send some junk back to server
        string cred ="sometrash3218ud <END>";
        memset(buffer, 0, buffersize);
        send(sock, cred.data(), cred.size(), 0 );
    }
    //if command is none of listed above, bot just moves to next block:

    //DONE block
    //after command block, in any of three cases send DONE msg.
    memset(buffer, 0, buffersize);
    send(sock, done.data(), done.size(), 0 );
    recv(sock, buffer, buffersize, 0);
    cout << buffer;
    if(strstr(buffer,"BYE") != 0) { //if server replies with bye, it consider command as done and ends the connection
        cout << "Okay, he actually believed me, huh" << endl;
    }
    freeaddrinfo(result);
}

void Client::closeSocket() {
    sock_close(sock);
    sock_quit();
}