
#include <freewpc.h>


extern inline void score_deff_begin (const font_t *font, U8 x, U8 y, const char *text)
{
	score_update_start ();
	dmd_alloc_low_high ();
	dmd_clean_page_low ();
	font_render_string_center (font, x, y, text);
	sprintf_current_score ();
	font_render_string_center (&font_fixed6, 64, 16, sprintf_buffer);
	dmd_copy_low_to_high ();
}


extern inline void score_deff_end (task_ticks_t flash_rate)
{
	dmd_show_low ();
	while (!score_update_required ())
	{
		task_sleep (flash_rate);
		dmd_show_other ();
	}
}


extern inline bool multiball_mode_start (U8 flag, U8 deff, U8 leff, U8 music)
{
	if (!flag_test (flag))
	{
		flag_on (flag);
		if (deff != DEFF_NULL)
		{
			deff_start (deff);
			if (music != 0)
			{
				audio_track_t track;
				track.code = music;
				track.prio = deff_table[deff].prio;
				music_start (track);
			}
		}
		if (leff != LEFF_NULL)
			leff_start (leff);
		return TRUE;
	}
	else
		return FALSE;
}


extern inline bool multiball_mode_stop (U8 flag, U8 deff, U8 leff, U8 music)
{
	if (flag_test (flag))
	{
		flag_off (flag);
		if (deff != DEFF_NULL)
		{
			deff_stop (deff);
			if (music != 0)
			{
				audio_track_t track;
				track.code = music;
				track.prio = deff_table[deff].prio;
				music_stop (track);
			}
		}
		if (leff != LEFF_NULL)
			leff_stop (leff);
		return TRUE;
	}
	else
		return FALSE;
}



__local__ U8 chaosmb_level;

__local__ U8 chaosmb_hits_to_relight;

struct {
	const char *shot_name;
	U8 jackpot_value;
	U8 lamp_num;
} chaosmb_shots[] = {
	{ "LEFT RAMP", 15, },
	{ "RIGHT RAMP", 20, },
	{ "PIANO", 25,  },
	{ "HITCHHIKER", 30 },
	{ "POWER PAYOFF", 40 },
	{ "DEAD END", 50 },
};

void chaosmb_lamp_update (void)
{
	if (flag_test (FLAG_CHAOSMB_RUNNING))
	{
	}
}


void chaosmb_running_deff (void)
{
	for (;;)
	{
		score_deff_begin (&font_fixed6, 64, 4, "CHAOS MULTIBALL");
		if (chaosmb_hits_to_relight == 0)
		{
			sprintf ("SHOOT %s", chaosmb_shots[chaosmb_level].shot_name);
			font_render_string_center (&font_var5, 64, 27, sprintf_buffer);
		}
		else
		{
			sprintf ("HIT CLOCK %d MORE TIMES", chaosmb_hits_to_relight);
			font_render_string_center (&font_var5, 64, 27, sprintf_buffer);
		}
		score_deff_end (TIME_100MS);
	}
}


void chaosmb_score_jackpot (void)
{
	chaosmb_level++;
	chaosmb_hits_to_relight = chaosmb_level * 2;
	sound_send (SND_EXPLOSION_1);
}



void chaosmb_start (void)
{
	if (multiball_mode_start (FLAG_CHAOSMB_RUNNING, DEFF_CHAOSMB_RUNNING, 0, MUS_SPIRAL_ROUND))
	{
		chaosmb_level = 0;
		chaosmb_hits_to_relight = 0;
		device_multiball_set (3);
	}
}

void chaosmb_stop (void)
{
	if (multiball_mode_stop (FLAG_CHAOSMB_RUNNING, DEFF_CHAOSMB_RUNNING, 0, MUS_SPIRAL_ROUND))
	{
	}
}


void chaosmb_check_level (U8 level)
{
	if (flag_test (FLAG_CHAOSMB_RUNNING)
		&& (chaosmb_level == level))
	{
		chaosmb_score_jackpot ();
	}
}

CALLSET_ENTRY (chaosmb, door_start_clock_chaos)
{
	chaosmb_start ();
}


CALLSET_ENTRY (chaosmb, sw_left_ramp_exit)
{
	chaosmb_check_level (0);
}

CALLSET_ENTRY (chaosmb, sw_right_ramp)
{
	chaosmb_check_level (1);
}

CALLSET_ENTRY (chaosmb, sw_piano)
{
	chaosmb_check_level (2);
}

CALLSET_ENTRY (chaosmb, sw_hitchhiker)
{
	chaosmb_check_level (3);
}

CALLSET_ENTRY (chaosmb, sw_power_payoff)
{
	chaosmb_check_level (4);
}

CALLSET_ENTRY (chaosmb, sw_dead_end)
{
	chaosmb_check_level (5);
}

CALLSET_ENTRY (chaosmb, sw_clock_target)
{
	if (flag_test (FLAG_CHAOSMB_RUNNING)
		&& (chaosmb_hits_to_relight > 0))
	{
		chaosmb_hits_to_relight--;
		if (chaosmb_hits_to_relight == 0)
		{
			chaosmb_level++;
		}
	}
}

CALLSET_ENTRY (chaosmb, single_ball_play)
{
	chaosmb_stop ();
}

CALLSET_ENTRY (chaosmb, start_player)
{
	chaosmb_level = 0;
	chaosmb_hits_to_relight = 0;
}


CALLSET_ENTRY (chaosmb, sw_buyin_button)
{
	if (in_live_game)
		chaosmb_start ();
}
