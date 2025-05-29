#include "carbon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int load_carbon_config(const char *filename, carbon_config_t *config) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char key[64];
        double val;
        if (sscanf(line, "%63[^=]=%lf", key, &val) == 2) {
            if (strcmp(key, "factors_charbon") == 0)
                config->factors.charbon = val;
            else if (strcmp(key, "factors_nucleaire") == 0)
                config->factors.nucleaire = val;
            else if (strcmp(key, "factors_renouvelable") == 0)
                config->factors.renouvelable = val;
            else if (strcmp(key, "mix_charbon") == 0)
                config->mix.charbon = val;
            else if (strcmp(key, "mix_nucleaire") == 0)
                config->mix.nucleaire = val;
            else if (strcmp(key, "mix_renouvelable") == 0)
                config->mix.renouvelable = val;
        }
    }

    fclose(fp);
    return 0;
}

double calculate_co2(double energy_joule, carbon_config_t *config) {
    double kwh = energy_joule / 3.6e6;
    double co2 = kwh * (
        config->factors.charbon * config->mix.charbon +
        config->factors.nucleaire * config->mix.nucleaire +
        config->factors.renouvelable * config->mix.renouvelable
    );
    return co2;
}
