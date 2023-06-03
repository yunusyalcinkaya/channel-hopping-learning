#include<stdio.h>
#include <stdlib.h>

#define STEP 10000
#define CHANNEL 16

int main(){
    srand(time(0));

    FILE *file = fopen("inputs/rssi-metrics","w");

    double number;
    int iteration;
    double average[CHANNEL];

    for(int i=0; i<STEP; i++){
        for(int j=0; j< CHANNEL; j++){
            number = ((double)rand() / RAND_MAX)*10 -90;

            average[j] = (average[j] * iteration + number) / (iteration + 1);

            fprintf(file, "%f\n", number);
        }
        iteration++;
    }

    printf("average metric values: \n");
    for(int i=0; i< CHANNEL; i++){
        printf("%f, ",average[i]);
    }
    printf("\n");
}