/*
 * Priority Inheritance Semaphores RTEMS Project
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
	PRINT_TIME("STARTING T1") ;
	consume_ticks(4);
	PRINT_TIME("TRYING TO GET SEMAPHORE T1") ;
	rtems_semaphore_obtain(critical_section_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT) ;
	PRINT_TIME("GOT SEMAPHORE T1") ;
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
	rtems_task_wake_after(15);
	PRINT_TIME("STARTING T2") ;
	consume_ticks(7) ;
	PRINT_TIME("FINISHING T2") ;

	rtems_task_delete(RTEMS_SELF);
}

rtems_task T3(rtems_task_argument argument)
{
	// TODO: Implement task's behaviour

	PRINT_TIME("STARTING T3");
	consume_ticks(6);
	PRINT_TIME("TRYING TO GET SEMAPHORE T3") ;
	rtems_semaphore_obtain(critical_section_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT) ;
	PRINT_TIME("GOT SEMAPHORE T3") ;
	consume_ticks(10);
	PRINT_TIME("RELEASING SEMAPHORE T3") ;

	rtems_semaphore_release(critical_section_sem);
	consume_ticks(4);
	PRINT_TIME("FINISHING  T3") ;

	rtems_task_delete(RTEMS_SELF);

}


rtems_task Init(rtems_task_argument arg)
{
	rtems_id T1_id;
	rtems_id T2_id;
	rtems_id T3_id;

	// TODO: Create the semaphore
	rtems_semaphore_create(rtems_build_name('s', 'e', 'm', '1'), 1,
			RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY, 0, &critical_section_sem) ;

	// Create T1
	rtems_task_create(rtems_build_name('T', 'M', 'S', 'V'),
			10, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T1_id);

	// Start T1
	rtems_task_start(T1_id, T1, 0);

	// Create T2
	rtems_task_create(rtems_build_name('H', 's', 'k', 'p'),
			15, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T2_id);

	// Start T2
	rtems_task_start(T2_id, T2, 0);

	// Create T3
	rtems_task_create(rtems_build_name('A', 'C', 'S', 'T'),
			20, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_NO_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &T3_id);

	// Start T3
	rtems_task_start(T3_id, T3, 0);

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}
