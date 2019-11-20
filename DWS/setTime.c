#include "dws.h"
#include <time.h>

void setTime(s_tm *t) {



    t->tm_year += 1900;

    //1900 :0year

    if(t->tm_mday==-1) { //alarm
        if (t->tm_min >= 60) {
            t->tm_min = 0;
            t->tm_hour++;
        }
        if (t->tm_hour >= 24) {
            t->tm_hour = 0;
        }
    }
    else if(t->tm_mday==-2) { //sw
        if (t->tm_sec >= 60) {
             t->tm_min = (t->tm_sec)/60;
            t->tm_sec = t->tm_sec - t->tm_min*60;

        }
    }
    else {
        if (t->tm_sec >= 60) {
            t->tm_sec = 0;
            t->tm_min++;
        }
        if (t->tm_min >= 60) {
            t->tm_min = 0;
            t->tm_hour++;
        }
        if (t->tm_hour >= 24) {
            t->tm_hour = 0;
            t->tm_mday++;
            t->tm_wday++;

        }
        moon_year(t);
        if (t->tm_mday > month_day[t->tm_mon]) {
            t->tm_wday += month_day;
            t->tm_mday = 1;
            t->tm_mon++;
        }
        if (t->tm_mon > 12) {
            t->tm_mon = 1;
            t->tm_year++;
        }
        if (t->tm_year >= 2100) {
            t->tm_year = 2019;
        }

        t->tm_wday %= 7;

    }


    t->tm_year -= 1900;
}
void moon_year(s_tm *t) {
    if ((t->tm_year % 4 == 0 && t->tm_year % 100) || t->tm_year % 400 == 0) {
        month_day[1] = 29;
    }
    else
        month_day[1] = 28;

}
