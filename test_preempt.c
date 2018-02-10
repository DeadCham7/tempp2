#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>
#include <preempt.h>

//Thread
int thread()
{
	preempt_start();
	
	do
	{
	   printf("current thread: %d\n", uthread_self());
	}
	while(0);

	return 0;
}

int main()
{
	int threadNum;
	int temp1, temp2;
	int choice;
	
	//print out prompt for number of threads
	printf("How many threads? ");
	temp1 = scanf("%d", &threadNum);
	
	//Create a bunch of threads
	for(int threadCnt = 0; threadCnt < temp1; threadCnt++)
	{
	   uthread_create(thread, NULL);
	}
	
	//print out prompt for choosing preempt or non-preempt
	printf("Preempt?(Type 1 or 2, no other answers accepted) \n1)Enable 2)Disable");
	temp2 = scanf("%d", &choice);
	
	//preempt enable or disable
	do
	{
	   if(temp2 == 1)
	   {
	      preempt_enable();
	      printf("preempt enabled.\n");
	   }
	   else if (temp2 == 2)
	   {
	      preempt_disable();
	      printf("preempt disabled.\n");
	   }
	   else
	   {
	      printf("Preempt?(Type 1 or 2, no other answers accepted) \n1)Enable 2)Disable");
	      temp2 = scanf("%d", &choice);
	   }
	}while(temp2 != 1 && temp2 != 2);
	
	//Output running thread
	while(1)
	{
	   printf("Running the main thread\n");
	}
	
	return 0;
}
