/*
    state : neighbour
    action : channel

    rssi değerine göre kanalları güncelle
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_CHANNELS 16
#define NUMBER_OF_TOTAL_NODE 100


FILE* file_input;
FILE* file_QTABLE;
FILE* file_mix_output;
FILE* file_number_of_chosen_action;

int number_of_neighbour;
int prev_number_of_neighbour;
float learning_rate = 0.1;
int number_of_dropped_packet=0;

int metric_values[NUMBER_OF_CHANNELS];
float QTABLE[NUMBER_OF_TOTAL_NODE][NUMBER_OF_CHANNELS];
int number_of_chosen_action_per_nbr[NUMBER_OF_TOTAL_NODE][NUMBER_OF_CHANNELS];
int unavailableChannels[] = {1,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1};

int read_cooja_log();
void train_algorithm();
int chooseAction(int state);
void init_QTABLE();
void update_QTABLE(int state, int action, float reward);
float get_reward(int channel,float rssi_metric);
void write_QTABLE_to_file(FILE* file);
void write_mix_output(int action, int timeslot);
void write_number_of_chosen_action_per_nbr();


void write_number_of_chosen_action_per_nbr(){
    fprintf(file_number_of_chosen_action,"\n------------- number of chosen action per neighbour ---------------\n");
    for(int nbr=0; nbr<prev_number_of_neighbour; nbr++){
        for(int channel=0; channel<NUMBER_OF_CHANNELS;channel++){
             fprintf(file_number_of_chosen_action,"%d, ",
                 number_of_chosen_action_per_nbr[nbr][channel]);
        }
        fprintf(file_number_of_chosen_action,"\n");
    }
    fprintf(file_number_of_chosen_action,"\n");
}

void write_mix_output(int action, int timeslot){

    fprintf(file_mix_output,"------------ rssi metric values ------------\n");
    for(int channel=0; channel<NUMBER_OF_CHANNELS; channel++){
        fprintf(file_mix_output,"%d, ", metric_values[channel]);
    }
    fprintf(file_mix_output,"\n");

    write_QTABLE_to_file(file_mix_output);

    fprintf(file_mix_output,"chosen action: %d, neighbour: %d\n \n",action, timeslot);
}

void write_QTABLE_to_file(FILE* file){
    fprintf(file," -------------------- QTABLE ----------------\n");
    for (int nbr = 0; nbr < NUMBER_OF_TOTAL_NODE; nbr++){
        if(QTABLE[nbr][0] == 0){
            continue;
        }
        for(int channel =0; channel< NUMBER_OF_CHANNELS; channel++){
            
            fprintf(file,"%f, ",QTABLE[nbr][channel]);
        }
        fprintf(file,"\n");
    } 
}

float get_reward(int channel, float rssi_metric){

    return rssi_metric/10.0;
}

void update_QTABLE(int state, int action, float reward){

    QTABLE[state][action] = (1 - learning_rate) * QTABLE[state][action] +
             learning_rate * reward;
}

void init_QTABLE(){
    for(int nbr=0; nbr<NUMBER_OF_TOTAL_NODE; nbr++){
        for(int channel=0; channel<NUMBER_OF_CHANNELS; channel++){
            QTABLE[nbr][channel] = 0;
        }
    }
}

int chooseAction(int state){

    // Epsilon-Greedy Policy
    float epsilon = 0.1;
    if ((double)rand() / RAND_MAX < epsilon){// Exploration
        return rand() % NUMBER_OF_CHANNELS;
    }
    else{// Exploitation
        int bestAction = 0;
        for (int action = 1; action < NUMBER_OF_CHANNELS; action++){
            if (QTABLE[state][action] > QTABLE[state][bestAction]){
                bestAction = action;
            }
        }
        return bestAction;
    }
}

int read_cooja_log(){
    
    int a;

    if(fscanf(file_input,"%d",&a) == EOF){
        printf("dosya sonu");
        return EOF;
    }

    return a;
}

void train_algorithm(){

    int neighbour_id = -1;
    float reward = 0;
    int action = -1;

    while((number_of_neighbour = read_cooja_log()) != EOF){
        for(int nbr=0; nbr<number_of_neighbour; nbr++){
            neighbour_id = read_cooja_log();
            for(int channel=0; channel<NUMBER_OF_CHANNELS; channel++){
                
                metric_values[channel] = read_cooja_log();
                if(unavailableChannels[channel] == 0){
                    metric_values[channel] -= 1;
                }
                reward = get_reward(channel,metric_values[channel]);
                update_QTABLE(neighbour_id,channel,reward);
            }
            action = chooseAction(neighbour_id);
            if(unavailableChannels[action] == 0){
                    number_of_dropped_packet++;
            }
            number_of_chosen_action_per_nbr[nbr][action]++;
            write_mix_output(action,neighbour_id);
        }
        write_QTABLE_to_file(file_QTABLE);
        prev_number_of_neighbour = number_of_neighbour;
    }    
}

int main(){
    srand(time(0));

    file_input = fopen("input/id85.txt", "r");
    file_QTABLE = fopen("outputs/q-table.txt","w");
    file_mix_output = fopen("outputs/mix-output.txt", "w");
    file_number_of_chosen_action = fopen("outputs/number-of-chosen-action.txt", "w");

    init_QTABLE();

    train_algorithm();
    write_number_of_chosen_action_per_nbr();

    printf("\ndropped packet: %d\n",number_of_dropped_packet);

    fclose(file_input);
    fclose(file_QTABLE);
    fclose(file_mix_output);
}