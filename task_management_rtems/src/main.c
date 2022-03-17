/*
 * Task Management RTEMS Project
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
			uint32_t __current_tick; \
			rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &__current_tick); \
			printf ("%lu: " fmt "\n", __current_tick, ##args); \
			} while (0)

#include <consume_ticks.h>

/**
 * First task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) prints a message: "T1 - START CYCLE" + number of ticks since boot.
 * 2) occupies the CPU for 100 ticks (i.e. one second).
 * 3) prints another message: "T1 -  END CYCLE" + number of ticks since boot.
 */

rtems_task first_task(rtems_task_argument argument)
{

	for (;;)
	{
		PRINT_TIME("T1 - START CYCLE");
		consume_ticks(100);
		PRINT_TIME("T1 - END_CYCLE");
	}
}

/**
 * Second task.
 *
 * This task must implement an infinite loop that does the following:
 *
 * 1) prints a message: "T2 - START CYCLE".
 * 2) occupies the CPU for 50 ticks (i.e. one second).
 * 3) prints another message: "T2 - END CYCLE".
 */

rtems_task second_task(rtems_task_argument argument)
{
	for (;;)
	{
		PRINT_TIME("T2 - START CYCLE");
		consume_ticks(50);
		PRINT_TIME("T2 - END_CYCLE");
	}
}


rtems_task Init(rtems_task_argument arg)
{
	rtems_id first_task_id;
	rtems_id second_task_id;

	// TODO: Create first task

	// TODO: Start first task

	// TODO: Create second task

	// TODO: Start second task

	rtems_task_create(rtems_build_name('T', 's', 'k', '1'),
	10, RTEMS_MINIMUM_STACK_SIZE,
	RTEMS_DEFAULT_MODES,
	RTEMS_DEFAULT_ATTRIBUTES, &first_task_id);
	rtems_task_start(first_task_id, first_task, 0);
	rtems_task_create(rtems_build_name('T', 's', 'k', '2'),
	20, RTEMS_MINIMUM_STACK_SIZE,
	RTEMS_DEFAULT_MODES,
	RTEMS_DEFAULT_ATTRIBUTES, &second_task_id);
	rtems_task_start(second_task_id, second_task, 0);

	/** Delete the initial task from the system */

	rtems_task_delete(RTEMS_SELF);

}
