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

    fprintf(file_mix_output,"chosen action: %d, neighbour: %d\n \n",action, timeslot);
}

int chooseAction(int state){

    int bestAction = 0;
    for (int action = 1; action < NUMBER_OF_CHANNELS; action++){
        if(metric_values[bestAction] < metric_values[action]){
            bestAction = action;
        }
    }
    return bestAction;
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
    int action = -1;

    while((number_of_neighbour = read_cooja_log()) != EOF){
        for(int nbr=0; nbr<number_of_neighbour; nbr++){
            neighbour_id = read_cooja_log();
            for(int channel=0; channel<NUMBER_OF_CHANNELS; channel++){
                metric_values[channel] = read_cooja_log();
                if(unavailableChannels[channel] == 0){
                    metric_values[channel] -= 1;
                }
            }
            action = chooseAction(neighbour_id);
            if(unavailableChannels[action] == 0){
                    number_of_dropped_packet++;
            }
            number_of_chosen_action_per_nbr[nbr][action]++;
            write_mix_output(action,neighbour_id);
        }

        prev_number_of_neighbour = number_of_neighbour;
    }    
}

int main(){

    file_input = fopen("input/id85.txt", "r");

    file_mix_output = fopen("greedy-outputs/mix-output.txt", "w");
    file_number_of_chosen_action = fopen("greedy-outputs/number-of-chosen-action.txt", "w");

    train_algorithm();
    write_number_of_chosen_action_per_nbr();

    printf("\ndropped packet: %d\n",number_of_dropped_packet);

    fclose(file_input);
    fclose(file_mix_output);
}