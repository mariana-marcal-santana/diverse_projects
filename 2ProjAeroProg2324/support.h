#ifndef SUPPORT_H
#define SUPPORT_H

#include "common.h"

void case_flights(Flight_list * flights_list,Airport_list *airports_list);
void case_airports(Airport_list *airports_list );
double calc_airport_distance(char *airportA_IATA, char *airportB_IATA,Airport_list *airports_list) ;
void print_flight_information(Flight *flight) ;
void free_system(Airport_list *airports_list, Flight_list *flights_list) ;
void free_airports_list(Airport_list *airports_list) ;
void free_flights_list(Flight_list *flights_list) ;
#endif