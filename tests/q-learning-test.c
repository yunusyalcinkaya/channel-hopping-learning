#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHANNELS 16 // actions (channels)
#define SLOTFRAME_SIZE 17 // states (timeslots)
#define LIMIT 1000

FILE *file_outputs;
FILE *file_actions;
FILE *file_metrics;
FILE *file_channels_availability;

double QTABLE[SLOTFRAME_SIZE][NUM_CHANNELS];

double metric_values[NUM_CHANNELS];
int channels_availability[SLOTFRAME_SIZE][NUM_CHANNELS];
int temp_metric_values[NUM_CHANNELS];

int iteration = 0;
int dropped;
double learning_rate = 0.1;

int chooseAction(int state);
void process();
void init_QTABLE();
void init_metric_values();
void copy_metric_values_to_temp();
void read_metric_values_from_file();
void write_outputs_to_file(int action);
void update_QTABLE(int state, int action, double reward);
double get_reward(double metric,int state, int action);
void read_channels_availability_from_file();



void read_channels_availability_from_file(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            fscanf(file_channels_availability,"%d",channels_availability[timeslot][channel]);
        }
    }
}

double get_reward(double metric, int state, int action){
    if(channels_availability[state][action] == 2){
        return -10;
    }
    return -(1/metric)*100;
}

void update_QTABLE(int state, int action, double reward){

    QTABLE[state][action] = (1 - learning_rate) * QTABLE[state][action] + learning_rate * reward;
}

void init_QTABLE(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            QTABLE[timeslot][channel] = 0;
        }
    }
}

void write_outputs_to_file(int action){
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_metric_values[channel]);
    }
    fprintf(file_outputs,"chosen action: %d\n",action);
    fprintf(file_outputs,"----------------------------------------------\n");
}


void copy_metric_values_to_temp(){
    for(int channel=0; channel <NUM_CHANNELS; channel++){
        temp_metric_values[channel] = metric_values[channel];
    }
}

void read_metric_values_from_file(){
    for(int metric=0; metric<NUM_CHANNELS; metric++){
        fscanf(file_metrics,"%f",&metric_values[metric]);
    }
}

void init_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        metric_values[channel] = -90;
    }
}


int chooseAction(int state){

    // Epsilon-Greedy Policy
    double epsilon = 0.1;
    if ((double)rand() / RAND_MAX < epsilon){
        printf("EXPLORATION CASE\n");
        // Exploration
        return rand() % NUM_CHANNELS;
    }
    else{
        // Exploitation
        int bestAction = 0;
        for (int action = 1; action < NUM_CHANNELS; action++){
            if (QTABLE[state][action] > QTABLE[state][bestAction]){
                bestAction = action;
            }
        }
        return bestAction;
    }
}

void process(){

    int action;
    double reward, metric;

    // repeat the slotframe #LIMIT times
    for(int slotframe = 0; slotframe < LIMIT; slotframe++){

        // loop over timeslots of the slotframe
        for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){

            
            action = chooseAction(timeslot);

            if(channels_availability[timeslot][action] == 2){
                dropped++;
            }

            read_metric_values_from_file();

            metric = temp_metric_values[action];

            reward = get_reward(metric,timeslot,action);

            update_QTABLE(timeslot,action,reward);

            write_outputs_to_file(action);

            fprintf(file_actions,"%d\n",action);

            copy_metric_values_to_temp();
        }


    }
}

int main(){

    file_actions = fopen("../outputs/q-learning-test-actions.txt","w");
    file_outputs = fopen("../outputs/greedy-test-outputs.txt","w");
    file_metrics = fopen("../outputs/greedy-test-metrics.txt","r");
    file_channels_availability = fopen("../outputs/greedy-available-channels.txt","r");

    srand(time(0));

    init_QTABLE();
    init_metric_values;
    process();
    printf("dropped: %d\n",dropped);

    fclose(file_actions);
    fclose(file_outputs);


    return 0;
}