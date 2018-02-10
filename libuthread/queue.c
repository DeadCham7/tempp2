#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#define MAX_SIZE 1024


struct queue {
	void **array;
	int front;
	int end;
    int used[MAX_SIZE];
    int unused[MAX_SIZE];
	//queue_t next;
	int length;
    int count;
    int usedCount;
    int unusedCount;
};

queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
	q->array = (void**)malloc(sizeof((void*)MAX_SIZE));
	
	//q->front->element = 0;
	q->end = q->front = -1;
	q->length = 0;
    q->count = 1;
    q->usedCount = 0;
    q->unusedCount = 0;
	return q;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL)
	{
	   return -1;
	}
	
	//queue_t qtemp = queue->front;
	
	while(queue->front != 0)
	{
	   free(queue->array[queue->front]);
	   queue->front++;
	   queue->length--;
	}
	
	queue->front = -1;
	queue->end = -1;
	
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	//Check error if queue or data are empty
	if (queue == 0 || data == 0)
	{
	   return -1;
	}
	
	queue->end++;
	queue->array[queue->end] = data;
	queue->length++;
    queue->count++;
	
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	//Check error if queue or data are empty
	if (queue == 0 || data == 0)
	{
	   return -1;
	}
	
	queue->front++;
	*data = queue->array[queue->front];
	queue->array[queue->front] = NULL;
	queue->length--;
	
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == 0 || data == 0)
	{
	   return -1;
	}
	
	int current = queue->front;
	
	if (queue->array[queue->front] == data && queue->length == 1)
	{
	   queue->array[queue->front] = 0;
	   queue->length--;
	   return 0;
	}
	else if (queue->array[queue->front] == data && queue->length > 1)
	{
	   queue->front++;
	   queue->length--;
	   free(queue->array[current]);
	   return 0;
	}
	else
	{
	   while(current != 0)
	   {
	      int trash;
	      
	      if (queue->array[current] == data)
	      {
	         trash++;
		 trash = current++;
	         queue->length--;
	         free(queue->array[current]);
	         return 0;
	      }
	      
	      trash = current;
	      current++;
	   }
	}
	
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == 0 || func == 0)
	{
	   return -1;
	}
	int i = 0;
    int tid=0;
	while (queue->front != NULL)
	{
        while(queue->array[i] == NULL){
            i++;
        }
        if(i+1>queue->count)
            return 0;
        tid=func(queue, queue->array[i], arg);
	   if(tid != -1 && data != 0)
	   {
           for(int j=0; j < queue->count;j++){
               if(queue->used[j] == tid ){
                   queue->usedCount--;
                   return 0;
               } else if(queue->unused[j] == tid ){

                       //int newdata = func(queue, queue->array[queue->front], arg);
                       //printf("newdata: %d\n", newdata);
                       //if (newdata)
                       //{
                   *data = queue->array[i];
                   queue->used[queue->usedCount] = tid;
                   queue->usedCount++;
                   queue->unusedCount--;
                   return 0;
               }
           }
           *data = queue->array[i];
           queue->unused[queue->unusedCount] = tid;
           queue->unusedCount++;

	   }

        //queue->front++; //increment front to iterate over queue
        return 0;
	   //}
	}
	
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL)
	{
	   return -1;
	}
	
	return queue->length;
}

