/*
 * Copyright 2007 by Brian Dominy <brian@oddchange.com>
 *
 * This file is part of FreeWPC.
 *
 * FreeWPC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * FreeWPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FreeWPC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <freewpc.h>

/* Include autoplunger logic only when all of these machine
 * characteristics are known */
#if defined(MACHINE_LAUNCH_SWITCH) && defined(MACHINE_LAUNCH_SOLENOID) && defined(MACHINE_SHOOTER_SWITCH)
#define INCLUDE_AUTOPLUNGER
#endif

/**
 * \file
 * \brief Common plunger routines.
 */

#ifdef INCLUDE_AUTOPLUNGER
void timed_plunger_monitor (void)
{
	task_sleep_sec (7);
	VOIDCALL (plunger_sw_launch_button);
	task_exit ();
#endif


CALLSET_ENTRY (plunger, ball_in_play)
{
#ifdef INCLUDE_AUTOPLUNGER
	task_kill_gid (GID_TIMED_PLUNGER_MONITOR);
#endif
}


CALLSET_ENTRY (plunger, sw_shooter)
{
#ifdef INCLUDE_AUTOPLUNGER
	if (ball_in_play)
	{
		/* If ball is already in play, then autolaunch any balls
		right away.  TODO : UNLESS the coin door is open, or in
		tournament mode. */
		VOIDCALL (plunger_sw_launch_button);
	}
	else if (system_config.timed_plunger == ON)
	{
		/* If timed plunger is enabled, then start a timer
		to autoplunge the ball regardless of button press */
		task_create_gid1 (GID_TIMED_PLUNGER_MONITOR, timed_plunger_monitor);
	}
#endif
}


CALLSET_ENTRY (plunger, sw_launch_button)
{
#ifdef INCLUDE_AUTOPLUNGER
	if (switch_poll (MACHINE_SHOOTER_SWITCH))
	{
		sol_pulse (MACHINE_LAUNCH_SOLENOID);
	}
#endif
}


CALLSET_ENTRY (plunger, sw_l_l_flipper_button)
{
#ifdef INCLUDE_AUTOPLUNGER
	if (system_config.flipper_plunger == ON)
		plunger_sw_launch_button ();
#endif
}


CALLSET_ENTRY (plunger, sw_l_r_flipper_button)
{
#ifdef INCLUDE_AUTOPLUNGER
	if (system_config.flipper_plunger == ON)
		plunger_sw_launch_button ();
#endif
}

