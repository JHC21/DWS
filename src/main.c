#include <stdio.h> /* for printf() */
#include <time.h> /* for time_t, time() and ctime() */
#include <string.h> /* for memset() */
#include <stdlib.h> /* for malloc() */
#include "dws.h" /* define mecro */


void display();

char status,isSet;
bool isAlarm;
bool alarmIndicator;

struct tm *currentTime, *alarmTime;
double measureTime, lapTime;

char* days[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};  

int main(int argc, const char * argv[]) {

    char input[BUFSIZE];
    isAlarm = false;
    alarmIndicator = false;
    bool isOff = true;

    clock_t sw_start, sw_end;
    clock_t timer_start, timer_end;
    clock_t light_start, light_end;

    /* Initialize mode */
    status = TKMODE;

    /* Initialize currentTime to 2019.01.01 00:00 */
    currentTime = malloc(sizeof(struct tm));
    currentTime->tm_year = 2019 - 1900;
    currentTime->tm_mon = 0; // start from 0
    currentTime->tm_mday = 1;
    currentTime->tm_hour = 0;
    currentTime->tm_min = 0;
    currentTime->tm_sec = 0;
    currentTime->tm_isdst = 0; // no summer time

    /* Initialize alarmTime to 12:00 */
    alarmTime = malloc(sizeof(struct tm));
    alarmTime->tm_hour = 12;
    alarmTime->tm_min = 0;
    alarmTime->tm_mday = '\0';

    /* Initialize Stopwatch Time */
    measureTime = 0;
    lapTime = 0;
    timer_start = clock();

    /* start DWS */
    while(true) {
       alarmCheck(isAlarm,currentTime, alarmTime);
       if(!isAlarm) {
          if(alarmIndicator){ /* if alarm indicator is On, check Alarm time */
             isAlarm = alarmCheck(isAlarm,currentTime, alarmTime);
          }
          memset(input,0,BUFSIZE);
          do {
	      timer_end = clock();
	      if (kbhit() != 0) {
      	          scanf("%s", input);
		  printf("*");
              }
          }while( ((double) (timer_end-timer_start) /CLOCKS_PER_SEC) < 1);
          timer_start = clock();
	  currentTime->tm_sec++;
  
          determinePriority(input);

          if(input[0]==BUTTOND){
	     light_start = clock();
	     isOff = false;
          }else{
             if(!isOff){
	        light_end = clock();

		if((double) (light_end - light_start) / CLOCKS_PER_SEC > 4) {
		   isOff = true;
		}	
	     }
	     backlightCheck(isOff);

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
                        } else if (input[0] == BUTTONB){
                           alarmIndicator = !alarmIndicator;
                        }

			display();
                        break;

                    case STALRTIME: /* set alarm time */
                        if (isSet == status) {
                            isSet = '\0';
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
			   currentTime->tm_sec++;
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
			   if(isSet == STCURTIME) currentTime->tm_hour++;
			   else if (isSet == STALRTIME) alarmTime->tm_hour++;
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
			   if(isSet == STCURTIME) currentTime->tm_min++;
			   else if (isSet == STALRTIME) alarmTime->tm_min++;
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
			   currentTime->tm_mday++;
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
                        } else if(input[0] == BUTTONB) { /* increase month */
			   currentTime->tm_mon++;
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
                        } else if(input[0] == BUTTONB) { /* increase year */
			   currentTime->tm_year++;
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
			    lapTime = 0;

                        } else if (input[0] == BUTTONB){
                            status = MSSWTIME;
                            sw_start = clock();
                        } else if (input[0] == BUTTONC){
                            status = TKMODE;
                        }

			display();
                        break;

                    case MSSWTIME: /* measure stopwatch time */
                        if (input[0] == BUTTONA){ /* measure laptime */
                           status = MSLPTIME;

                           sw_end = clock();
                           lapTime = measureTime + (double)(sw_end-sw_start)/(CLOCKS_PER_SEC/1000.0);

			   display();
                        } else if (input[0] == BUTTONB){ /* add up measure time */
                            status = SWMODE;

                            sw_end = clock();
                            measureTime += (double) (sw_end-sw_start)/(CLOCKS_PER_SEC/1000.0);

			   display();
                        }
                        break;

                    case MSLPTIME: /* measure laptime */
                        if (input[0] == BUTTONA) {
			   sw_end = clock();
			   lapTime = measureTime + (double)(sw_end-sw_start)/(CLOCKS_PER_SEC/1000.0);

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
    }
    free(currentTime);
    free(alarmTime);
    return 0;
}


void display() {
    system("clear");
    printf("A                   B\n");

    switch((status/10)*10){
        case TKMODE:
            if(!(status%10)){
                printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                printf("%c \t%02d : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
            }else{
                switch(status){
                    case STSEC:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d. %d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        printf("           __\n");
                        break;
                    case STHOU:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d. %d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        printf("   __\n");
                        break;
                    case STMIN:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d. %d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        printf("        __\n");
                        break;
                    case STYEA:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d. %d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        printf("%d\n", currentTime->tm_year);
                        break;
                    case STMON:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("   __\n");
                        printf("%c %d : %2d. %2d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        break;
                    case STDAY:
                        printf("\t%s %d %d\n", days[currentTime->tm_wday], (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("      __\n");
                        printf("%c %d : %d. %d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
                        break;
                }
            }
            break;

        case SWMODE:
            if(!(status%10)){
                printf("ST %d %d\n", currentTime->tm_hour, currentTime->tm_min);
                printf("%f' %f''%f\n", measureTime/6000, measureTime/100, measureTime-(measureTime/100));
            }else{
                switch(status){
                    case MSSWTIME:
                        printf("ST %d %d\n", currentTime->tm_hour, currentTime->tm_min);
                        printf("%f' %f''%f\n", measureTime/6000, measureTime/100, measureTime-(measureTime/100));
                        break;
                    case MSLPTIME:
                        printf("ST %d %d\n", currentTime->tm_hour, currentTime->tm_min);
                        printf("%f' %f''%f\n", lapTime/6000, lapTime/100, lapTime-(lapTime/100));
                        break;
                    case RSSWTIME:
                        printf("ST %d %d\n", currentTime->tm_hour, currentTime->tm_min);
                        printf("%f' %f''%f\n", measureTime/6000, measureTime/100, measureTime-(measureTime/100));
                        break;
                }
	    }
            break;

        case ALMODE:
            if(!(status%10)){
                printf("AL %d %d\n", (currentTime->tm_mon)+1, currentTime->tm_mday);
                printf("%c %d : %d\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
            } else{
                switch(status){
                    case STHOU:
                        printf("AL %d %d\n", (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
                        printf("   __\n");
                        break;
                    case STMIN:
                        printf("AL %d %d\n", (currentTime->tm_mon)+1, currentTime->tm_mday);
                        printf("%c %d : %d\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
                        printf("        __\n");
                        break;
		}

            }
            break;

    }
    printf("C                   D\n");
    return;
}
