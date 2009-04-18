/*
 * Copyright 2007, 2008, 2009 by Brian Dominy <brian@oddchange.com>
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

/**
 * \file
 * \brief Manages which music track should be played.
 *
 * Like other effect modules (deffs and leffs), music effects are queued
 * and use priorities to determine which one gets played.  Up to MAX_TRACKS
 * can be queued at a time.  Whenever the highest priority queued effect
 * changes, the sound board is re-written with the updated music code.
 */


/** Starts a new music track. */
void music_start (audio_track_t track)
{
}


/** Stops a queued/running music track. */
void music_stop (audio_track_t track)
{
}

