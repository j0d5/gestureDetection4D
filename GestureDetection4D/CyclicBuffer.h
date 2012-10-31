//
//  CyclicBuffer.h
//  CyclicBuffer
//
//  Created by Marcel Vielsack on 24.10.12.
//  Copyright (c) 2012 Marcel Vielsack. All rights reserved.
//

#ifndef __CyclicBuffer__CyclicBuffer__
#define __CyclicBuffer__CyclicBuffer__

#include <iostream>

template <class T>
class CyclicBuffer {
    
private:
    
    T* startInMemoryElement; // acctual start in memory
    T* endInMemoryElement;   // acctual end in memory
    T* firstElement; // first valid element
    T* lastElement;  // last valid element
    T* iterator;
    bool isBufferFull = false; // is buffer completly full
    
public:
    
    //constructor
    CyclicBuffer(int size) {
        startInMemoryElement = (T*)(malloc(size * sizeof(T)));
        firstElement = startInMemoryElement;
        lastElement = firstElement; //buffer is empty
        endInMemoryElement = startInMemoryElement + size-1;
        iterator = startInMemoryElement;
    }
    
    //destructor
    ~CyclicBuffer(void) {
    	free(startInMemoryElement);
    }
    
    //adds an new element to the buffer. If the buffer is full, the oldest will be replaced 
    void push(T newElement) {
        if(lastElement == endInMemoryElement) {
            isBufferFull = true;
            *lastElement = newElement;
            lastElement = startInMemoryElement;

        } else {
            *lastElement++ = newElement;
        }
        
        if(firstElement == endInMemoryElement) { //start over if end in memory is reached
            firstElement = startInMemoryElement;
        } else if(isBufferFull) {
            ++firstElement;
        }
    }
    
	//checks if the buffer is empty
    bool isEmpty(){
        return firstElement == lastElement;
    }
    
    
    //checks if the buffer is full
    bool isFull() {
        return isBufferFull;
    }
    
    
    //gets the next element in the buffer
    T* next() {
        if(iterator == endInMemoryElement) {
            iterator = startInMemoryElement;
            return endInMemoryElement;
        }
        
        return iterator++;
        
    }
    
    //resets the Iterator to the first element of the buffer
    void resetIterator() {
        iterator = firstElement;
    }
    
};

#endif /* defined(__CyclicBuffer__CyclicBuffer__) */
