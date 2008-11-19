
#include <freewpc.h>
#include <simulation.h>

/** The maximum number of balls that can be tracked in simulation */
#define SIM_MAX_BALLS 8

#define MAX_BALL_LOCATIONS 128

#define SIM_LOCATION_NONE 0
#define SIM_NO_BALL_HERE -1


void_function sim_location_handler[MAX_BALL_LOCATIONS] = { NULL, };

/**
 * Maintains the locations of all the pinballs.
 *
 * If 0, it means the ball is untracked.
 * If less than NUM_SWITCHES, it means the ball is on
 *   a particular switch.
 * Larger values are reserved for game-specific
 *   purposes.
 */
unsigned int sim_ball_location[SIM_MAX_BALLS];


/**
 * The same information, in reverse.
 * Maintain which pinball is located at each location.
 * A value of -1 means no pinball is there.
 */
int sim_location_ball[MAX_BALL_LOCATIONS];


void sim_location_toggle (unsigned int location)
{
	if (location < NUM_SWITCHES)
		sim_switch_toggle (location);
}

const char *
sim_ball_location_name (unsigned int location)
{
	extern const char *names_of_switches[];
	if (location == 0)
		return "Free";
	else
		return names_of_switches[location];
}


/**
 * Move a ball to a specific location.
 */
void sim_ball_move (unsigned int ballno, unsigned int location)
{
	/* Remove the ball from its previous location */
	unsigned int prev_location = sim_ball_location[ballno];
	if (prev_location != SIM_LOCATION_NONE)
	{
		sim_location_toggle (prev_location);
		sim_location_ball[location] = SIM_NO_BALL_HERE;
	}

	/* Set the ball at the new location */
	sim_location_toggle (location);
	sim_ball_location[ballno] = location;
	sim_location_ball[location] = ballno;
#ifdef CONFIG_UI
	ui_update_ball_tracker (ballno, location);
#endif
	simlog (SLC_DEBUG, "Ball %d @ %s", ballno, sim_ball_location_name (location));
}


void sim_register_location_handler (unsigned int location,
                                    void_function handler)
{
}


void sim_ball_init (void)
{
	int n;
	/* Initialize all locations to empty */
	for (n=0; n < MAX_BALL_LOCATIONS; n++)
		sim_location_ball[n] = SIM_NO_BALL_HERE;

	/* Initialize all balls to no location */
	for (n=0; n < SIM_MAX_BALLS; n++)
		sim_ball_location[n] = SIM_LOCATION_NONE;
}
