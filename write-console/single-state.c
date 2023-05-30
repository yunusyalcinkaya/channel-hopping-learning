// single state
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ACTIONS 16
#define MAX_STEPS 1000

void update_Q_TABLE(int action, double reward, double alpha);
int chooseAction();
void train_algorithm();
void print_Q_TABLE();
int prev_action = 0;

double Q_TABLE[NUM_ACTIONS];
double rewards[NUM_ACTIONS];

int chooseAction(){

    // Epsilon-Greedy Policy
    double epsilon = 0.1;
    if ((double)rand() / RAND_MAX < 0){
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
        
        print_Q_TABLE();
        printf("chosen action: %d\n", action);

        if(prev_action == action){
            reward = -5;
        }
        else {
            printf("rewards: ");
            for(int i = 0; i < NUM_ACTIONS; i++){
                rewards[i] = (((double)rand() / RAND_MAX) * 10) + Q_TABLE[i];
                if((double)rand()/RAND_MAX < 0.1){
                    rewards[i] *= -1;
                }
                printf("%f, ",rewards[i]);
            }
            reward = rewards[action];
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

    // Fill the Q-Table with initial values
    for (int j = 0; j < NUM_ACTIONS; j++)
    {
        Q_TABLE[j] = (double)j / 10;
    }
        Q_TABLE[0] = 25.0;

    train_algorithm();

    print_Q_TABLE();
    return 0;
}