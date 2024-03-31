#ifndef READ_H
#define READ_H

#include "common.h"

void read_airports(FILE *airports, Airport_list *airports_list);
void read_fligths(FILE *routes , Flight_list *flights_list,Airport_list *airports_list);
bool check_data_airport(Airport * airport);
bool check_data_flight(Flight * flight);

#endif // READ_H