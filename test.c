#include <stdio.h>
#include <stdlib.h>

int kbhit(void);

int main(){
char* input;
while(1){
    if (kbhit() != 0) {
             input = getchar();
    }
printf("%s\n\n",input);
}
    return 0;
}
