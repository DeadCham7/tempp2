#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct itimerval timee;
struct sigaction sigact;
struct sigaction oldsigact;
sigset_t n_set;
sigset_t o_set;

void sig_handler(int signal)
{
	//force thread to yield
	uthread_yield();
}

void preempt_disable(void)
{
	//Create an empty set
	sigemptyset(&n_set);
	//Add signal to set of signals
	sigaddset(&n_set, SIGVTALRM);
	//Block set of signals in n_set during critical section
	sigprocmask(SIG_BLOCK, &n_set, &o_set);
}

void preempt_enable(void)
{
	//unblock SIGVTALRM signals and resume old set
	sigprocmask(SIG_SETMASK, &o_set, NULL);
}

void preempt_start(void)
{
	//STEP 1: Install a signal handler that receives alarm signals
	sigact.sa_handler = sig_handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	//STEP 2: Configure timer which will fire an alarm a hundred times per sec
	sigaction(SIGVTALRM, &sigact, NULL); //Use SIGVTALRM
	//Set intervals here
	timee.it_value.tv_sec = 0;
	timee.it_value.tv_usec = 1000000;
	timee.it_value.tv_usec = timee.it_value.tv_usec / HZ;
	timee.it_interval.tv_sec = 0;
	timee.it_interval.tv_usec = 1000000;
	timee.it_interval.tv_usec = timee.it_interval.tv_usec / HZ;
	//Sets timer specified by which according to new use ITIMER_VIRTUAL
	setitimer(ITIMER_VIRTUAL, &timee, NULL);
}

