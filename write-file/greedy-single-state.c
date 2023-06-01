// single state, print only actions to file 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHANNEL 16
#define MAX_STEPS 10000

FILE *file_outputs;
FILE *file_actions;
FILE *file_metrics;
double metric_values[NUM_CHANNEL];

double metric_average[NUM_CHANNEL];
int iteration = 0;

int chooseAction();
void process();

int chooseAction(){
    int max_index=0;
    for(int i = 1; i < NUM_CHANNEL; i++){
        if(metric_values[i] > metric_values[max_index])
            max_index = i;
    }
    return max_index;
}

void process(){

    int action;

    for(int i = 0; i < MAX_STEPS; i++){

        for(int i=0; i<NUM_CHANNEL; i++){
            metric_values[i]= ((double)rand() / RAND_MAX)*20 -90;

            fprintf(file_metrics,"%f\n",metric_values[i]);
            metric_average[i] = (metric_average[i] * iteration + metric_values[i]) / (iteration + 1);
            fprintf(file_outputs,"metric value: %f, metric average: %f\n",metric_values[i],metric_average[i]);
        }
        iteration++;

        action = chooseAction(metric_values);

        fprintf(file_outputs,"\n");
        fprintf(file_outputs,"chosen action: %d\n",action);
        fprintf(file_outputs,"----------------------------------------------\n");

        fprintf(file_actions,"%d\n",action);
    }
}


int main(){

    file_actions = fopen("../outputs/greedy-actions.txt","w");
    file_outputs = fopen("../outputs/greedy-outputs.txt","w");
    file_metrics = fopen("../outputs/greedy-metrics.txt","w");

    process();

    fclose(file_actions);
    fclose(file_outputs);
    printf("metrics average: ");
    for(int i=0; i<NUM_CHANNEL;i++){
        printf("%f, ", metric_average[i]);
    }
    printf("\n");

    return 0;
}