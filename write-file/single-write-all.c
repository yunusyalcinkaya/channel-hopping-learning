// single state, random reward, write all outputs to file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ACTIONS 16
#define MAX_STEPS 10000

void update_Q_TABLE(int action, double reward, double alpha);
int chooseAction();
void train_algorithm(FILE* file);
void print_Q_TABLE(FILE* file);
int prev_action = 0;

FILE *file;

double Q_TABLE[NUM_ACTIONS];
double rewards[NUM_ACTIONS];

int chooseAction(){

    // Epsilon-Greedy Policy
    double epsilon = 0.1;
    if ((double)rand() / RAND_MAX < 0){
        fprintf(file,"UNDESIRED CASE\n");
        // Exploration
        return rand() % NUM_ACTIONS;
    }
    else{
        // Exploitation
        int bestAction = 0;
        for (int i = 1; i < NUM_ACTIONS; i++){
            if (Q_TABLE[i] > Q_TABLE[bestAction]){
                bestAction = i;
            }
        }
        return bestAction;
    }
}

void update_Q_TABLE(int action, double reward, double alpha){
    Q_TABLE[action] = (1 - alpha) * Q_TABLE[action] + alpha * reward;
}

// Q-Learning Algorithm
void train_algorithm(FILE* file){

    for (int step = 0; step < MAX_STEPS; step++){
        int action = chooseAction();// best action index
        double reward;
        
        print_Q_TABLE(file);
        fprintf(file,"chosen action: %d\n", action);

        if(prev_action == action){
            reward = -10;
        }
        else {
            fprintf(file,"rewards: ");
            for(int i = 0; i < NUM_ACTIONS; i++){
                rewards[i] = (((double)rand() / RAND_MAX) * 10) + Q_TABLE[i];
                // if((double)rand()/RAND_MAX < 0.5){
                //     rewards[i] *= -1;
                // }
                fprintf(file,"%f, ",rewards[i]);
            }
            reward = rewards[action];
        }
        fprintf(file,"\n --------------------------------------\n");


        prev_action = action;

        // Update the Q-Table
        double alpha = 0.1; // Learning Rate

        update_Q_TABLE(action, reward, alpha);
    }
}

void print_Q_TABLE(FILE* file){

    fprintf(file,"Q-table:\n");

    for (int j = 0; j < NUM_ACTIONS; j++){
        fprintf(file,"%f ", Q_TABLE[j]);
    }

    fprintf(file,"\n");
}

int main(){
    file = fopen("../outputs/single-write-all.txt","w");

    // Fill the Q-Table with initial values
    for (int j = 0; j < NUM_ACTIONS; j++){
        Q_TABLE[j] = (double)j;
    }
        Q_TABLE[0] = 25.0;

    train_algorithm(file);

    print_Q_TABLE(file);
    fclose(file);
    return 0;
}