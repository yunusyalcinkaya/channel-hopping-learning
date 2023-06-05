#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHANNELS 80
#define SLOTFRAME_SIZE 17
#define LIMIT 1000

FILE *file_outputs;
FILE *file_actions;
FILE *file_rssi_metrics;
FILE *file_channels_availability;
FILE *file_lqi_metrics;

float rssi_metric_values[NUM_CHANNELS];
float rssi_metric_average[NUM_CHANNELS];
float lqi_metric_values[NUM_CHANNELS];
float lqi_metric_average[NUM_CHANNELS];
int channels_availability[SLOTFRAME_SIZE][NUM_CHANNELS];
float temp_rssi_metric_values[NUM_CHANNELS];
float temp_lqi_metric_values[NUM_CHANNELS];
float total_metric_average[NUM_CHANNELS];

int iteration = 0;
int dropped;

int chooseAction();
void process();
void init_channels_availability();
void write_channels_availability_to_file();
void init_rssi_metric_average();
void init_rssi_metric_values();
void random_metric_values(int timeslot);// for both rssi and lqi
void copy_metric_values_to_temp();// for both rssi and lqi
void write_metric_values_to_file();// for both rssi and lqi
void print_metric_average();
void write_outputs_to_file(int action);
void init_lqi_metric_values();
void init_lqi_metric_average();
void random_metric_values2(int timeslot);

void init_lqi_metric_average(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        lqi_metric_average[channel] = 100;
    }
}

void init_lqi_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        lqi_metric_values[channel] = 100;
    }
}

void write_outputs_to_file(int action){
    fprintf(file_outputs,"rssi metric values: \n");
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_rssi_metric_values[channel]);
    }
    fprintf(file_outputs,"\n lqi metric values: \n");
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_lqi_metric_values[channel]);
    }
    fprintf(file_outputs,"\nchosen action: %d\n",action);
    fprintf(file_outputs,"----------------------------------------------\n");
}

void print_metric_average(){
    printf("------------rssi metric average---------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        printf("%f, ", rssi_metric_average[channel]);
    }
    printf("\n");
    printf("------------lqi metric average---------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        printf("%f, ", lqi_metric_average[channel]);
    }
    printf("\n");
    printf("------------total metric average---------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        printf("%f, ", total_metric_average[channel]);
    }
    printf("\n");
}

void copy_metric_values_to_temp(){
    for(int channel=0; channel <NUM_CHANNELS; channel++){
        temp_rssi_metric_values[channel] = rssi_metric_values[channel];
        temp_lqi_metric_values[channel] = lqi_metric_values[channel];
    }
}

void write_metric_values_to_file(){

    for(int metric=0; metric<NUM_CHANNELS; metric++){
        fprintf(file_rssi_metrics,"%f\n",rssi_metric_values[metric]);
        fprintf(file_lqi_metrics,"%f\n",lqi_metric_values[metric]);
    }
    
}

void random_metric_values(int timeslot){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        rssi_metric_values[channel]= ((float)rand() / RAND_MAX)*30 -90;
        lqi_metric_values[channel] = ((float)rand() / RAND_MAX)*100;

        rssi_metric_average[channel] = (rssi_metric_average[channel] * iteration +
                    rssi_metric_values[channel]*channels_availability[timeslot][channel])/
                    (iteration + 1);
        lqi_metric_average[channel] = (lqi_metric_average[channel] * iteration +
                    lqi_metric_values[channel]/channels_availability[timeslot][channel])/
                    (iteration + 1);
        total_metric_average[channel] = (lqi_metric_average[channel] +
                                            rssi_metric_average[channel]) /2;
                        
        fprintf(file_outputs,"rssi metric value: %f, rssi metric average: %f\n",
                                rssi_metric_values[channel],rssi_metric_average[channel]);
        fprintf(file_outputs,"lqi metric value: %f, lqi metric average: %f\n",
                                lqi_metric_values[channel],lqi_metric_average[channel]);
    }
    iteration++;
    write_metric_values_to_file();
}

void random_metric_values2(int timeslot){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        rssi_metric_values[channel]= ((float)rand() / RAND_MAX)*20 -90;
        lqi_metric_values[channel] = ((float)rand() / RAND_MAX)*100;

        rssi_metric_average[channel] = (rssi_metric_average[channel] * iteration +
                    rssi_metric_values[channel])/
                    (iteration + 1);
        lqi_metric_average[channel] = (lqi_metric_average[channel] * iteration +
                    lqi_metric_values[channel])/
                    (iteration + 1);
        total_metric_average[channel] = (lqi_metric_average[channel] +
                                            rssi_metric_average[channel]) /2;
                        
        fprintf(file_outputs,"rssi metric value: %f, rssi metric average: %f\n",
                                rssi_metric_values[channel],rssi_metric_average[channel]);
        fprintf(file_outputs,"lqi metric value: %f, lqi metric average: %f\n",
                                lqi_metric_values[channel],lqi_metric_average[channel]);
    }
    iteration++;
    write_metric_values_to_file();
}

void init_rssi_metric_average(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        rssi_metric_average[channel] = -90;
    }
}

void init_rssi_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        rssi_metric_values[channel] = -90;
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
        if(temp_rssi_metric_values[channel] + temp_lqi_metric_values[channel] > 
                    temp_rssi_metric_values[max_index] + temp_lqi_metric_values[max_index]){
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

            random_metric_values2(timeslot);
            
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
    file_rssi_metrics = fopen("../outputs/greedy-test-rssi-metrics.txt","w");
    file_channels_availability = fopen("../outputs/greedy-available-channels.txt","w");
    file_lqi_metrics = fopen("../outputs/greedy-test-lqi-metrics.txt","w");

    srand(time(0));

    init_lqi_metric_average();
    init_lqi_metric_values();
    init_rssi_metric_average();
    init_rssi_metric_values;
    init_channels_availability();
    write_channels_availability_to_file();
    process();

    print_metric_average();
    printf("dropped: %d\n",dropped);

    fclose(file_actions);
    fclose(file_outputs);
    fclose(file_rssi_metrics);
    fclose(file_channels_availability);
    fclose(file_lqi_metrics);

    return 0;
}