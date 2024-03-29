#ifndef SHOW_H
#define SHOW_H

#include "common.h"

void show_direct_flights(char *origin, char *destiny, Flight_list *flights_list) ;
void show_flights_1connection(char *origin, char *destiny, Flight_list *flights_list) ;
void show_flights_2connections(char *origin, char *destiny, Flight_list *flights_list) ;
void show_direct_flights_sorted(char *origin, char *destiny, char *sort_type, Flight_list *flights_list) ;
void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type, Flight_list *flights_list) ;
void show_flights_2connections_sorted(char *origin, char *destiny, char *sort_type, Flight_list *flights_list) ;
/*void show_flights_shortest_distance_1connection(char *origin, char *destiny, char *sort_type,Flight_list *flights_list) {}
void show_flights_shortest_distance_2connections(char *origin, char *destiny, char *sort_type,Flight_list *flights_list) {}
*/
#endif // SHOW_H
