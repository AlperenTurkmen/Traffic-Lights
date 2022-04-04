#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <time.h>

int l_max_time = 3;
int r_max_time = 4; //will be parameter
int l_time = 0;
int r_time = 0;
int r = 0;
int l = 0;
int left_arrival_rate = 90;
int right_arrival_rate = 1;
//To calculate the waiting time for a car, we need to record it
// when it's passing the lights, we need to do time-timestamp.

typedef struct node {
    /*
   This data structure is to keep the cars, I'll use them as nodes in a linked list,
    all cars will be connected to the car before & after them.
    */
    int timestamp;
    struct node *next;
} Car;


bool car_incoming(int probability) {
/*
 * Random boolean creator, decides if a car is coming from a side.
 * */
    return (rand() & 100) < probability;
}

bool light() { // check if there is green light for left side.
    /* Controls the lights:
     * If a light reaches its max time, this function changes the colors of the lights for both sides,
     * allows the other side to pass in the next timestep.
     * */

    if (r_time > 0) {
        if (r_time < r_max_time) {
            r_time++;
            return false;
        } else {
            r_time = 0;
            l_time++;
            return true;
        }
    } else {
        if (l_time < l_max_time) {
            l_time++;
            return true;
        } else {
            l_time = 0;
            r_time++;
            return false;
        }
    }

    return false;
}

void addCar(Car *head, int timestamp) {
    /* This function here adds a car to the selected queue,
     * and stamps the car with the current timestep.
     * */

    //printf("ADD %d \n",head);
    Car *current = head;
    //int s=0;
    //printf("current %d \n",head);
    while (current->next != NULL) {
        // s++;
        //  printf(" s =%d \n",s);
        current = current->next;
    }
    /* now we can add a new variable */
    current->next = (Car *) malloc(sizeof(Car));
    current->next->timestamp = timestamp;

    current->next->next = NULL;
}

int pop(Car **head) {
    /*
     * This function pops (removes) the car on the front. Checks the light, and also if there is a car in the queue.
     */
    int retval = -1;
    //printf("\n R L ********   %d % d\n", r, l);
    Car *next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->next;
    retval = (*head)->timestamp;

    free(*head);
    *head = next_node;
    return retval;
}

int runOneSimulation() {
    /*
     * This function runs a simulation of 500 steps.
     */
    Car *rightQueue = (Car *) malloc(sizeof(Car));
    Car *leftQueue = (Car *) malloc(sizeof(Car));
    int counter = 0;
    int total_cars_l = 0;
    int total_cars_r = 0;
    int left_waiting = 0;
    int right_waiting = 0;
    int total_waiting = 0;
    int max_wait_left = 0;
    int max_wait_right = 0;
    int waiting_time = 0;
    int right_clearance = 0;
    int left_clearance = 0;

    //srand(time(NULL));
    //addCar(leftQueue, 1);
    //addCar(rightQueue, 1);
    while (counter < 500) {
        counter++;
        //printf("Counter-----   %d. l=%d r=%d\n", counter, l, r);
        if (car_incoming(left_arrival_rate)) {
            addCar(leftQueue, counter);
            l++;
        }
        if (car_incoming(right_arrival_rate)) {
            r++;
            addCar(rightQueue, counter);

        }
        if (light()) {
            //printf("L-%d \n",counter);
            if (l > 0) {
                l--;
                waiting_time = counter - pop(&leftQueue);//I'll calculate the waiting time here.
                if (waiting_time > max_wait_left) {
                    max_wait_left = waiting_time;
                }
                total_waiting = total_waiting + waiting_time;
                total_cars_l++;

            }
        } else {
            //  printf("R-%d \n",counter);
            if (r > 0) {
                r--;
                waiting_time = counter - pop(&rightQueue);
                if (waiting_time > max_wait_right) {
                    max_wait_right = waiting_time;
                }
                total_waiting = total_waiting + waiting_time;
                total_cars_r++;

            }
        }

    }
    if (r > 0 || l > 0) {
        //printf("\nKalanlar %d %d", r, l);
    }
    while (r > 0 || l > 0) {
        counter++;
        //printf("Counter-----   %d. l=%d r=%d\n", counter, l, r);
        if (light()) {
            if (l > 0) {
                l--;
                waiting_time = counter - pop(&leftQueue);
                if (waiting_time > max_wait_left) {
                    max_wait_left = waiting_time;
                }
                left_waiting = left_waiting + waiting_time;
                total_cars_l++;
                left_clearance = counter - 500;
            }
        } else {
            if (r > 0) {
                r--;
                waiting_time = counter - pop(&rightQueue);
                if (waiting_time > max_wait_right) {
                    max_wait_right = waiting_time;
                }
                right_waiting = right_waiting + waiting_time;
                total_cars_r++;
                right_clearance = counter - 500;
            }
        }
    }

    if (r > 0 || l > 0) {
        //printf("\nek Kalanlar %d %d", r, l);
    }
    //printf("\nTotal number of cars in total: %d", total_cars_r + total_cars_l);
    //printf("\nTotal waiting time: %d", left_waiting + right_waiting);
    //printf("\nAverage waiting time: %d", total_waiting / (total_cars_r + total_cars_l));
    printf("\nParameter values:");
    printf("\n  from right:");
    printf("\n      traffic arrival rate: %d", right_arrival_rate);
    printf("\n      traffic light period: %d", r_max_time);
    printf("\n  from left:");
    printf("\n      traffic arrival rate: %d", left_arrival_rate);
    printf("\n      traffic light period: %d\n", l_max_time);

    printf("\nResults ( averaged over a run ):");

    printf("\n  from right:");
    printf("\n      number of vehicles: %d", total_cars_r);
    printf("\n      average waiting time: %d", right_waiting / total_cars_r);
    printf("\n      maximum waiting time: %d", max_wait_right);
    printf("\n      clearance time: %d", right_clearance);

    printf("\n  from left:");
    printf("\n      number of vehicles: %d", total_cars_l);
    printf("\n      average waiting time: %d", left_waiting / total_cars_l);
    printf("\n      maximum waiting time: %d", max_wait_left);
    printf("\n      clearance time: %d\n", left_clearance); //This wasn't working, but it has to work now, running out of time :)
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 5) {
        //These lines here change the string to an integer slowly. I used a temp long value between.
        long right_arrival_rate_l = strtol(argv[1], NULL, 10);
        long r_max_time_l = strtol(argv[2], NULL, 10);
        long left_arrival_rate_l = strtol(argv[3], NULL, 10);
        long l_max_time_l = strtol(argv[4], NULL, 10);
        right_arrival_rate = right_arrival_rate_l;
        r_max_time = r_max_time_l;
        left_arrival_rate = left_arrival_rate_l;
        l_max_time = l_max_time_l;
        //printf("--------------------%ld %ld\n", right_arrival_rate_l,left_arrival_rate_l);
        //printf("\nBU ISTE:%d %d %d %d\n",right_arrival_rate,left_arrival_rate,r_max_time,l_max_time);

        srand(time(NULL)); //resets the random integer outputs coming from "rand()".

        if (r_time == 0 && l_time == 0) { //If there isn't a green light, our program does this to decide the side.
            if ((rand() & 1) == 1) {
                r_time = r_max_time; //Then equalises the time to max time,
                // so that the color can change without losing any timesteps between.
            } else {
                l_time = l_max_time; //Same for here.
            }
        }

        runOneSimulation();
    } else {
        printf("wrong number of parameters. Please give 4 of them.");//My only exception handling...
    }
    return 1;
}

