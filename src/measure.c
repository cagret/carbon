#define _GNU_SOURCE

#include "measure.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

static struct timespec start_real, end_real;
static struct rusage start_rusage, end_rusage;
static double energy_start_cpu = 0.0;
static double energy_start_gpu = 0.0;

static bool rapl_available = false;

bool detect_rapl(void) {
    static bool detected = false;
    static bool checked = false;

    if (!checked) {
        int fd = open("/sys/class/powercap/intel-rapl:0/energy_uj", O_RDONLY);
        if (fd < 0)
            detected = false;
        else {
            detected = true;
            close(fd);
        }
        checked = true;
    }
    return detected;
}

double read_rapl(void) {
    int fd = open("/sys/class/powercap/intel-rapl:0/energy_uj", O_RDONLY);
    if (fd < 0)
        return -1;
    char buf[32] = {0};
    ssize_t len = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (len <= 0)
        return -1;
    return atof(buf)/1e6;
}

bool detect_nvml(void) {
    return false;
}

double read_nvml(void) {
    return 0.0;
}

void measure_start(void) {
    clock_gettime(CLOCK_MONOTONIC, &start_real);
    getrusage(RUSAGE_SELF, &start_rusage);

    rapl_available = detect_rapl();
    if (rapl_available) {
        energy_start_cpu = read_rapl();
    } else {
        energy_start_cpu = 0.0;
    }

    if (detect_nvml()) {
        energy_start_gpu = read_nvml();
    } else {
        energy_start_gpu = 0.0;
    }
}

void measure_end(measure_data_t *data) {
    clock_gettime(CLOCK_MONOTONIC, &end_real);
    getrusage(RUSAGE_SELF, &end_rusage);

    data->time_real_s = (end_real.tv_sec - start_real.tv_sec) +
                        (end_real.tv_nsec - start_real.tv_nsec)/1e9;

    data->time_cpu_user_s = (end_rusage.ru_utime.tv_sec - start_rusage.ru_utime.tv_sec) +
                            (end_rusage.ru_utime.tv_usec - start_rusage.ru_utime.tv_usec)/1e6;

    data->time_cpu_sys_s = (end_rusage.ru_stime.tv_sec - start_rusage.ru_stime.tv_sec) +
                           (end_rusage.ru_stime.tv_usec - start_rusage.ru_stime.tv_usec)/1e6;

    data->mem_max_kb = end_rusage.ru_maxrss;

    if (rapl_available) {
        double energy_end_cpu = read_rapl();

        if (energy_end_cpu < energy_start_cpu) {
            double max_counter = 4294.967296;
            data->energy_cpu_j = (energy_end_cpu + max_counter) - energy_start_cpu;
        } else {
            data->energy_cpu_j = energy_end_cpu - energy_start_cpu;
        }
        data->rapl_available = true;
    } else {
        data->energy_cpu_j = 0.0;
        data->rapl_available = false;
    }

    if (detect_nvml()) {
        double energy_end_gpu = read_nvml();
        data->energy_gpu_j = energy_end_gpu - energy_start_gpu;
        data->nvml_available = true;
    } else {
        data->energy_gpu_j = 0.0;
        data->nvml_available = false;
    }
}

