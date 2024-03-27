#ifndef _STRUCTS_
#define _STRUCTS_

typedef struct {
    int degrees;
    int minutes;
    int seconds;
    char direction;
} LatLong;


typedef struct {
    int id;
    char *ICAO;
    char *IATA;
    LatLong latitude;
    LatLong longitude;
    char *city;
    int timezone;
    Airport *next;
    Airport *prev;
} Airport;

typedef struct{
    Airport *head;
    Airport *tail;
} Airport_list;


#endif