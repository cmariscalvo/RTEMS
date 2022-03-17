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

		// TODO: Simulate processing

		// TODO: Signal the event EVENT_HK_END

		// TODO: Wait until next execution

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

		// TODO: Simulate processing

		// TODO: Signal the event EVENT_ACS_END

		// TODO: Wait until next execution

	}
}


rtems_task Init(rtems_task_argument arg)
{

	// TODO: Create Logging Server

	// TODO: Start Logging Server

	// TODO: Create Housekeeping task

	// TODO: Start Housekeeping task

	// TODO: Create ACS task

	// TODO: Start ACS task

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}
