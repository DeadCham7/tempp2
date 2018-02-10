#include <stdio.h>
#include "queue.h"
#include <assert.h>
#include <stdint.h>


static int inc_item(queue_t q, void *data, void *arg)
{
	int *a = (int*) data;
	int inc = (intptr_t) arg;
	
	*a += inc;
	
	return 0;
} //phase 1 increment hint function, increments to items of queue 

void test_iterator(void)
{	   
	queue_t q;	   
	int data2[] = {1, 2, 3, 4, 5};
	int i;
	int *ptr;
	   
	//initialize queue and enqueue items
	q = queue_create();
	for (i = 0; i < 5; i++)
	{
	   queue_enqueue(q, &data2[i]);
	}
	
	//check length after enqueue
	int qlen3;
	qlen3 = queue_length(q);
	printf("This is the new current length of the queue: %d", qlen3);
	
	//iterate and add 1 to every item in queue
	queue_iterate(q, inc_item, (void*)1, NULL);
	
	//initialize pointer to print out new incremented data from queue
	void *ddata2;
	
	printf("dequeued new values:");
	for(int k = qlen3; k > 0; k--)
	{
	   queue_dequeue(q, &ddata2);
	   printf(" %d", (*(int*)ddata2));
	}
	printf("\n");
}

int main()
{
	queue_t newqueue;
	
	assert(queue_destroy(NULL) == -1);
	assert(queue_enqueue(NULL,NULL) == -1);
	
	newqueue = queue_create();
	
	assert(queue_length(newqueue) == 0);
	
	int qlen;
	
	qlen = queue_length(newqueue);
	printf("This is the queue initial length: %d\n", qlen);	
	
	int data[] = {4, 3, 2, 1};
	int qlen2;
	
	//Check for length and enqueue
	for (int i = 0; i < 4; i++)
	{
	   queue_enqueue(newqueue, &data[i]);
	   qlen2 = queue_length(newqueue);
	   printf("This is the queue length: %d\n", qlen2);
	}
	
	void *ddata;
	
	//Check for dequeue and print out data from queue
	for (int j = qlen2; j > 0; j--)
	{
	   queue_dequeue(newqueue, &ddata);
	   printf(" %d", (*(int*)ddata));
	}
	printf("\n");

	test_iterator();

	return 0;	
}
