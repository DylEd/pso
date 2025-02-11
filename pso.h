#ifndef __PSO
#define __PSO

#include <stdint.h>
#include <float.h>

typedef uint32_t pso_uint;
typedef double pso_float;
//const pso_float PSO_FLOAT_MAX = DBL_MAX;
#define PSO_FLOAT_MAX DBL_MAX;

typedef pso_float (*pso_function_t)(pso_uint n,pso_float[n]);

// return 0 if solution is not accepted; return non-0 if solution is accepted
typedef pso_uint (*pso_post_iteration_function_t)(	pso_float const	 swarm_best_pos_value,
							pso_float const	*swarm_best_pos,
							pso_uint	 iteration,
							pso_uint	 iteration_total,
							pso_float	*inertia_weight,
							pso_float	*cognitive_coefficient,
							pso_float	*social_coefficient);

pso_float *
pso_swarm(	pso_function_t			function,
		pso_uint			swarm_count,
		pso_uint			dimension,
		pso_float			ranges[dimension][2],
		pso_float			inertia_weight,		// (0,1)
		pso_float			cognitive_coefficient,	// try [1,3]
		pso_float			social_coefficient,	// try [1,3]
		pso_uint			iterations,
		pso_post_iteration_function_t	post_iteration_function);

#endif /* __PSO */

