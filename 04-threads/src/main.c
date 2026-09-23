/**
 * @file main.c
 * @brief Zephyr RTOS multi-threading example with message queues and mutexes.
 *
 * This file implements a main thread that reads from a message queue, along 
 * with two additional threads that handle shared global data safely using a 
 * mutex and push data into the message queue.
 */

/* Zephyr includes */
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/** 
 * @brief Register the logging module for the main application. 
 */
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @brief Base sleep time in milliseconds.
 * 
 * 1000 msec = 1 sec
 */
#define SLEEP_TIME_MS   1000

/* Thread function prototypes */
void thread_1(void);
void thread_2(void);

/**
 * @brief Statically define and initialize thread 1.
 * 
 * Stack size is 2048 bytes, priority is 10, and it starts after a 3000ms delay.
 */
K_THREAD_DEFINE(thread0_id, 2048, thread_1, NULL, NULL, NULL, 10, 0, 3000);

/**
 * @brief Statically define and initialize thread 2.
 * 
 * Stack size is 2048 bytes, priority is 10, and it starts after a 3000ms delay.
 */
K_THREAD_DEFINE(thread1_id, 2048, thread_2, NULL, NULL, NULL, 10, 0, 3000);

/**
 * @brief Define a message queue holding up to 10 integers.
 */
K_MSGQ_DEFINE(my_msgq, sizeof(int), 10, 1);

/**
 * @brief Structure representing shared sensor data.
 */
typedef struct sensor_data {
    uint32_t field1; /**< First data field */
    uint32_t field2; /**< Second data field */
} sensor_data_t;

/**
 * @brief Global structure instance shared between threads.
 */
sensor_data_t my_global_struct = {0};

/**
 * @brief Mutex to protect read/write access to my_global_struct.
 */
K_MUTEX_DEFINE(global_data_mutex);

/**
 * @brief Main application entry point.
 *
 * Runs an infinite loop that waits indefinitely to consume integer messages 
 * from the queue and logs them.
 *
 * @return Returns 0 upon completion (runs indefinitely in practice).
 */
int main(void)
{
	int data;

	LOG_INF("Main thread, waiting for queue messages.");

	while (1) {
		 while (1) {
        	/* Get a data item */
        	k_msgq_get(&my_msgq, &data, K_FOREVER);

       		/* Process data item */
			LOG_INF("Main received %d.", data);
    	}
	}	

	return 0;
}

/**
 * @brief Routine for thread 1.
 *
 * Runs an infinite loop that locks the global data mutex, safely increments 
 * the fields inside the global structure, unlocks the mutex, and then sleeps.
 */
void thread_1(void)
{
	while (1){
		/* Lock mutex to write global data shared by threads*/
		k_mutex_lock(&global_data_mutex, K_FOREVER);
		my_global_struct.field1++;
		my_global_struct.field2++;
		k_mutex_unlock(&global_data_mutex);
		
		LOG_INF("Hello from thread_1!");	
		k_msleep(4*SLEEP_TIME_MS);
	}
}

/**
 * @brief Routine for thread 2.
 *
 * Runs an infinite loop that increments a local counter and enqueues it 
 * to the message queue. If the queue is full, it purges the old data 
 * before retrying.
 */
void thread_2(void)
{
	int my_counter = 0;

	while (1){

		LOG_INF("Hello from thread_2, enqueue %d.", my_counter);	

		/* Send data to consumers */
        while (k_msgq_put(&my_msgq, &my_counter, K_NO_WAIT) != 0) {
            /* Message queue is full: purge old data & try again */
            k_msgq_purge(&my_msgq);
        }

		my_counter++;

		k_msleep(SLEEP_TIME_MS);
	}
}