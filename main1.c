#include <stdio.h> /* for printf() */
#include <time.h> /* for time_t, time() and ctime() */
#include <string.h> /* for memset() */
#include <conio.h> /* for kbhit() */
#include "dws.h" /* define */

void display();

int main(int argc, const char * argv[]) {
    
    char input[BUFSIZE];
    char status;
    bool alarmIndicator = false;
    bool isAlarm = false;
    
    time_t currentTime, alarmTime;
    double measureTime, lapTime;
    clock_t start, end;
    clock_t timer_start, timer_end;

    /* Initialize */
    status = TKMODE;
    time(&currentTime);
    measureTime = 0;
    lapTime = 0;

    /* start DWS */
    while(true) { 
	timer_start = clock(NULL);

        while(isAlarm){ /* if alarm is on */
		fprintf(stdout, "\aBeep!\n" );
	    
		if (kbhit() != 0) {
			isAlarm = !isAlarm; /* alarm OFF */
		}
        }else{
            if(alarmIndicator){ /* if alarm indicator is On, check Alarm time */
                isAlarm = alarmCheck(currentTime, alarmTime);
            }
            memset(input,0,BUFSIZE);
            scanf("%s", input);
            determinePriority(input);
            
            if(input[0]==BUTTOND){
                backlightCheck();
            }else{
                switch( status ){
                    case TKMODE:
			if (input[0] == BUTTONC){
				status = ALMODE;
			}
                        break;

                    case STCURTIME: /* set current time */

			break;


                    case ALMODE:
			if (input[0] == BUTTONC){
				status = SWMODE;
			}
                        break;

                    case STALRTIME: /* set alarm time */

			break;

                        
                    case SWMODE:
                       if (input[0] == BUTTONA){
				status = RSSWTIME;
				measureTime = 0;

			} else if (input[0] == BUTTONB){
				status = MSSWTIME;
				start = clock(NULL);
			} else if (input[0] == BUTTONC){
				status = TKMODE;
			}
                        break;

                    case MSSWTIME: /* measure stopwatch time */
			if (input[0] == BUTTONA){ /* measure laptime */
				status = MSLPTIME;

				end = clock(NULL);
				lapTime = (double)(end-start)/CLOCKS_PER_SEC;

			} else if (input[0] == BUTTONB){ /* add up measure time */
				status = SWMODE;

				end = clock(NULL);
				measureTime += (double) (end-start)/CLOCKS_PER_SEC;
			}
			break;

                    case MSLPTIME: /* measure laptime */
			status = MSSWTIME;
			break;

                    case RSSWTIME: /* reset stopwatch time */
			status = SWMODE;
			break;
                        
                    default:
                        printf("Invalid state\n");
			return -1;
                }
		display();
            }
        }
    }
    
    return 0;
}
