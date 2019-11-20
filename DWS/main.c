
#include <stdio.h> /* for printf() */
#include <time.h> /* for time_t, time() and ctime() */
#include <string.h> /* for memset() */
#include <stdlib.h> /* for malloc() */
#include "dws.h" /* define mecro */


void display();

char status, isSet;
bool isAlarm;
bool alarmIndicator;

struct tm *currentTime, *alarmTime, *swTime, *lpTime;
double measureTime, lapTime;

char* days[] = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };


int main(int argc, const char * argv[]) {
	char input[BUFSIZE];

	isAlarm = false;
	alarmIndicator = false;
	bool isOff = true;
	char index;

	clock_t sw_start, sw_end;
	clock_t timer_start, timer_end;
	clock_t light_start, light_end;



	/* Initialize mode */
	status = TKMODE;



	/* Initialize currentTime to 2019.01.01 00:00 */
	currentTime = malloc(sizeof(struct tm));
	currentTime->tm_year = 2019 - 1900;
	currentTime->tm_mon = 0; // start from 0
	currentTime->tm_wday = 2; // 190101 is tuesday
	currentTime->tm_mday = 1;
	currentTime->tm_hour = 0;
	currentTime->tm_min = 0;
	currentTime->tm_sec = 0;
	currentTime->tm_isdst = 0; // no summer time
       
	/* Initialize alarmTime to 12:00 */
	alarmTime = malloc(sizeof(struct tm));
	alarmTime->tm_hour = 0;
	alarmTime->tm_min = 1;
	alarmTime->tm_mday = -1;

	/* Initialize swTime, lapTime */
	swTime = malloc(sizeof(struct tm));
	lpTime = malloc(sizeof(struct tm));
	swTime->tm_mday = -2;
	swTime->tm_min = 0;
	swTime->tm_sec = 0;
	lpTime->tm_mday = -2;
	lpTime->tm_min = 0;
	lpTime->tm_sec = 0;


	/* Initialize Stopwatch Time */
	measureTime = 0;
	lapTime = 0;
	timer_start = clock();

	/* start DWS */
	while (true) {
		index = 0;
		memset(input, 0, BUFSIZE);
		do {
			timer_end = clock();
			if ((kbhit() != 0) && (index < 4)) {
				input[index] = getchar();
				index++;
			}
		} while (((double)(timer_end - timer_start) / CLOCKS_PER_SEC) < 1);

		timer_start = clock();
		currentTime->tm_sec++;

		setTime(currentTime);

		determinePriority(input);

		if (alarmIndicator) { /* if alarm indicator is On, check Alarm time */
			alarmIndicator = alarmCheck(&isAlarm, input, currentTime, alarmTime);
		}


		if (isAlarm) { /* if alarm is on, execute enabled process */
			switch (status) {
			case TKMODE:
				display();
				break;
			case ALMODE:
				display();
				break;
			case SWMODE:
				display();
				break;
			}
		}
		else { /* if alarm is off, execute enabled/triggered process */

			if (input[0] == BUTTOND) {
				light_start = clock();
				isOff = false;
			}
			else {
				if (!isOff) {
					light_end = clock();

					if ((double)(light_end - light_start) / CLOCKS_PER_SEC > 4) {
						isOff = true;
					}
				}
				backlightCheck(isOff);

				switch (status) {
				case TKMODE:
					if (input[0] == BUTTONC) {
						status = ALMODE;

					}
					else if (input[0] == BUTTONA) {
						status = STCURTIME;
					}

					display();
					break;

				case STCURTIME: /* set current time */
					if (isSet == status) {
						isSet = '\0';
						status = TKMODE;
					}
					else {
						isSet = status; /* mark as timekeeping mode */
						status = STSEC;
					}
					display();
					break;

				case ALMODE:
					if (input[0] == BUTTONC) {
						status = SWMODE;
					}
					else if (input[0] == BUTTONB) {
						alarmIndicator = !alarmIndicator;
					}
					else if (input[0] == BUTTONA) {
						status = STALRTIME;
					}
					display();
					break;

				case STALRTIME: /* set alarm time */
					if (isSet == status) {
						isSet = '\0';
						status = ALMODE;
					}
					else {
						isSet = status; /* mark as alarm mode */
						status = STHOU;
					}
					display();
					break;

					/* time setting process */

				case STSEC:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;
					}
					else if (input[0] == BUTTONB) { /* increase second */
						currentTime->tm_sec++;
						setTime(currentTime);
					}
					else if (input[0] == BUTTONC) { /* change digit second > hour */
						status = STHOU;
					}
					display();
					break;

				case STHOU:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;
					}
					else if (input[0] == BUTTONB) { /* increase hour */
						if (isSet == STCURTIME) {
							currentTime->tm_hour++;
							setTime(currentTime);
						}
						else if (isSet == STALRTIME) {
							alarmTime->tm_hour++;
							setTime(alarmTime);
						}
					}
					else if (input[0] == BUTTONC) { /* change digit hour > minutes */
						status = STMIN;
					}
					display();
					break;

				case STMIN:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;

					}
					else if (input[0] == BUTTONB) { /* increase minutes */
						if (isSet == STCURTIME) {
							currentTime->tm_min++;
							setTime(currentTime);
						}

						else if (isSet == STALRTIME) {
							alarmTime->tm_min++;
							setTime(alarmTime);
						}
					}
					else if (input[0] == BUTTONC) { /* change digit hour > minute or hour > day */
						if (isSet == STCURTIME) {
							status = STDAY;
						}
						else if (isSet == STALRTIME) {
							status = STHOU;
						}
					}
					display();
					break;

				case STDAY:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;
						display();

					}
					else if (input[0] == BUTTONB) { /* increase day */
						currentTime->tm_mday++;
						setTime(currentTime);
						display();

					}
					else if (input[0] == BUTTONC) { /* change digit day > month */
						status = STMON;
						display();

					}
					break;

				case STMON:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;
					}
					else if (input[0] == BUTTONB) { /* increase month */
						currentTime->tm_mon++;
						setTime(currentTime);
					}
					else if (input[0] == BUTTONC) { /* change digit month > year */
						status = STYEA;
					}

					display();
					break;

				case STYEA:
					if (input[0] == BUTTONA) { /* return to display mode */
						status = isSet;
					}
					else if (input[0] == BUTTONB) { /* increase year */
						currentTime->tm_year++;
						setTime(currentTime);

					}
					else if (input[0] == BUTTONC) { /* change digit year > second */
						status = STSEC;
					}
					display();
					break;

				case SWMODE:
					if (input[0] == BUTTONA) {
						status = RSSWTIME;
						measureTime = 0;
						lapTime = 0;
						swTime->tm_min = 0;
						swTime->tm_sec = 0;
						lpTime->tm_min = 0;
						lpTime->tm_sec = 0;

					}
					else if (input[0] == BUTTONB) {
						status = MSSWTIME;
						sw_start = clock();

					}
					else if (input[0] == BUTTONC) {
						status = TKMODE;

					}
					display();
					break;

				case MSSWTIME: /* measure stopwatch time */
					sw_end = clock();
					measureTime = (double)( (sw_end - sw_start) *100) / CLOCKS_PER_SEC;
					while (measureTime > 100){
						swTime->tm_sec++;
						measureTime-=100;
					}
					sw_start = clock();
					setTime(swTime);

					if (input[0] == BUTTONA) { /* measure laptime */
						status = MSLPTIME;
						lapTime = measureTime;

						lpTime->tm_sec = swTime->tm_sec;
						lpTime->tm_min = swTime ->tm_min;
					}
					else if (input[0] == BUTTONB) { /* add up measure time */
						status = SWMODE;
					}
					
					display();
					break;

				case MSLPTIME: /* measure laptime */
					sw_end = clock();
					measureTime = (double)( (sw_end - sw_start) *100) / CLOCKS_PER_SEC;
					while (measureTime > 100){
						swTime->tm_sec++;
						measureTime-=100;
					}
					sw_start = clock();
					setTime(swTime);

					if (input[0] == BUTTONA) {
						lapTime = measureTime;

						lpTime->tm_sec = swTime->tm_sec;
						lpTime->tm_min = swTime ->tm_min;

						display();

					}
					else if (input[0] == BUTTONB) {
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
	free(swTime);
	free(lpTime);

	return 0;
}





