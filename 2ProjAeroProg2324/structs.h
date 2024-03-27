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
} Airport;

#endif