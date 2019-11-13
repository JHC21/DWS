#include <stdio.h> /* for printf() */
#include <time.h> /* for time_t, time() and ctime() */
#include <string.h> /* for memset() */
#include <curses.h> /* for kbhit() */
#include "dws.h" /* define mecro */


void display();

int main(int argc, const char * argv[]) {

    char input[BUFSIZE];
    char status,isSet;
    bool isAlarm = false;

    time_t currentTime, alarmTime;
    double measureTime, lapTime;
    clock_t sw_start, sw_end;
    clock_t timer_start, timer_end;

    /* Initialize */
    status = TKMODE;
    time(&currentTime);
    measureTime = 0;
    lapTime = 0;

    /* start DWS */
    while(true) {

        timer_start = clock();

        alarmCheck(currentTime, alarmTime);
        if(!isAlarm) {
            if(alarmIndicator){ /* if alarm indicator is On, check Alarm time */
                isAlarm = alarmCheck(currentTime, alarmTime);
            }
            memset(input,0,BUFSIZE);
            if (kbhit() != 0) {
               scanf("%s", input);
            }
  
            determinePriority(input);

            if(input[0]==BUTTOND){
                backlightCheck();
            }else{
                switch( status ){
                    case TKMODE:
                        if (input[0] == BUTTONC){
                            status = ALMODE;
                        }

			display();
                        break;

                    case STCURTIME: /* set current time */
                        if (isSet == status) {
                            isSet = '\0';
                            status = TKMODE;
                        } else  {
                            isSet = status; /* mark as timekeeping mode */
                            status = STSEC;
                        }

			display();
                        break;


                    case ALMODE:
                        if (input[0] == BUTTONC){
                            status = SWMODE;
                        }

			display();
                        break;

                    case STALRTIME: /* set alarm time */
                        if (isSet == status) {
                            isSet = '\0'
                            status = ALMODE;
                        } else  {
                            isSet = status; /* mark as alarm mode */
                            status = STHOU;
                        }

			display();
                        break;


                        /* time setting process */
                    case STSEC:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display();
                        } else if(input[0] == BUTTONB) { /* increase second */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit second > hour */
                           status = STHOU;
			   display();   
                        }

                        break;

                    case STHOU:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display(); 
                        } else if(input[0] == BUTTONB) { /* increase hour */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit hour > minutes */
                           status = STMIN;
			   display(); 
                        }
                        break;

                    case STMIN:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display(); 
                        } else if(input[0] == BUTTONB) { /* increase minutes */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit hour > minute or hour > day */
                           if (isSet == STCURTIME) {
                              status = STDAY;
                           } else if (isSet == STALRTIME) {
                              status = STMIN;
                           }
			   display(); 
                        }
                        break;

                    case STDAY:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display();
                        } else if(input[0] == BUTTONB) { /* increase day */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit day > month */
                           status = STMON;
			   display();
                        }
                        break;

                    case STMON:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display();
                        } else if(input[0] == BUTTONB) { /* increase hour */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit month > year */
                           status = STYEA;
			   display();
                        }
                        break;

                    case STYEA:
                        if (input[0] == BUTTONA ) { /* return to display mode */
                           status = isSet;
			   display();
                        } else if(input[0] == BUTTONB) { /* increase hour */

			   display();
                        } else if(input[0] == BUTTONC ) { /* change digit year > second */
                           status = STSEC;
			   display();
                        }
                        break;

                    case SWMODE:
                        if (input[0] == BUTTONA){
                            status = RSSWTIME;
                            measureTime = 0;

                        } else if (input[0] == BUTTONB){
                            status = MSSWTIME;
                            sw_start = clock(NULL);
                        } else if (input[0] == BUTTONC){
                            status = TKMODE;
                        }

			display();
                        break;

                    case MSSWTIME: /* measure stopwatch time */
                        if (input[0] == BUTTONA){ /* measure laptime */
                           status = MSLPTIME;

                           sw_end = clock(NULL);
                           lapTime = measureTime + (double)(sw_end-sw_start)/CLOCKS_PER_SEC;

			   display();
                        } else if (input[0] == BUTTONB){ /* add up measure time */
                            status = SWMODE;

                            sw_end = clock(NULL);
                            measureTime += (double) (sw_end-sw_start)/CLOCKS_PER_SEC;

			   display();
                        }
                        break;

                    case MSLPTIME: /* measure laptime */
                        if (input[0] == BUTTONA) {
			   sw_end = clock(NULL);
			   lapTime = measureTime + (double)(sw_end-sw_start)/CLOCKS_PER_SEC;

			   display();
			} else if (input[0] == BUTTONB){
			   status = MSSWTIME;

			   display();
			}
                        break;

                    case RSSWTIME: /* reset stopwatch time */
                        status = SWMODE;
			display();
                        break;

                    default:
                        printf("Invalid state\n");
                        return -1;
                }
            }
        }
        timer_end = clock();
    }
    return 0;
}
