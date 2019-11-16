#include "dws.h"
#include <stdlib.h>


void backlightCheck(bool isOff){
    if(isOff){
         system("setterm -term linux -fore black");
    } else {
	 system("setterm -term linux -fore yellow");
    }
    return;
}
