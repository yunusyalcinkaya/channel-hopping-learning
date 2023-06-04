#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHANNELS 16
#define SLOTFRAME_SIZE 17
#define LIMIT 1000

FILE *file_outputs;
FILE *file_actions;
FILE *file_metrics;
FILE *file_channels_availability;

double metric_values[NUM_CHANNELS];
double metric_average[NUM_CHANNELS];
int channels_availability[SLOTFRAME_SIZE][NUM_CHANNELS];
int temp_metric_values[NUM_CHANNELS];

int iteration = 0;
int dropped;

int chooseAction();
void process();
void init_channels_availability();
void write_channels_availability_to_file();
void init_metric_average();
void init_metric_values();
void random_metric_values(int timeslot);
void copy_metric_values_to_temp();
void write_metric_values_to_file();
void print_metric_average();
void write_outputs_to_file(int action);


void write_outputs_to_file(int action){
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_metric_values[channel]);
    }
    fprintf(file_outputs,"chosen action: %d\n",action);
    fprintf(file_outputs,"----------------------------------------------\n");
}

void print_metric_average(){
    printf("------------metric average---------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        printf("%f, ", metric_average[channel]);
    }
    printf("\n");
}

void copy_metric_values_to_temp(){
    for(int channel=0; channel <NUM_CHANNELS; channel++){
        temp_metric_values[channel] = metric_values[channel];
    }
}

void write_metric_values_to_file(){
    for(int metric=0; metric<NUM_CHANNELS; metric++){
        fprintf(file_metrics,"%f\n",metric_values[metric]);
    }
}

void random_metric_values(int timeslot){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        metric_values[channel]= ((double)rand() / RAND_MAX)*30 -90;

        metric_average[channel] = (metric_average[channel] * iteration +
                    metric_values[channel]*channels_availability[timeslot][channel])/
                    (iteration + 1);
                        
        fprintf(file_outputs,"metric value: %f, metric average: %f\n",
                                metric_values[channel],metric_average[channel]);
    }
    iteration++;
    write_metric_values_to_file();
}

void init_metric_average(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        metric_average[channel] = -90;
    }
}

void init_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        metric_values[channel] = -90;
    }
}

void write_channels_availability_to_file(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            fprintf(file_channels_availability,"%d\n",channels_availability[timeslot][channel]);
        }
    }
}

void init_channels_availability(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            if((double)rand()/RAND_MAX < 0.5){
                channels_availability[timeslot][channel] = 1; //available
            }
            else{
                channels_availability[timeslot][channel] =2; //unavailable
            }
        }
    }
}

int chooseAction(){
    int max_index=0;
    for(int channel = 1; channel < NUM_CHANNELS; channel++){
        if(temp_metric_values[channel] > temp_metric_values[max_index]){
            max_index = channel;
        }
    }
    return max_index;
}

void process(){

    int action;

    // repeat the slotframe #LIMIT times
    for(int slotframe = 0; slotframe < LIMIT; slotframe++){

        // loop over timeslots of the slotframe
        for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){

            random_metric_values(timeslot);
            
            action = chooseAction();

            if(channels_availability[timeslot][action] == 2){
                dropped++;
            }

            write_outputs_to_file(action);

            fprintf(file_actions,"%d\n",action);

            copy_metric_values_to_temp();
        }


    }
}

int main(){

    file_actions = fopen("../outputs/greedy-test-actions.txt","w");
    file_outputs = fopen("../outputs/greedy-test-outputs.txt","w");
    file_metrics = fopen("../outputs/greedy-test-metrics.txt","w");
    file_channels_availability = fopen("../outputs/greedy-available-channels.txt","w");

    srand(time(0));

    init_metric_average();
    init_metric_values;
    init_channels_availability();
    write_channels_availability_to_file();
    process();

    print_metric_average();
    printf("dropped: %d\n",dropped);

    fclose(file_actions);
    fclose(file_outputs);

    return 0;
}