/*
 * Blocking Semaphores RTEMS Project
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

/** The one and only semaphore */
rtems_id critical_section_sem;


rtems_task T1(rtems_task_argument argument)
{

	// TODO: Implement task's behaviour
	rtems_task_wake_after(8);
	consume_ticks(4);
	PRINT_TIME("OBTAINING SEMAPHORE T1") ;
	rtems_semaphore_obtain(critical_section_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT) ;
	consume_ticks(3);
	PRINT_TIME("RELEASING SEMAPHORE T1") ;
	rtems_semaphore_release(critical_section_sem);
	consume_ticks(2);
	PRINT_TIME("FINISHING T1") ;


	rtems_task_delete(RTEMS_SELF);
}

rtems_task T2(rtems_task_argument argument)
{

	// TODO: Implement task's behaviour
	rtems_task_wake_after(12);
	consume_ticks(7) ;
	rtems_task_delete(RTEMS_SELF);
}

rtems_task T3(rtems_task_argument argument)
{

	// TODO: Implement task's behaviour
	consume_ticks(0);
	PRINT_TIME("OBTAINING SEMAPHORE T3") ;
	rtems_semaphore_obtain(critical_section_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT) ;
	consume_ticks(10);
	PRINT_TIME("RELEASING SEMAPHORE T3") ;
	rtems_semaphore_release(critical_section_sem);
	consume_ticks(4);
	PRINT_TIME("FINISHING SEMAPHORE T3") ;
	rtems_task_delete(RTEMS_SELF);
}


rtems_task Init(rtems_task_argument arg)
{
	rtems_id T1_id;
	rtems_id T2_id;
	rtems_id T3_id;

	// TODO: Create the semaphore
	rtems_semaphore_create(rtems_build_name('S','e','m','1'),1,RTEMS_BINARY_SEMAPHORE, 0, &critical_section_sem);

	// Create T1
	rtems_task_create(rtems_build_name('T', 'S', 'K', '1'),
			10, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T1_id);

	// Start T1 Server
	rtems_task_start(T1_id, T1, 0);

	// Create T2 task
	rtems_task_create(rtems_build_name('T', 'S', 'K', '2'),
			15, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T2_id);

	// Start T2 task
	rtems_task_start(T2_id, T2, 0);

	// Create T3 task
	rtems_task_create(rtems_build_name('T', 'S', 'K', '3'),
			20, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T3_id);

	// Start T3 task
	rtems_task_start(T3_id, T3, 0);

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}

