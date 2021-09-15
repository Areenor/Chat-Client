#include "CircularLineBuffer.h"
#include <string.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * This method writes the given number of characters into the buffer,
 * starting at the next free location.
 * If there is not enough space left in the buffer, it writes nothing into the buffer and returns false.
 *
 * @param chars Pointer to the characters to write into the buffer.
 * @param nchars The number of characters to write.
 * @return False if there was not enough space in the buffer. True otherwise.
 */
bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars)   {


    if(nchars > freeSpace())    { //checks if there is enough space left
        return false;
    }

    if(isFull() == false) { //checks if the buffer is full
        string newn = chars;
        int counter = 0;
        int i = nextFreeIndex(); //starting point in the buffer

        if((i+nchars)<= (bufferSize - 1)) { //Method#1 when writing doesnt need to loop back around
            /*while (newn[counter] != '\n') { //writes string characters into the buffer till found \n
                buffer[i] = newn.at(counter);
                counter++;
                count++;
                i++;
            }

            buffer[i] = newn.at(counter); //adds in \n in the buffer

            count++;*/
            int loop = nchars + i;
            for(int o=i; o<loop; o++)    {
                buffer[o] = newn.at(counter);
                counter++;
                count++;
            }

            return true;
        }
        if((i+nchars)> (bufferSize - 1)) { //method#2 when writing does need to loop around

            int loop = nchars + i - bufferSize;
            int cont = 0; //start buffer
            for(int k=i; k<bufferSize;k++)    { //loops and writes characters till end of the buffer
                buffer[k] = newn.at(counter);
                counter++;
                count++;

            }
            for(int o=cont; o<loop; o++)    {
                buffer[o] = newn.at(counter);
                counter++;
                count++;
            }

            /*
            while (newn[counter] != '\n') { //start at index 0 again, continues till found \n
                buffer[cont] = newn.at(counter);
                counter++;
                count++;
                cont++;
            }

            buffer[cont] = newn.at(counter); //adds in \n character in the buffer

            count++;*/


            return true;
        }
    }
    else   {
        return false;
    }


}

/**
 * This method reads a line from the buffer,
 * starting from location 'start'.
 * If there is no complete line (no '\n') in the buffer, this method returns an empty string.
 *
 * @return The next string from the buffer. Returns an empty string if the buffer is empty.
 */
string CircularLineBuffer::_readLine() {

    int i = start;


    int countr = 1;

    int stuff = 0;

    int stuff2 = 0;

    char stor[bufferSize];
    if(!hasLine()){
        string Zero;
        return Zero;
    }
    for (int j = 0; j < bufferSize; j++) {
        stor[j] = 5;
    }

    int line = findNewline();
    string returnstuff = stor;

    if (start < findNewline())    {
        while (buffer[i] != '\n') {
            returnstuff.at(stuff) = buffer[i];
            count--;
            countr++;
            i++;
            stuff++;

        }

        count--;


        int ender = returnstuff.length();


        returnstuff.erase(stuff, ender);
        start = start + countr;

        if(start==bufferSize)    {
            start=0;
        }

        return returnstuff;
    }
    else    {

        countr=0;
        int cont = 0; //start buffer
        for(i; i<bufferSize;i++)    { //loops and writes characters till end of the buffer
            returnstuff.at(stuff) = buffer[i];
            stuff++;
            count--;
            countr++;

        }
        while (buffer[cont] != '\n') {
            returnstuff.at(stuff) = buffer[cont];
            count--;
            countr++;
            cont++;
            stuff++;

        }


        count--;


        int ender = returnstuff.length();


        returnstuff.erase(stuff, ender);


        start = (start+ countr) - (bufferSize - 1);

        return returnstuff;

    }
}

/**
     * @return The amount of free space in the buffer in number of characters.
     */
int CircularLineBuffer::freeSpace()   {

    return bufferSize - count;
}

/**
 *
 * @return true if and only if (iff) the buffer is full.
 */
bool CircularLineBuffer::isFull()   {

    return bufferSize==count;
}

/**
 *
 * @return true if and only if (iff) the buffer is empty.
 */
bool CircularLineBuffer::isEmpty()   {

    return count==0;
}

/**
 * This method should return the next free spot in the buffer as seen from the current value of 'start'.
 * 
 * If the buffer is full, the behavior is undefined.
 *
 * @return The index of the first free position in the buffer.
 */
int CircularLineBuffer::nextFreeIndex()    {

    if(!isFull()) { //checks if the buffer is not full
        if (start + count < bufferSize - 1) { //if doesnt loop around to find free index
            return start + count;
        }
        else { //if does loop around to find free index
            return (start + count) - bufferSize;
        }
    }
    else    {
        return -99; //undefined
    }
}

/**
 * The position of the next newline character (\n), as seen from the current value of 'start'.
 *
 * @return The position of the next newline character (\n), as seen from the current value of 'start'.
 */
int CircularLineBuffer::findNewline()    {
    int NewLineIndex = 0;
    bool Noccur = false; // to know if it is needed to check from beginning too
    for(int u = start; u < bufferSize;u++){ //checking from start till end of buffer
        if(buffer[u] != '\n') {
            NewLineIndex++;
        }
        else{
            Noccur = true; //if newline occur in this part of buffer
            break; //exiting the loop
        }
    }
    if(!Noccur){ //if newline did not occur
        NewLineIndex = 0;
        for(int k = 0; k < start;k++){  //checks for newline from beginning to start - 1
            if(buffer[k] != '\n') {
                NewLineIndex++;
            }
            else{
                break;
            }
        }
    }

    if(Noccur){ //if new line is before end of the buffer, add start index number
        NewLineIndex =  NewLineIndex+start;
    }

    return NewLineIndex; //returning index

}

/**
 * Checks if there is a complete line in the buffer.
 * You can make your life easier by implementing this method using the method above.
 *
 * @return true if and only if (iff) there is at least one complete line in the buffer.
 */
bool CircularLineBuffer::hasLine()   {
    int control = 0;
    bool line = false;

    if(start + count <= bufferSize) {
        for (int j = start; j < start + count; j++) {
            if (buffer[j] == '\n') {
                line = true;
            }
        }
    }
    else {
        for (int i = start; i < bufferSize; i++) {
            control++;
            if (buffer[i] == '\n') {
                line = true;
            }
        }
        int loop = count - control;
        for (int k = 0; k < loop; k++) {
            if (buffer[k] == '\n') {
                line = true;
            }
        }
    }
    return line;
}