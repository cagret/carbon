#ifndef CARBON_H
#define CARBON_H

#include "measure.h"


typedef struct {
	double charbon;
	double nucleaire;
	double renouvelable;
} factors_t;


typedef struct {
	double charbon;
	double nucleaire;
	double renouvelable;
} mix_t;


typedef struct {
	factors_t factors;
	mix_t mix;
} carbon_config_t;


int load_carbon_config(const char *filename, carbon_config_t *config);
double calculate_co2(double energy_joule, carbon_config_t *config);
#endif
