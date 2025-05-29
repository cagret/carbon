# Carbon

`Carbon` measures the energy usage and estimates the CO₂ emissions of a command.

## Usage

```sh
carbon -c energy_mix.conf command [args...]
```

* Run `command` with `args`
* Measure runtime, energy, and CO₂ emissions
* Print summary at end

## Build

```sh
make        # build carbon executable
make clean  # remove binaries and objects
```

## Configuration

Energy mix file is conf format with emission factors and energy source mix:

```conf
; Source: https://www.statista.com/
factors_charbon=720
factors_nucleaire=12
factors_renouvelable=14
mix_charbon=0.95
mix_nucleaire=0.02
mix_renouvelable=0.03
```

## Requirements

* POSIX environment
* C compiler (`cc`)

## Notes

* RAPL energy measurement requires compatible CPU and permissions
* GPU measurement is optional and stubbed
* cgroups tracking only on Linux

---

*Measure time. Measure energy. Measure impact.*
