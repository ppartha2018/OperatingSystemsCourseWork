/*
 * Scheduler.
 *
 * The default scheduler is very simple, just a round-robin run queue.
 * You'll want to improve it.
 */

#include <types.h>
#include <lib.h>
#include <scheduler.h>
#include <thread.h>
#include <machine/spl.h>
#include <queue.h>
#include <clock.h>
/*
 *  Scheduler data
 */

// Queue of runnable threads
static struct queue *runqueue;
static struct queue *mlf_runqueue; //HZ = 2X
static struct thread *thread_executed;
int counter;
/*
 * Setup function
 */
void
scheduler_bootstrap(void)
{
    counter = 0;
    thread_executed = NULL;
    runqueue = q_create(32);
    if (runqueue == NULL) {
        panic("scheduler: Could not create run queue\n");
    }
    mlf_runqueue = q_create(32);
    if (mlf_runqueue == NULL) {
        panic("scheduler: Could not create mlf run queue\n");
    }
}

/*
 * Ensure space for handling at least NTHREADS threads.
 * This is done only to ensure that make_runnable() does not fail -
 * if you change the scheduler to not require space outside the
 * thread structure, for instance, this function can reasonably
 * do nothing.
 */
int
scheduler_preallocate(int nthreads)
{
    assert(curspl>0);
    if(q_preallocate(runqueue, nthreads) == 1)
        return 1;
    return q_preallocate(mlf_runqueue, nthreads);
}

/*
 * This is called during panic shutdown to dispose of threads other
 * than the one invoking panic. We drop them on the floor instead of
 * cleaning them up properly; since we're about to go down it doesn't
 * really matter, and freeing everything might cause further panics.
 */
void
scheduler_killall(void)
{
    assert(curspl>0);
    while (!q_empty(runqueue)) {
        struct thread *t = q_remhead(runqueue);
        kprintf("scheduler: Dropping thread %s.\n", t->t_name);
    }
    while (!q_empty(mlf_runqueue)) {
        struct thread *t = q_remhead(mlf_runqueue);
        kprintf("scheduler: Dropping thread %s.\n", t->t_name);
    }
}

/*
 * Cleanup function.
 *
 * The queue objects to being destroyed if it's got stuff in it.
 * Use scheduler_killall to make sure this is the case. During
 * ordinary shutdown, normally it should be.
 */
void
scheduler_shutdown(void)
{
    scheduler_killall();

    assert(curspl>0);
    q_destroy(runqueue);
    q_destroy(mlf_runqueue);
    runqueue = NULL;
    mlf_runqueue = NULL;
    thread_executed = NULL;
}

/*
 * Actual scheduler. Returns the next thread to run.  Calls cpu_idle()
 * if there's nothing ready. (Note: cpu_idle must be called in a loop
 * until something's ready - it doesn't know whether the things that
 * wake it up are going to make a thread runnable or not.)
 */
struct thread *
scheduler(void)
{
    // meant to be called with interrupts off
    assert(curspl>0);

    while (q_empty(runqueue) && q_empty(mlf_runqueue)) {
        //        kprintf("CPU busy\n");
        cpu_idle();
    }

    // You can actually uncomment this to see what the scheduler's
    // doing - even this deep inside thread code, the console
    // still works. However, the amount of text printed is
    // prohibitive.
    //
    //print_run_queue();
    if(!q_empty(runqueue)){
        struct thread *t = q_remhead(runqueue);
        DEBUG(DB_VM, "RUN #1 %s c= %d \n", t->t_name, counter);
        t->priority = 1; //transfer it to mlfqueue if not done executing
        thread_executed = NULL;
        counter = 0;
        return t;
    } else {
        if(counter == 2){
            struct thread *t = q_remhead(mlf_runqueue);
            DEBUG(DB_VM, "MLF #1 %s c= %d \n", t->t_name, counter);
            thread_executed = t;
            counter=0;
            return t;
        }else{
            if(counter != 0 && q_peektail(mlf_runqueue) == thread_executed){
                //same thread as last one
                //schedule it for double quantum time
                counter++;
                struct thread *t = q_remtail(mlf_runqueue);
                DEBUG(DB_VM, "MLF #2 %s c= %d \n", t->t_name, counter);
                return t;
            }else{
                //thread might be done, or goes into wait/sleep state,
                //so it is not in mlf queue
                //Get new thread to execute
                struct thread *t = q_remhead(mlf_runqueue);
                DEBUG(DB_VM, "MLF #3 %s c= %d \n", t->t_name, counter);
                thread_executed = t;
                counter = 1;
                return t;
            }
        }
    }
}

/*
 * Make a thread runnable.
 * With the base scheduler, just add it to the end of the run queue.
 */
int
make_runnable(struct thread *t)
{
    // meant to be called with interrupts off
    assert(curspl>0);

    if(t->priority == 0){
        DEBUG(DB_VM, "runnable runqueue %s\n", t->t_name);
        return q_addtail(runqueue, t);
    } else {
        DEBUG(DB_VM, "runnable mlf_runqueue %s\n", t->t_name);
        return q_addtail(mlf_runqueue, t);
    }
}

/*
 * Debugging function to dump the run queue.
 */
void
print_run_queue(void)
{
    /* Turn interrupts off so the whole list prints atomically. */
    int spl = splhigh();

    int i,k=0;
    i = q_getstart(runqueue);

    while (i!=q_getend(runqueue)) {
        struct thread *t = q_getguy(runqueue, i);
        kprintf("  %2d: %s %p\n", k, t->t_name, t->t_sleepaddr);
        i=(i+1)%q_getsize(runqueue);
        k++;
    }

    splx(spl);
}
