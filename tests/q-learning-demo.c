#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHANNELS 16 // actions (channels)
#define SLOTFRAME_SIZE 5 // states (timeslots)
#define LIMIT 1000

FILE *file_outputs;
FILE *file_actions;
FILE *file_rssi_metrics;
FILE *file_channels_availability;
FILE *file_lqi_metrics;
FILE *file_formatted_channels_availability;
FILE *file_number_of_chosen_action_per_slot;
FILE *file_QTABLE;
FILE *file_mix_output;

float QTABLE[SLOTFRAME_SIZE][NUM_CHANNELS];
float rssi_metric_values[NUM_CHANNELS];
int channels_availability[SLOTFRAME_SIZE][NUM_CHANNELS];
float temp_rssi_metric_values[NUM_CHANNELS];
float lqi_metric_values[NUM_CHANNELS];
float temp_lqi_metric_values[NUM_CHANNELS];
int number_of_chosen_action_per_slot[SLOTFRAME_SIZE][NUM_CHANNELS];

int iteration = 0;
int dropped;
float learning_rate = 0.1;
int previous_action =0;

int chooseAction(int state);
void process();
void init_QTABLE();
void init_rssi_metric_values();
void init_lqi_metric_values();
void copy_metric_values_to_temp();
void read_metric_values_from_file();
void write_outputs_to_file(int action);
void update_QTABLE(int state, int action, float reward);
float get_reward(float rssi_metric, float lqi_metric, int state, int action);
void read_channels_availability_from_file();
void write_QTABLE_to_file();
void write_channels_availability_to_file();
void init_number_of_chosen_action_per_slot();
void write_number_of_chosen_action_per_slot();
void write_mix_output(int action, int timeslot);


void write_mix_output(int action, int timeslot){
    fprintf(file_mix_output,"------------ lqi metric values ------------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        fprintf(file_mix_output,"%f, ", temp_lqi_metric_values[channel]);
    }
    fprintf(file_mix_output,"\n");
    fprintf(file_mix_output,"------------ rssi metric values ------------\n");
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        fprintf(file_mix_output,"%f, ", temp_rssi_metric_values[channel]);
    }
    fprintf(file_mix_output,"\n");
    fprintf(file_mix_output,"------------ Q-TABLE ------------\n");
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            fprintf(file_mix_output,"%f, ",QTABLE[timeslot][channel]);
        }
        fprintf(file_mix_output,"\n");
    }
    fprintf(file_mix_output,"chosen action: %d, timeslot: %d\n \n",action, timeslot);
}



void init_number_of_chosen_action_per_slot(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS;channel++){
            // fprintf(file_number_of_chosen_action_per_slot,"%d",
            //     number_of_chosen_action_per_slot[timeslot][channel]);
            number_of_chosen_action_per_slot[timeslot][channel] =0;
        }
    }
}

void write_number_of_chosen_action_per_slot(){
    fprintf(file_number_of_chosen_action_per_slot,"\n------------- number of chosen action per slot ---------------\n");
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS;channel++){
             fprintf(file_number_of_chosen_action_per_slot,"%d, ",
                 number_of_chosen_action_per_slot[timeslot][channel]);
        }
        fprintf(file_number_of_chosen_action_per_slot,"\n");
    }
    fprintf(file_number_of_chosen_action_per_slot,"\n");
}


void write_channels_availability_to_file(){
    fprintf(file_formatted_channels_availability,"----------- channels availability-----------\n");
    for(int i=0; i<SLOTFRAME_SIZE;i++){
        for(int j=0; j<NUM_CHANNELS; j++){
            fprintf(file_formatted_channels_availability,"%d, ",channels_availability[i][j]);
        }
        fprintf(file_formatted_channels_availability,"\n");
    }
    fprintf(file_formatted_channels_availability,"\n");
}

void write_QTABLE_to_file(){
    fprintf(file_QTABLE,"\n -------------------- QTABLE ----------------\n");
    for (int timeslot = 0; timeslot < SLOTFRAME_SIZE; timeslot++){
        for(int channel =0; channel< NUM_CHANNELS; channel++){
            fprintf(file_QTABLE,"%f, ",QTABLE[timeslot][channel]);
        }
        fprintf(file_QTABLE,"\n");
    } 
}

void read_channels_availability_from_file(){
    for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){
        for(int channel=0; channel<NUM_CHANNELS; channel++){
            fscanf(file_channels_availability,"%d",&channels_availability[timeslot][channel]);
        }
    }
}

float get_reward(float rssi_metric, float lqi_metric, int state, int action){
    if(channels_availability[state][action] == 2){// || previous_choise[state] == action
        return -50;
    }
    //previous_action = action;
    //return -(10/rssi_metric)*lqi_metric;
    //return metric +91;
    return rssi_metric + lqi_metric;
}


