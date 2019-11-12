#include <stdio.h> /* for printf() */
#include <time.h> /* for time_t, time() and ctime() */
#include <string.h> /* for memset() */
#include <conio.h> /* for kbhit() */
#include "dws.h" /* define mecro */

void display();

int main(int argc, const char * argv[]) {
    
    char input[BUFSIZE];
    char status,isSet;
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
        }
	if(!isAlarm) {
            if(alarmIndicator){ /* if alarm indicator is On, check Alarm time */
                isAlarm = alarmCheck(currentTime, alarmTime);
            }
            memset(input,0,BUFSIZE);
	    do {
		if (kbhit() != 0) {
			scanf("%s", input);
		}
		timer_end = clock(NULL);
	    }while( ((timer_end-timer_start)/CLOCKS_PER_SEC) < 1 ); /* for 1 sec */

	    if (input[0] == '\0') continue; /* no input, so go back to alarm check */
            
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
			if (isSet == status) {
				isSet = '\0'
				status = TKMODE;
			} else  {
				isSet = status; /* mark as timekeeping mode */
			        status = STSEC;
			}
			break;


                    case ALMODE:
			if (input[0] == BUTTONC){
				status = SWMODE;
			}
                        break;

                    case STALRTIME: /* set alarm time */
			if (isSet == status) {
				isSet = '\0'
				status = ALMODE; 
			} else  {
				isSet = status; /* mark as alarm mode */
			        status = STHOU;
			}
			break;
				
				
		    /* time setting process */
                    case STSEC: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase second */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit second > hour */
				status = STHOU;
			}
			
			break;
				
                    case STHOU: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase hour */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit hour > minutes */
				status = STMIN;
			}
			break;
                        
                    case STMIN: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase minutes */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit hour > minute // hour > day */
				if (isSet == STCURTIME) {
					status = STDAY;
				} else if (isSet == STALRTIME) {
					status = STMIN;
				}
			}
			break;
				
                    case STDAY: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase day */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit day > month */
				status = STMON;
			}
			break;
				
	            case STMON: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase hour */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit month > year */
				status = STYEA;
			}
			break;
				
                    case STYEA: 
			if (input[0] == BUTTONA ) { /* return to display mode */
				status = isSet;
			} else if(input[0] == BUTTONB) { /* increase hour */
				
				
			} else if(input[0] == BUTTONC ) { /* change digit year > second */
				status = STSEC;
			}
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
