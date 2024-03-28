#include <stdio.h>
void case_flights() ;
void case_airports() ;
void read_airports(FILE *airports) ;
void read_fligths(FILE *routes) ;
void show_direct_flights(char *argv1, char *argv2) ;
void show_direct_flights_sorted(char *origin, char *destiny, char *sort_type);
void show_flights_1connection(char *origin, char *destiny) ;
void show_flights_2connections(char *origin, char *destiny) ;
void sort_flights_ascending(Flight_list *flights);
void sort_flights_descending(Flight_list *flights);
void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type) ;
void show_flights_2connections_sorted(char *origin, char *destiny, char *sort_type) ;
void show_flights_shortest_distance_1connection(char *origin, char *destiny, char *sort_type) ;
void show_flights_shortest_distance_2connections(char *origin, char *destiny, char *sort_type) ;
double calc_airport_distance(char *argv1, char *argv2) ;
void print_flight_information(Flight *flight) ;
void free_system();