void update_QTABLE(int state, int action, float reward){

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
    fprintf(file_outputs,"rssi metric values: \n");
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_rssi_metric_values[channel]);
    }
    fprintf(file_outputs,"\n lqi metric values: \n");
    for(int channel=0; channel < NUM_CHANNELS; channel++){
            fprintf(file_outputs,"%f, ", temp_lqi_metric_values[channel]);
    }
    write_QTABLE_to_file();
    fprintf(file_outputs,"\n chosen action: %d\n",action);
    fprintf(file_outputs,"----------------------------------------------\n");
}


void copy_metric_values_to_temp(){
    for(int channel=0; channel <NUM_CHANNELS; channel++){
        temp_rssi_metric_values[channel] = rssi_metric_values[channel];
        temp_lqi_metric_values[channel] = lqi_metric_values[channel];
    }
}

void read_metric_values_from_file(){
    for(int metric=0; metric<NUM_CHANNELS; metric++){
        fscanf(file_rssi_metrics,"%f",&rssi_metric_values[metric]);
        fscanf(file_lqi_metrics, "%f",&lqi_metric_values[metric]);
    }
}

void init_rssi_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        rssi_metric_values[channel] = -90;
        temp_rssi_metric_values[channel] = -90;
    }
}
void init_lqi_metric_values(){
    for(int channel=0; channel<NUM_CHANNELS; channel++){
        lqi_metric_values[channel] = 100;
        temp_lqi_metric_values[channel] =100;
    }
}

int chooseAction(int state){

    // Epsilon-Greedy Policy
    float epsilon = 0.0;
    if ((double)rand() / RAND_MAX < epsilon){// Exploration
        return rand() % NUM_CHANNELS;
    }
    else{// Exploitation
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
    float reward, rssi_metric, lqi_metric;

    // repeat the slotframe #LIMIT times
    for(int slotframe = 0; slotframe < LIMIT; slotframe++){

        // loop over timeslots of the slotframe
        for(int timeslot=0; timeslot<SLOTFRAME_SIZE; timeslot++){

            read_metric_values_from_file();

            action = chooseAction(timeslot);
            number_of_chosen_action_per_slot[timeslot][action]++;
            write_mix_output(action,timeslot);

            if(channels_availability[timeslot][action] == 2){
                dropped++;
            }

            // rssi_metric = temp_rssi_metric_values[action];
            // lqi_metric = temp_lqi_metric_values[action];

            // reward = get_reward(rssi_metric, lqi_metric,timeslot,action);
            // update_QTABLE(timeslot,action,reward);

            for(int channel=0;channel<NUM_CHANNELS;channel++){
                reward = get_reward(temp_rssi_metric_values[channel],
                            temp_lqi_metric_values[channel],timeslot,channel);
                update_QTABLE(timeslot,channel,reward);
            }

            copy_metric_values_to_temp();


            write_outputs_to_file(action);
            fprintf(file_actions,"%d\n",action);

            
        }
    }
}

int main(){

    file_actions = fopen("../learning-demo-outputs/actions.txt","w");
    file_outputs = fopen("../learning-demo-outputs/metric-outputs.txt","w");
    file_formatted_channels_availability = fopen("../learning-demo-outputs/formatted-channels-availability.txt","w");
    file_number_of_chosen_action_per_slot = fopen("../learning-demo-outputs/number-of-chosen-action-per-slot.txt","w");
    file_QTABLE = fopen("../learning-demo-outputs/q-table.txt","w");
    file_mix_output = fopen("../learning-demo-outputs/mix-output.txt","w");

    // read
    file_rssi_metrics = fopen("../inputs/greedy-test-rssi-metrics.txt","r");
    file_channels_availability = fopen("../inputs/greedy-available-channels.txt","r");
    file_lqi_metrics = fopen("../inputs/greedy-test-lqi-metrics.txt","r");

    srand(time(0));

    init_number_of_chosen_action_per_slot();
    read_channels_availability_from_file();
    init_QTABLE();
    init_rssi_metric_values();
    init_lqi_metric_values();
    process();
    printf("dropped: %d\n",dropped);
    write_QTABLE_to_file();
    write_channels_availability_to_file();
    write_number_of_chosen_action_per_slot();

    fclose(file_actions);
    fclose(file_outputs);
    fclose(file_rssi_metrics);
    fclose(file_channels_availability);
    fclose(file_lqi_metrics);
    fclose(file_formatted_channels_availability);
    fclose(file_number_of_chosen_action_per_slot);
    fclose(file_QTABLE);
    fclose(file_mix_output);

    return 0;
}