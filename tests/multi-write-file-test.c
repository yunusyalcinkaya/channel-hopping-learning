// multi state, constant metric, write only choosen action to file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ACTIONS 16
#define NUM_STATES 16
#define MAX_STEPS 10000

FILE *file;

void update_Q_TABLE(int state,int action, double reward, double alpha);
int chooseAction(int state);
void train_algorithm();
void print_Q_TABLE();

int current_state = 0;

double Q_TABLE[NUM_STATES][NUM_ACTIONS];
// double metric_values[]= {-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-85.0,-85.0,
//                             -85.0,-85.0,-85.0,-80.0,-80.0,-80.0,-80.0,-80.0};

double metric_values[]= {-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,
                            -90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0};


int chooseAction(int state){

    // Epsilon-Greedy Policy
    double epsilon = 0.2;
    if ((double)rand() / RAND_MAX < epsilon){
        printf("EXPLORATION CASE\n");
        // Exploration
        return rand() % NUM_ACTIONS;
    }
    else{
        // Exploitation
        int bestAction = 0;
        for (int i = 1; i < NUM_ACTIONS; i++){
            if (Q_TABLE[state][i] > Q_TABLE[state][bestAction]){
                bestAction = i;
            }
        }
        return bestAction;
    }
}

void update_Q_TABLE(int state, int action, double reward, double alpha){
    Q_TABLE[state][action] = (1 - alpha) * Q_TABLE[state][action] + alpha * reward;
}

// Q-Learning Algorithm
void train_algorithm(){

    for (int step = 0; step < MAX_STEPS; step++){
        int action = chooseAction(current_state);// best action index
        double reward;
       
        fprintf(file,"%d\n", action);

        print_Q_TABLE();
        printf("state: %d, choosen action: %d\n",current_state, action);

        if(current_state == action){
            reward = -10;
        }
        else {
            reward = (metric_values[action] - metric_values[current_state])*10;
            printf("\n --------------------------------------\n");
        }

        // Update the Q-Table
        double alpha = 0.1; // Learning Rate

        update_Q_TABLE(current_state, action, reward, alpha);
        current_state = action;
    }
}

void print_Q_TABLE(){

    printf("Q-table:\n");

    for(int i = 0; i < NUM_STATES;i++){
        for(int j = 0; j < NUM_ACTIONS; j++){
            printf("%f ", Q_TABLE[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

int main(){

    file = fopen("../outputs/multi-write-action-test.txt","w");

    for(int i = 0; i < NUM_STATES;i++){
        for(int j = 0; j < NUM_ACTIONS;j++){
            Q_TABLE[i][j] = 0;
        }
    }

    train_algorithm();

    print_Q_TABLE();

    fclose(file);
    return 0;
}