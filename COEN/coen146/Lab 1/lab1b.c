//Daren Liu
//1/5/2021
//Lab 1 part 6 and 7
//Computes numbers in both circuit switching and packet switching scenario


#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

double factorial(double n)
{
    double total = 1;
    int i;

    for (i = 1; i <= n; i++)
    {
        total *= i;
    }
    return total;
}

int nCircuitSwitching (int linkBW, int userBW)
{
    return linkBW/userBW;
}

double a (double tPSusers)
{
    return tPSusers;
}

double b (double pPSusers)
{
    return (1.0 - pPSusers);
}

double c (double pPSusers, double nPSusers)
{
    return pow((1 - pPSusers), nPSusers - 1);
}

double d (double pPSusers, double pPSusersNotBusy, double nPSusers)
{
    return (pPSusers * pow(pPSusersNotBusy, (nPSusers - 1)));
}

double e (double nPSusers, double pPSusers, double pPSusersNotBusy)
{
    return nPSusers * (pPSusers) * pow(pPSusersNotBusy, nPSusers - 1);
}

double f (double pPSusers, double pPSusersNotBusy, double nPSusers)
{
    return pow(pPSusers, 10) * pow((pPSusersNotBusy), (nPSusers - 10));
}

double g (double pPSusers, double nPSusers, double pPSusersNotBusy)
{
    double first = factorial(nPSusers)/ (factorial(10) * factorial(nPSusers - 10));
    return first * pow(pPSusers, 10) * pow(pPSusersNotBusy, nPSusers - 10);
}

double h(double nPSusers, double pPSusers, double pPSusersNotBusy)
{
    
    double total = 0;
    int i;
    for (i = 11; i <= nPSusers; i++)
    {
        double first = factorial(nPSusers) / (factorial(i) * factorial(nPSusers - i));
        total += (first * pow(pPSusers, i) * pow(pPSusersNotBusy, nPSusers - i));
    }
    return total;
}

int main()
{
    int linkBandwidth = 200;
    int userBandwidth = 20;
    double tPSusers = 0.10;
    double nPSusers = 19;

    int nCSusers = nCircuitSwitching(linkBandwidth, userBandwidth);

    double pPSusers = a(tPSusers);
    double pPSusersNotBusy = b(pPSusers);
    double cAns = c(pPSusers, nPSusers);
    double dAns = d(pPSusers, pPSusersNotBusy, nPSusers);
    double eAns = e(nPSusers, pPSusers, pPSusersNotBusy);
    double fAns = f(pPSusers, pPSusersNotBusy, nPSusers);
    double gAns = g(pPSusers, nPSusers, pPSusersNotBusy);
    double hAns = h(nPSusers, pPSusers, pPSusersNotBusy);

    printf("The number of circuit-switched users that can be supported is %d. \n \n", nCSusers);

    printf("The probability that one specific other user is not busy is %f. \n", pPSusers);
    printf("The probability that all of the other specific other users are not busy is %f. \n", pPSusersNotBusy);
    printf("The probability that all of the other specific other users are not busy is %f. \n", cAns);
    printf("The probability that one specific user is transmitting and the remaining users are not transmitting is %f. \n", dAns);
    printf("The probability that exactly one (anyone) of thenPSusersusers is busy is %f. \n", eAns);
    printf("The probability that 10 specific users of nPSusersare transmitting and the others are idle is %f. \n", fAns);
    printf("The probability that any 10 users of nPSusersare transmitting and the others are idle is %f. \n", gAns);
    printf("The probability that more than 10 users of nPSusersare transmitting and the others are idle is %f. \n", hAns);
    return 0;
}