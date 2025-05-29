#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "measure.h"
#include "carbon.h"

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s -c config -- program [args ...]\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc < 4 || strcmp(argv[1], "-c") != 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *config_file = argv[2];
    char **prog_argv = &argv[3];

    carbon_config_t config;
    if (load_carbon_config(config_file, &config) != 0) {
        fprintf(stderr, "[ERREUR] Impossible de charger le fichier de configuration : %s\n", config_file);
        return EXIT_FAILURE;
    }

    measure_data_t data = {0};
    measure_start();

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        execvp(prog_argv[0], prog_argv);
        perror("execvp");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    measure_end(&data);

    fprintf(stderr, "=== Carbon Report ===\n");
    fprintf(stderr, "Real time        : %.3f s\n", data.time_real_s);
    fprintf(stderr, "CPU user time    : %.3f s\n", data.time_cpu_user_s);
    fprintf(stderr, "CPU system time  : %.3f s\n", data.time_cpu_sys_s);
    fprintf(stderr, "Max RSS          : %ld kB\n", data.mem_max_kb);
    fprintf(stderr, "Energy CPU       : %.3f J %s\n", data.energy_cpu_j,
            data.rapl_available ? "(RAPL)" : "(-)");
    fprintf(stderr, "Energy GPU       : %.3f J %s\n", data.energy_gpu_j,
            data.nvml_available ? "(NVML)" : "(-)");

    double co2_cpu = calculate_co2(data.energy_cpu_j, &config);
    double co2_gpu = calculate_co2(data.energy_gpu_j, &config);
    double co2_total = co2_cpu + co2_gpu;

    fprintf(stderr, "CO₂ CPU (g)      : %.3f\n", co2_cpu);
    fprintf(stderr, "CO₂ GPU (g)      : %.3f\n", co2_gpu);
    fprintf(stderr, "CO₂ total (g)    : %.3f\n", co2_total);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        fprintf(stderr, "[ERREUR] Le programme a été tué par le signal %d (%s)\n",
                WTERMSIG(status), strsignal(WTERMSIG(status)));
        return 128 + WTERMSIG(status);
    } else {
        return EXIT_FAILURE;
    }
}
