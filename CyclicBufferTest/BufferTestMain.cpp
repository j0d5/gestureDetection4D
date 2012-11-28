//
//  main.cpp
//  CyclicBuffer
//
//  Created by Marcel Vielsack on 24.10.12.
//  Copyright (c) 2012 Marcel Vielsack. All rights reserved.
//

#include <iostream>
#include "../GestureDetection4D/CyclicBuffer.h"


int main(int argc, const char * argv[])
{
    
    CyclicBuffer<int>* testBuffer = new CyclicBuffer<int>(25);
    
    testBuffer->isFull();
    
    for(int i = 0; i < 120;i++) {
        testBuffer->push(i);
        
        if(testBuffer->isFull()) {
        
            if(i == 42) {
                testBuffer->flush();
            }
        
            testBuffer->resetIterator();
            for(int j =0; j < 25; j++) {
                printf("inhalt an der stelle %d : %d \n", j, *(testBuffer->next()));
            }
            printf("\n");
        }
    }

    return 0;
}

