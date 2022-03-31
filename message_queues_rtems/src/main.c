/*
 * Message Queue Server RTEMS Project
 * Copyright (C) 2017-2020 University of Alcalá
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <rtems.h>

#include <rtems_config.h>

/** Luckily, we have the libc! :) */
#include <stdio.h>
#include <stdlib.h>

#define PRINT(fmt,args...)  printf ( fmt "\n",\
                            ##args)

#define PRINT_TIME(fmt,args...) do { \
			unsigned int __current_tick; \
			rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &__current_tick); \
			printf ("%lu: " fmt "\n", __current_tick, ##args); \
			} while (0)

#include <consume_ticks.h>

typedef struct {

	/** Sender Task ID */
	rtems_id sender_id;
	/** Telemetry Frame Counter */
	unsigned int counter;
	/** Data size */
	unsigned int data_size;
	/** Data buffer */
	char data[20];

} telemetry_t;

/** The one and only message queue */
rtems_id tm_message_queue;

/** Telemetry Server Task ID */
rtems_id telemetry_server_id;

/** Houskeeping Task ID */
rtems_id housekeeping_task_id;

/** ACS Task ID */
rtems_id acs_task_id;

/**
 * Telemetry downlink server demo task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) waits for an incoming message from the rest of the tasks.
 * 2) when a message arrives, it formats the message and "sends: it
 *    (i.e. it prints it).
 * 3) occupies the CPU for 1 tick
 */

rtems_task telemetry_server(rtems_task_argument argument)
{
	telemetry_t tm;
	size_t size;

	for (;;)
	{
		// TODO: Wait for a message
		rtems_message_queue_receive(tm_message_queue, &tm, &size, RTEMS_WAIT, RTEMS_NO_TIMEOUT) ;

		// TODO: Simulate processing

		PRINT_TIME("Sent TM => %s | %u | %s",
				((tm.sender_id == housekeeping_task_id) ?
				"HK" : "ACS"),
				tm.data_size,
				tm.data);
		consume_ticks(1);

	}
}

/**
 * Houskeeping demo task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) occupies the CPU for 2 ticks
 * 2) sends a telemetry
 * 3) sleeps for 10 ticks
 */

rtems_task housekeeping_task(rtems_task_argument argument)
{
	telemetry_t tm;
	tm.sender_id = housekeeping_task_id;
	tm.counter = 0;

	for (;;)
	{
		PRINT_TIME("HK Activation");
		consume_ticks(2) ;

		// TODO: Simulate processing

		// Fill the telemetry packet
		strcpy(tm.data, "SYSTEM OK");
		tm.data_size = strlen(tm.data);
		tm.counter++;

		// TODO: Send the message
		rtems_message_queue_send(tm_message_queue, &tm, sizeof(tm)) ;

		// TODO: Wait until next execution
		rtems_task_wake_after(10) ;

	}
}

/**
 * ACS demo task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) occupies the CPU for 40 ticks
 * 2) sends a telemetry
 * 3) sleeps for 100 ticks
 */
rtems_task acs_task(rtems_task_argument argument)
{
	telemetry_t tm;
	tm.sender_id = acs_task_id;
	tm.counter = 0;

	for (;;)
	{
		PRINT_TIME("ACS Activation");

		// TODO: Simulate processing
		consume_ticks(40);

		// Fill the telemetry packet
		strcpy(tm.data, "ACS OK");
		tm.data_size = strlen(tm.data);
		tm.counter++;

		// TODO: Send the message
		rtems_message_queue_send(tm_message_queue, &tm, sizeof(tm)) ;

		rtems_task_wake_after(100) ;
		// TODO: Wait until next execution

	}
}


rtems_task Init(rtems_task_argument arg)
{

	// TODO: Create the message queue
	rtems_message_queue_create(rtems_build_name('A', 'B', 'C', 'D'), 128, 64, RTEMS_FIFO, &tm_message_queue) ;

	// TODO: Create Telemetry Server
	rtems_task_create(rtems_build_name('T', 'S', 'K', '1'),
			10, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &telemetry_server_id);

		// TODO: Start Logging Server
	rtems_task_start(telemetry_server_id, telemetry_server, 0);
	// TODO: Start Telemetry Server

	// TODO: Create Housekeeping task
	rtems_task_create(rtems_build_name('T', 'S', 'K', '2'),
				10, RTEMS_MINIMUM_STACK_SIZE,
				RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
				RTEMS_DEFAULT_ATTRIBUTES, &housekeeping_task_id);

			// TODO: Start Logging Server
	rtems_task_start(housekeeping_task_id, housekeeping_task, 0);

	// TODO: Start Housekeeping task

	// TODO: Create ACS task
	rtems_task_create(rtems_build_name('T', 'S', 'K', '3'),
					10, RTEMS_MINIMUM_STACK_SIZE,
					RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
					RTEMS_DEFAULT_ATTRIBUTES, &acs_task_id);

				// TODO: Start Logging Server
	rtems_task_start(acs_task_id, acs_task, 0);

	// TODO: Start ACS task

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}
