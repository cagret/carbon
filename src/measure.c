#define _GNU_SOURCE

#include "measure.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/resource.h>

#define ENERGY_FILE "energy_uj"
#define RAPL_BASE_PATH "/sys/class/powercap"
#define MAX_PATH 256

static char rapl_energy_path[MAX_PATH] = {0};
static bool rapl_checked = false;
static bool rapl_available = false;

static struct timespec start_real, end_real;
static struct rusage start_rusage, end_rusage;
static double energy_start_cpu = 0.0;
static double energy_start_gpu = 0.0;

bool detect_rapl(void) {
    if (rapl_checked)
        return rapl_available;

    DIR *dir = opendir(RAPL_BASE_PATH);
    if (!dir) {
        rapl_checked = true;
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "intel-rapl:", 11) == 0) {
            int written = snprintf(rapl_energy_path, sizeof(rapl_energy_path),
                       "%s/%s/%s", RAPL_BASE_PATH, entry->d_name, ENERGY_FILE);
	    if (written < 0 || (size_t)written >= sizeof(rapl_energy_path)) {
		    fprintf(stderr, "RAPL path too long, skipping %s\n", entry->d_name);
		    continue;
	    }
            int fd = open(rapl_energy_path, O_RDONLY);
            if (fd >= 0) {
                close(fd);
                rapl_available = true;
                break;
            }
        }
    }

    closedir(dir);
    rapl_checked = true;
    return rapl_available;
}


double read_rapl(void) {
    if (!detect_rapl())
        return -1.0;

    int fd = open(rapl_energy_path, O_RDONLY);
    if (fd < 0)
        return -1.0;

    char buf[32] = {0};
    ssize_t len = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (len <= 0)
        return -1.0;

    return atof(buf) / 1e6;
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

    if (detect_rapl()) {
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