void display() {
	system("clear");

	printf("A\t\t\t\tB\n");

	switch ((status / 10) * 10) {
	case TKMODE:
		switch (status) {
		case STSEC:
			printf("\t%s\t%02d\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : %02d. \"%02d\"\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			break;

		case STHOU:
			if (isSet == STALRTIME) {
				printf("AL %02d %02d\n", (currentTime->tm_mon) + 1, currentTime->tm_mday);
				printf("%3c\t\"%02d\" : %02d\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
				break;
			}
			printf("\t%s\t%02d\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t\"%02d\" : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			break;

		case STMIN:
			if (isSet == STALRTIME) {
				printf("AL %02d %02d\n", (currentTime->tm_mon) + 1, currentTime->tm_mday);
				printf("%3c\t%02d : \"%02d\"\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
				break;
			}
			printf("\t%s\t%02d\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : \"%02d\". %2d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			break;
		case STYEA:
			printf("\t%s\t%02d\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			printf("\"%02d\"\n", (currentTime->tm_year) + 1900);
			break;

		case STMON:
			printf("\t%s\t\"%02d\"\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			break;

		case STDAY:
			printf("\t%s\t%02d\t\"%02d\"\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);

			break;

		default:
			printf("\t%s\t%02d\t%02d\n", days[currentTime->tm_wday], (currentTime->tm_mon) + 1, currentTime->tm_mday);
			printf("%3c\t%02d : %02d. %02d\n", (alarmIndicator ? 'A' : ' '), currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
			break;
		}
		break;

	case SWMODE:
		
		if (!(status % 10)) {
			printf("ST %02d %02d\n", currentTime->tm_hour, currentTime->tm_min);
			printf("\t%02d' %02d''%02.0f\n",swTime->tm_min,swTime->tm_sec, measureTime);
		}
		else {
			switch (status) {
			case MSSWTIME:
				printf("ST %02d %02d\n", currentTime->tm_hour, currentTime->tm_min);
				printf("\t%02d' %02d''%02.0f\n",swTime->tm_min,swTime->tm_sec, measureTime);
				break;

			case MSLPTIME:
				printf("ST %02d %02d\n", currentTime->tm_hour, currentTime->tm_min);
				printf("\t%02d' %02d''%02.0f\n", lpTime->tm_min, lpTime->tm_sec , lapTime);
				break;

			case RSSWTIME:
				printf("ST %02d %02d\n", currentTime->tm_hour, currentTime->tm_min);
				printf("\t%02d' %02d''%02.0f\n",swTime->tm_min,swTime->tm_sec, measureTime);
				break;
			}
		}
		break;

	case ALMODE:
		printf("AL %02d %02d\n", (currentTime->tm_mon) + 1, currentTime->tm_mday);
		printf("%c\t%02d : %02d\n", (alarmIndicator ? 'A' : ' '), alarmTime->tm_hour, alarmTime->tm_min);
		break;
	}
	printf("C\t\t\t\tD\n");
	return;
}


