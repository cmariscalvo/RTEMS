/*
 * Error Management RTEMS Project
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

#define PRINT(fmt,args...)  printk ( fmt "\n",\
                            ##args)

#define PRINT_TIME(fmt,args...) do { \
			unsigned int __current_tick; \
			rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &__current_tick); \
			printk ("%lu: " fmt "\n", __current_tick, ##args); \
			} while (0)


void force_division_by_zero(void)
{
	uint32_t dummy = 12345;
	uint32_t result = 0;

    __asm__ __volatile__ (
            "udiv %1, 0, %0" : "=r" (result) : "r" (dummy) );
}

/** ISR for the divide-by-zero trap */
void divide_by_zero_isr(rtems_vector_number vector)
{
	CPU_Interrupt_frame * isf;

	__asm__ __volatile__ (
	            "mov %%i1, %0" : "=r" (isf) : );

	PRINT("Division by zero on address (0x%X)", isf->tpc);
	return;
}

/**
 * This task forces a divide-by-zero error.
 */
rtems_task the_task(rtems_task_argument argument)
{

	PRINT_TIME("Task Starts");

	force_division_by_zero();

	PRINT_TIME("I'm still alive!");

	rtems_task_delete(RTEMS_SELF);
}


rtems_task Init(rtems_task_argument arg)
{
	rtems_id the_task_id;
	rtems_isr_entry old_isr;

	// TODO: Install the handler for the SYNCHRONOUS INTERRUPT
	// "divide by zero" trap (0x2A)

	rtems_task_create(rtems_build_name('T', 'a', 's', 'k'),
			10, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT | RTEMS_TIMESLICE,
			RTEMS_DEFAULT_ATTRIBUTES, &the_task_id);

	rtems_task_start(the_task_id, the_task, 0);

	/** Delete the initial task from the system */
	rtems_task_delete(RTEMS_SELF);

}
