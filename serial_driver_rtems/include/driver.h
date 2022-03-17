/*
 * Serial Driver functions. This file belongs to the Serial Driver project.
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

#ifndef MAIN__DRIVER_H
#define MAIN__DRIVER_H

#include <rtems.h>
#include <rtems/bspIo.h>

rtems_device_driver serial_driver_read (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg);

rtems_device_driver serial_driver_initialize (
			rtems_device_major_number major,
			rtems_device_minor_number dummy,
			void *arg);

rtems_device_driver serial_driver_close (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg);

rtems_device_driver serial_driver_write (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg);

rtems_device_driver serial_driver_open(
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg);

#endif // MAIN__DRIVER_H
