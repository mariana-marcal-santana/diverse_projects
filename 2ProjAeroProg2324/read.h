#ifndef READ_H
#define READ_H

#include "common.h"

void read_airports(FILE *airports, Airport_list *airports_list);
void read_fligths(FILE *routes , Flight_list *flights_list);

#endif // READ_H