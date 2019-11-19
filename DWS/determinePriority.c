#include "dws.h"

void determinePriority(char *input){
    char temp = input[0]+16;
    for(int i=1; i<BUFSIZE; i++){
        input[i]+=16;
        if(input[i] > temp){
            temp = input[i];
        }
    }
    input[0] = temp;
    return;
}
