#include "dws.h"

void determinePriority(char *input){
	char temp = input[0];
	for(int i=1; i<BUFSIZE; i++){
		if(input[i] > temp){
			temp = input[i];
		}
	}
	input[0] = temp;
	return;
}
