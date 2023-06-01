// single state, print only actions to file 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ACTIONS 16
#define MAX_STEPS 10000

FILE *file;

void update_Q_TABLE(int action, double reward, double alpha);
int chooseAction();
void train_algorithm();
void print_Q_TABLE();
int prev_action = 0;

double Q_TABLE[NUM_ACTIONS];

//double rssi_metric_values[]= {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0};
double rssi_metric_values[]= {-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-85.0,-85.0,
                            -85.0,-85.0,-85.0,-80.0,-80.0,-80.0,-80.0,-80.0};

double second_metric[] = {};

// double rssi_metric_values[]= {-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,
//                             -90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0,-90.0};



int chooseAction(){

    // Epsilon-Greedy Policy
    double epsilon = 0.1;
    if ((double)rand() / RAND_MAX < epsilon){
        printf("UNDESIRED CASE\n");
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
void train_algorithm(){

    for (int step = 0; step < MAX_STEPS; step++){
        int action = chooseAction();// best action index
        double reward;
       
        fprintf(file,"%d\n", action);
        
        print_Q_TABLE();
        printf("choosen action: %d\n", action);

        if(prev_action == action){
            reward = -10;
        }
        else {
            reward =  (rssi_metric_values[action] - rssi_metric_values[prev_action])*10;
            printf("\n --------------------------------------\n");
        }


        prev_action = action;

        // Update the Q-Table
        double alpha = 0.1; // Learning Rate

        update_Q_TABLE(action, reward, alpha);
    }
}

void print_Q_TABLE(){

    printf("Q-table:\n");

    for (int j = 0; j < NUM_ACTIONS; j++)
    {
        printf("%f ", Q_TABLE[j]);
    }

    printf("\n");
}

int main(){

    file = fopen("../outputs/single-state.txt","w");

    // Fill the Q-Table with initial values
    for (int j = 0; j < NUM_ACTIONS; j++)
    {
        Q_TABLE[j] = 0;
    }
        //Q_TABLE[0] = 25.0;

    train_algorithm();

    print_Q_TABLE();

    fclose(file);
    return 0;
}