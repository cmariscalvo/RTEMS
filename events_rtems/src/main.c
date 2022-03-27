/*
 * Event Server RTEMS Project
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

/** Events to be logged */
#define EVENT_ACS_START 	RTEMS_EVENT_0
#define EVENT_ACS_END 		RTEMS_EVENT_1
#define EVENT_HK_START 		RTEMS_EVENT_2
#define EVENT_HK_END		RTEMS_EVENT_3

/** Logging Server Task ID */
rtems_id logging_server_id;

/** Housekeeping Task ID */
rtems_id housekeeping_task_id;

/** ACS Task ID */
rtems_id acs_task_id;

/**
 * Logging server task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) waits for the occurrence of an event.
 * 2) prints the time and the event type on screen, emulating the log
 */
rtems_task logging_server(rtems_task_argument argument)
{
	rtems_event_set event_out;

	for (;;)
	{

		// TODO: Wait for any of the events defined
		//	ESPERA BLOQUEANTE HASTA QUE LLEGUE ALGUN EVENTO
		rtems_event_receive(EVENT_ACS_START | EVENT_ACS_END | EVENT_HK_START | EVENT_HK_END,
				RTEMS_WAIT|RTEMS_EVENT_ANY, RTEMS_NO_TIMEOUT, &event_out ) ;

		//VERDADERO CUANTO EVENT_OUT TIENE 1 EN LA MISMA POSICION QUE HOUSE_KEEPING START
		if (event_out & EVENT_HK_START){
			PRINT_TIME("EVENT_HK_START");

		} ;
		if (event_out & EVENT_HK_END){

			PRINT_TIME("EVENT_HK_END");
		} ;

		if (event_out & EVENT_ACS_START){
					PRINT_TIME("EVENT_ACS_START");

				} ;
		if (event_out & EVENT_ACS_END){

					PRINT_TIME("EVENT_ACS_END");
				} ;

		// TODO: When an event takes place, print time and event type

	}
}

/**
 * Housekeeping demo task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) Signals the EVENT_HK_START to the logging server
 * 2) occupies the CPU for 2 ticks
 * 3) Signals the EVENT_HK_END to the logging server
 * 4) sleeps for 10 ticks
 */
rtems_task housekeeping_task(rtems_task_argument argument)
{

	for (;;)
	{
		// TODO: Signal the event EVENT_HK_START
		rtems_event_send(logging_server_id, EVENT_HK_START);
		// TODO: Simulate processing
		consume_ticks(2);
		// TODO: Signal the event EVENT_HK_END
		rtems_event_send(logging_server_id, EVENT_HK_END) ;
		// TODO: Wait until next execution
		rtems_task_wake_after(10);

	}
}

/**
 * ACS demo task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) Signals the EVENT_ACS_START to the logging server
 * 2) occupies the CPU for 40 ticks
 * 3) Signals the EVENT_ACS_END to the logging server
 * 3) sleeps for 100 ticks
 */
rtems_task acs_task(rtems_task_argument argument)
{
	for (;;)
	{
		// TODO: Signal the event EVENT_ACS_START
		rtems_event_send(logging_server_id, EVENT_ACS_START);

		// TODO: Simulate processing
		consume_ticks(40);

		// TODO: Signal the event EVENT_ACS_END
		rtems_event_send(logging_server_id, EVENT_ACS_END);

		// TODO: Wait until next execution
		rtems_task_wake_after(100);

	}
}


rtems_task Init(rtems_task_argument arg)
{
	// TODO: Create Logging Server
		rtems_task_create(rtems_build_name('T', 'S', 'K', '1'),
		10, RTEMS_MINIMUM_STACK_SIZE,
		RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
		RTEMS_DEFAULT_ATTRIBUTES, &logging_server_id);

	// TODO: Start Logging Server
		rtems_task_start(logging_server_id, logging_server, 0);

	// TODO: Create Housekeeping task
		rtems_task_create(rtems_build_name('T', 'S', 'K', '2'),
		10, RTEMS_MINIMUM_STACK_SIZE,
		RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
		RTEMS_DEFAULT_ATTRIBUTES, &housekeeping_task_id);

	// TODO: Start Housekeeping task
		rtems_task_start(housekeeping_task_id, housekeeping_task, 0);

	// TODO: Create ACS task
		rtems_task_create(rtems_build_name('T', 'S', 'K', '3'),
		10, RTEMS_MINIMUM_STACK_SIZE,
		RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
		RTEMS_DEFAULT_ATTRIBUTES, &acs_task_id);

	// TODO: Start ACS task
		rtems_task_start(acs_task_id, acs_task, 0);

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}
