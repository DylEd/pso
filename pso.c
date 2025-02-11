#include <bits/time.h>
#include "pso.h"

#include <stdlib.h>
#include <time.h>

static
void
fill(	pso_uint	dim,
	pso_float	to[dim],
	pso_float	from[dim])
{
	for(int i = 0; i < dim; i++)
	{
		to[i] = from[i];
	}
}

static
pso_float
gen_random()
{
	return (pso_float) random() / (pso_float) (1L<<31);
	//return (pso_float) random() / (pso_float) (2L^31 - 1);
}

pso_float *
pso_swarm(	pso_function_t			function,
		pso_uint			swarm_count,
		pso_uint			dimension,
		pso_float			ranges[dimension][2],
		pso_float			inertia_weight,
		pso_float			cognitive_coefficient,
		pso_float			social_coefficient,
		pso_uint			iterations,
		pso_post_iteration_function_t	post_iteration_function)
{
	srandom(clock());

	pso_float particles[swarm_count][dimension];
	pso_float particles_best_known_pos[swarm_count][dimension];
	pso_float particles_best_known_pos_value[swarm_count];
	pso_float velocities[swarm_count][dimension];

	//pso_float swarm_best_known_pos[dimension];
	pso_float *swarm_best_known_pos = malloc(dimension * sizeof(pso_float));
	pso_float swarm_best_known_pos_value = PSO_FLOAT_MAX;

	pso_float mins[dimension];
	pso_float maxs[dimension];
	pso_float rans[dimension];

	for(int i = 0; i < dimension; i++)
	{
		mins[i] = ranges[i][0] < ranges[i][1] ? ranges[i][0] : ranges[i][1];
		maxs[i] = ranges[i][0] > ranges[i][1] ? ranges[i][0] : ranges[i][1];
		rans[i] = maxs[i] - mins[i];
	}

	// INITIALIZE PARTICLES
	for(int i = 0; i < swarm_count; i++)
	{
		for(int j = 0; j < dimension; j++)
		{
			particles[i][j] = rans[j] * gen_random() + mins[j];
			particles_best_known_pos[i][j] = particles[i][j];

			velocities[i][j] = 2 * rans[j] * gen_random() - rans[j];
		}

		particles_best_known_pos_value[i] = function(dimension,particles[i]);

		pso_float r = function(dimension,particles[i]);
		if(r < swarm_best_known_pos_value)
		{
			fill(dimension,swarm_best_known_pos,particles_best_known_pos[i]);
			swarm_best_known_pos_value = r;
		}
	}

	// SWARM
	for(int iter = 0; iter < iterations; iter++)
	{
		for(int i = 0; i < swarm_count; i++)
		{
			for(int j = 0; j < dimension; j++)
			{
				pso_float cognitive_rate = gen_random();
				pso_float social_rate = gen_random();

				velocities[i][j] = inertia_weight * velocities[i][j]
							+ cognitive_coefficient * cognitive_rate * (particles_best_known_pos[i][j] - particles[i][j])
							+ social_coefficient * social_rate * (swarm_best_known_pos[j] - particles[i][j]);

				particles[i][j] += velocities[i][j];
			}

			pso_float r = function(dimension,particles[i]);
			if(r < particles_best_known_pos_value[i])
			{
				fill(dimension,particles_best_known_pos[i],particles[i]);
				particles_best_known_pos_value[i] = r;
				if(r < swarm_best_known_pos_value)
				{
					fill(dimension,swarm_best_known_pos,particles[i]);
					swarm_best_known_pos_value = r;
				}
			}
		}

		if(post_iteration_function != 0)
		{
			pso_uint b;
			b = post_iteration_function(swarm_best_known_pos_value,swarm_best_known_pos,iter,iterations,
						&inertia_weight,&cognitive_coefficient,&social_coefficient);

			if(b)
			{
				break;
			}
		}
	}

	return swarm_best_known_pos;
}

