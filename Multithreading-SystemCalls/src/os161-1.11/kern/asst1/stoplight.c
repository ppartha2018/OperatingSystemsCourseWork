/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20

static void gostraight(unsigned long cardirection,
                       unsigned long carnumber);
static void turnleft(unsigned long cardirection,
                     unsigned long carnumber);
static void turnright(unsigned long cardirection,
                     unsigned long carnumber);
typedef enum {
    RIGHT,
    STRAIGHT,
    LEFT
} direction;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} cardirection;

static struct {
    direction name;
    void (*func)(unsigned long cardirection, unsigned long carnumber);
} directiontable[] = {
    {RIGHT, turnright},
    {LEFT, turnleft},
    {STRAIGHT, gostraight}
};
static struct lock *NWlock;
static struct lock *SWlock;
static struct lock *NElock;
static struct lock *SElock;
int finished = 0;
/*
 *
 * Function Definitions
 *
 */


/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
    /*
     * Avoid unused variable warnings.
     */
    if(cardirection==NORTH){
        kprintf("Car# %lu Entering Intersection going straight(NW-SW) from NORTH %lu\n",carnumber,cardirection);
        
        two_lock_acquire(NWlock, SWlock,carnumber);
        
        kprintf("Car# %lu In Intersection going straight(NW-SW) from NORTH %lu\n",carnumber,cardirection);
        
        two_lock_release(NWlock, SWlock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going straight(NW-SW) from NORTH %lu\n",carnumber,cardirection);
    }else if(cardirection==EAST){
        kprintf("Car# %lu Entering Intersection going straight(NE-NW) from EAST %lu\n",carnumber,cardirection);
        
        two_lock_acquire(NElock, NWlock,carnumber);
        
        kprintf("Car# %lu In Intersection going straight(NE-NW) from EAST %lu\n",carnumber,cardirection);
        
        two_lock_release(NElock, NWlock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going straight(NE-NW) from EAST %lu\n",carnumber,cardirection);
    }else if(cardirection==SOUTH){
        kprintf("Car# %lu Entering Intersection going straight(SE-NE) from SOUTH %lu\n",carnumber,cardirection);
        
        two_lock_acquire(SElock, NElock,carnumber);
        kprintf("Car# %lu In Intersection going straight(SE-NE) from SOUTH %lu\n",carnumber,cardirection);
        
        two_lock_release(SElock, NElock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going straight(SE-NE) from SOUTH %lu\n",carnumber,cardirection);
    }else if(cardirection==WEST){
        kprintf("Car# %lu Entering Intersection going straight(SW-SE) from WEST %lu\n",carnumber,cardirection);
        two_lock_acquire(SWlock, SElock,carnumber);
        kprintf("Car# %lu In Intersection going straight(SW-SE) from WEST %lu\n",carnumber,cardirection);
        two_lock_release(SWlock, SElock,carnumber);
        kprintf("Car# %lu Leaving Intersection going straight(SW-SE) from WEST %lu\n",carnumber,cardirection);
    }
//        (void) cardirection;
//        (void) carnumber;
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
    if(cardirection==NORTH){
        kprintf("Car# %lu Entering Intersection going left(NW-SW-SE) from NORTH %lu\n",carnumber,cardirection);
        three_lock_acquire(NWlock, SWlock, SElock,carnumber);
        
        kprintf("Car# %lu In Intersection going left(NW-SW-SE) from NORTH %lu\n",carnumber,cardirection);
        
        three_lock_release(NWlock, SWlock, SElock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going left(NW-SW-SE) from NORTH %lu\n",carnumber,cardirection);
    }else if(cardirection==EAST){
        
        kprintf("Car# %lu Entering Intersection going left(NE-NW-SW) from EAST %lu\n",carnumber,cardirection);
        three_lock_acquire(NElock, NWlock, SWlock,carnumber);
        
        kprintf("Car# %lu In Intersection going left(NE-NW-SW) from EAST %lu\n",carnumber,cardirection);
        
        three_lock_release(NElock, NWlock, SWlock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going left(NE-NW-SW) from EAST %lu\n",carnumber,cardirection);
        
    }else if(cardirection==SOUTH){
        
        kprintf("Car# %lu Entering Intersection going left(SE-NE-NW) from SOUTH %lu\n",carnumber,cardirection);
        three_lock_acquire(SElock, NElock, NWlock,carnumber);
        
        kprintf("Car# %lu In Intersection going left(SE-NE-NW) from SOUTH %lu\n",carnumber,cardirection);
        
        three_lock_release(SElock, NElock, NWlock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going left(SE-NE-NW) from SOUTH %lu\n",carnumber,cardirection);
        
    }else if(cardirection==WEST){
        
        kprintf("Car# %lu Entering Intersection going left(SW-SE-NE) from WEST %lu\n",carnumber,cardirection);
        
        three_lock_acquire(SWlock, SElock, NElock,carnumber);
        
        kprintf("Car# %lu In Intersection going left(SW-SE-NE) from WEST %lu\n",carnumber,cardirection);
        
        three_lock_release(SWlock, SElock, NElock,carnumber);
        
        kprintf("Car# %lu Leaving Intersection going left(SW-SE-NE) from WEST %lu\n",carnumber,cardirection);
    }

//        (void) cardirection;
//        (void) carnumber;
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
    if(cardirection==NORTH){
        kprintf("Car# %lu Entering Intersection going right(NW) from NORTH %lu\n",carnumber,cardirection);
        one_lock_acquire(NWlock,carnumber);
        kprintf("Car# %lu In Intersection going right(NW) from NORTH %lu\n",carnumber,cardirection);
        one_lock_release(NWlock,carnumber);
        kprintf("Car# %lu Leaving Intersection going right(NW) from NORTH %lu\n",carnumber,cardirection);
    }else if(cardirection==EAST){
        kprintf("Car# %lu Entering Intersection going right(NE) from EAST %lu\n",carnumber,cardirection);
        one_lock_acquire(NElock,carnumber);
        kprintf("Car# %lu In Intersection going right(NE) from EAST %lu\n",carnumber,cardirection);
        one_lock_release(NElock,carnumber);
        kprintf("Car# %lu Leaving Intersection going right(NE) from EAST %lu\n",carnumber,cardirection);
    }else if(cardirection==SOUTH){
        kprintf("Car# %lu Entering Intersection going right(SE) from SOUTH %lu\n",carnumber,cardirection);
        one_lock_acquire(SElock,carnumber);
        kprintf("Car# %lu In Intersection going right(SE) from SOUTH %lu\n",carnumber,cardirection);
        one_lock_release(SElock,carnumber);
        kprintf("Car# %lu Leaving Intersection going right(SE) from SOUTH %lu\n",carnumber,cardirection);
    }else if(cardirection==WEST){
        kprintf("Car# %lu Entering Intersection going right(SW) from WEST %lu\n",carnumber,cardirection);
        one_lock_acquire(SWlock,carnumber);
        kprintf("Car# %lu In Intersection going right(SW) from WEST %lu\n",carnumber,cardirection);
        one_lock_release(SWlock,carnumber);
        kprintf("Car# %lu Leaving Intersection going right(SW) from WEST %lu\n",carnumber,cardirection);
    }
//        (void) cardirection;
//        (void) carnumber;
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
    cardirection cardirection;
    direction turndirection;
    int i;
    /*
     * Avoid unused variable and function warnings.
     */
    
    (void) unusedpointer;
    (void) carnumber;
    (void) gostraight;
    (void) turnleft;
    (void) turnright;
    
    /*
     * cardirection is set randomly.
     */
    
    cardirection = random() % 4;
    turndirection = random() % 3;
    i = 0;
    while (i < 3) {
        if(directiontable[i].name == turndirection){
//            kprintf("Car# %lu Entering Intersection from %d and want to go %d : \n",carnumber, cardirection, turndirection);
            directiontable[i].func(cardirection, carnumber);
//            kprintf("Car# %lu Leaving Intersection %d : entered from %d \n",carnumber, turndirection, cardirection);
        }
        i++;
    }
    int spl;
    spl=splhigh();
    finished++;
    splx(spl);
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;
    finished = 0;
        /*
         * Avoid unused variable warnings.
         */

//        (void) nargs;
//        (void) args;
        kprintf("In CreateCars()...");
        /*
         * Start NCARS approachintersection() threads.
         */
        //init locks
        if (NWlock==NULL) {
            kprintf("Creating NWlock\n");
            NWlock = lock_create("NWlock");
            if (NWlock == NULL) {
                kprintf("Couldn't NWlock\n");
                panic("signal: NWlock lock_create failed\n");
            }
        }
        if (NElock==NULL) {
            kprintf("Creating NElock\n");
            NElock = lock_create("NElock");
            if (NElock == NULL) {
                kprintf("Couldn't NElock\n");
                panic("signal: NElock lock_create failed\n");
            }
        }
        if (SWlock==NULL) {
            kprintf("Creating SWlock\n");
            SWlock = lock_create("SWlock");
            if (SWlock == NULL) {
                kprintf("Couldn't SWlock\n");
                panic("signal: SWlock lock_create failed\n");
            }
        }
        if (SElock==NULL) {
            kprintf("Creating SElock\n");
            SElock = lock_create("SElock");
            if (SElock == NULL) {
                kprintf("Couldn't SElock\n");
                panic("signal: SElock lock_create failed\n");
            }
        }
    kprintf("Creating cars...");
        for (index = 0; index < NCARS; index++) {
            
                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }
     kprintf("Exiting CreateCars()...");
    while(finished<NCARS);
    //Destroy locks
     lock_destroy(SWlock);
     lock_destroy(NWlock);
     lock_destroy(NElock);
     lock_destroy(SElock);
    SWlock = NULL;
    NWlock = NULL;
    NElock = NULL;
    SElock = NULL;
        return 0;
}
