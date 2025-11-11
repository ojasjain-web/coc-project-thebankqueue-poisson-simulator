#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BANK_DAY 480  // 8 hours of bank hustle

// A customer in line
typedef struct Customer {
    int arrivedAt;
    struct Customer* next;
} Customer;

//  Poisson magic – how many people show up this minute
int getRandomArrivals(double lambda) {
    double L = exp(-lambda), p = 1.0;
    int count = 0;
    while (p > L) {
        p *= rand() / (RAND_MAX + 1.0);
        count++;
    }
    return count - 1;
}

//  Add a customer to the queue
void joinQueue(Customer** front, Customer** back, int time) {
    Customer* newPerson = malloc(sizeof(Customer));
    newPerson->arrivedAt = time;
    newPerson->next = NULL;
    if (*back) (*back)->next = newPerson;
    else *front = newPerson;
    *back = newPerson;
}

//  Serve the next customer
Customer* leaveQueue(Customer** front) {
    Customer* person = *front;
    if (person) *front = person->next;
    return person;
}

int main() {
    srand(time(NULL));
    double lambda;

    printf(" Welcome to Chill Bank Queue Sim!\n");
    printf(" How many people show up per minute (λ)? ");
    scanf("%lf", &lambda);

    Customer *queueFront = NULL, *queueBack = NULL;
    int waitTimes[10000], totalServed = 0, tellerFreeAt = 0;

    //  Simulate each minute of the bank day
    for (int minute = 0; minute < BANK_DAY; minute++) {
        int arrivals = getRandomArrivals(lambda);
        while (arrivals--) joinQueue(&queueFront, &queueBack, minute);

        if (tellerFreeAt <= minute && queueFront) {
            Customer* person = leaveQueue(&queueFront);
            waitTimes[totalServed++] = minute - person->arrivedAt;
            tellerFreeAt = minute + 2 + rand() % 2;  // Service time: 2–3 mins
            free(person);
        }
    }

    //  Analyze the wait times
    int totalWait = 0, longestWait = 0;
    for (int i = 0; i < totalServed; i++) {
        totalWait += waitTimes[i];
        if (waitTimes[i] > longestWait) longestWait = waitTimes[i];
    }

    double averageWait = (totalServed > 0) ? (double)totalWait / totalServed : 0;

    //  Final report
    printf("\n Bank closed! Here's how it went:\n");
    printf(" Total customers served: %d\n", totalServed);
    printf(" Average wait time: %.2f minutes\n", averageWait);
    printf(" Longest wait time: %d minutes\n", longestWait);

    //  Clean up the queue
    while (queueFront) free(leaveQueue(&queueFront));

    return 0;
}
