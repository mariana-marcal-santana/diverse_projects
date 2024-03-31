#ifndef _STRUCTS_
#define _STRUCTS_

typedef struct {
    int degrees;
    int minutes;
    int seconds;
    char direction;
} LatLong;

typedef struct Flight {
    char *airline;
    char *flight_code;
    char *depart_IATA;
    char *arrival_IATA;
    int depart_time_hour;
    int depart_time_minute;
    int arrival_time_hour;
    int arrival_time_minute;
    struct Flight *next;
    struct Flight *prev;
    double distance;
} Flight;

typedef struct {
    Flight *head;
    Flight *tail;
} Flight_list;

typedef struct Airport {
    int id;
    char *ICAO;
    char *IATA;
    LatLong latitude;
    LatLong longitude;
    char *city;
    int timezone;
    struct Airport *next;
    struct Airport *prev;
} Airport;

typedef struct{
    Airport *head;
    Airport *tail;
} Airport_list;

#endif