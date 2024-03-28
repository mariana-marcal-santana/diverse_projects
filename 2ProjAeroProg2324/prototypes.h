#include <stdio.h>
void case_flights() ;
void case_airports() ;
void read_airports(FILE *airports) ;
void read_fligths(FILE *routes) ;
void show_direct_flights(char *argv1, char *argv2) ;
void show_direct_flights_sorted(char *origin, char *destiny, char *sort_type);
void sort_flights_ascending(Flight_list *flights);
void sort_flights_descending(Flight_list *flights);
double calc_airport_distance(char *argv1, char *argv2) ;