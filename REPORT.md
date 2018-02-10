#General Information
*ECS150 Project 2:* User-level thread library
*Authors:* Kevin Chung, Jisoo Yun

###Phase 1
We implemented a queue using a struct that contains a *front*, an *end*, of 
type *int* as well as a *length* attribute and *array* with a maximum size 
of 1024. The queue uses the *front* and *end* integers as indices for the 
array. Each time some data is enqueued or dequeued, the counter shifts over
by incrementing *front* or *end*. Then data is inserted or removed from the
proper index position in the array. Afterwards, the length attribute is changed
as necessary. *Create* initializes the struct data members and *destroy* frees
the memory of each index in the array. Overtime, we added some additional 
attributes to the *struct queue* in order to solve some issues we had in Phase
2 and 3 with the *iterator* function. In order to accomodate the possibility of
two threads having the same TID with our previous iterator implementation, we
attempted to solve this issue with two arrays that distinguish a used TID vs
an unused TID and making sure that when iterating we check for any redundancies
for the TIDs. 

###Phase 2
We defined the *TCB* as a struct that contains TID, context, and booleans to 
represent the various thread states (ready, running, block, zombie). We created
a function for initializing the main thread with all of the respective initial
values in which only ready and running are true and the rest are false. If the
thread being created isn't the main thread then we allocate a new *TCB struct*
and then pass the new struct object into our function that initializes the TCB.
The *uthread_self* function just returns the TID of the currently running 
thread. For the *uthread_yield* function we first check if the main thread is
ready and unblocked. If either of those are untrue then we iterate through our
queue of threads and apply our *find_next_ready* function which checks the 
*TCB* attributes to have the ready state as true and exited and blocked as 
false. 

###Phase 3
We first iterate to find the TID in the queue of threads to do some 
preliminary error checking. Then we check if a thread has exited, and if it has
then we delete the *TCB* in the thread queue. If the thread is ready, then we 
block it and set the joined status to true and then called *uthread_yield* 
before we delete the thread data from the queue. 


###Phase 4
We didn't get to test the preemption since the test file was created at the end
so it may or may not work. Preemption started with creating a signal handler 
function that calls *yield* and then assigning the respective values for the 
*sigaction struct* attributes. Then using attributes of the *itimerval struct*, 
we set the intervals for the timer, and set the timer using *setitimer()*. 
Enabling preempt was as simple as calling *sigprocmask* which we used to 
unblock the old set. For disabling, we had to create a new set and call 
*sigprocmask* to block the set of signals. As to whether any of this works, is
a mystery even to us though it theoretically should since all of the resources
we researched had clear examples of how these signals should be handled and we
analyzed the process step by step.

##Sources
<https://www.ibm.com/support/knowledgecenter/ssw_i5_54/apis/sigaset.htm>
<https://www.ibm.com/support/knowledgecenter/ssw_i5_54/apis/sigeset.htm>
<https://www.ibm.com/support/knowledgecenter/en/ssw_i5_54/apis/sigpmsk.htm>
<https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/rstimr.htm>
<https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html#Elapsed-Time>
<http://www.informit.com/articles/article.aspx?p=23618&seqNum=14>
<http://pubs.opengroup.org/onlinepubs/009604499/functions/sigaction.html>
<http://man7.org/linux/man-pages/man2/sigaction.2.html>
<https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html>
<https://www.gnu.org/software/libc/manual/html_mono/libc.html#Setting-an-Alarm>
<https://www.gnu.org/software/libc/manual/html_mono/libc.html#Signal-Actions>
<https://www.gnu.org/software/libc/manual/html_mono/libc.html#Blocking-Signals>
<https://stackoverflow.com/questions/39047190/creating-a-queue-with-structs-in-c>

