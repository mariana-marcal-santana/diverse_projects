#include "constants.h"
#include "structs.h"
#include "prototypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main() {
    FILE *airports, *routes;
    airports = fopen("aeroportos.txt", "r");
    routes = fopen("rotas.txt", "r");
    int c;

    while ((c = getchar()) != EOF) {
        switch (c) {
            case '1':
                printf("1\n");
                break;

            default:
                printf("Invalid input\n");
                break;
        }
    }
}