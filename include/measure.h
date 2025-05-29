#ifndef MEASURE_H
#define MEASURE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	double energy_cpu_j;
	double energy_gpu_j;
	double time_real_s;
	double time_cpu_user_s;
	double time_cpu_sys_s;
	bool rapl_available;
	bool nvml_available;
	long mem_max_kb;
} measure_data_t;


bool detect_rapl(void);
double read_rapl(void);
bool detect_nvml(void);
double read_nvml(void);
void measure_start(void);
void measure_end(measure_data_t *data);

#endif
