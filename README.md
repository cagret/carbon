# Carbon

`Carbon` measures the energy usage and estimates the CO₂ emissions of a command.

<<<<<<< HEAD
It works like `time`, but reports energy consumed and carbon footprint.
=======
When benchmarking software, developers commonly use the `time` command to measure runtime. While useful, this measure alone is **incomplete**: it tells us **how long** a program runs, but says nothing about **how much energy** it consumes or **how polluting** that energy is.

In this project, we aim to go further:
- Provide **real energy usage measurements**, from sources like RAPL (for CPU) and cgroups.
- Estimate the **CO₂ emissions** using a configurable energy mix.
- **Raise awareness** about the ecological impact of computing.

We believe it's time to shift perspective: 
> Instead of asking "how fast is my program?", we should ask:  
> "Is it energy-efficient?" and "What is its environmental cost?"

## Why it matters

Digital tools consume real energy and depending on how that energy is produced, the ecological footprint can vary dramatically. In an age of climate urgency, developers must become conscious of:
- The impact of inefficient code.
- The benefit of **running fewer, smarter jobs** rather than brute-forcing.
- The ecological value of **efficient, low-level languages like C** that can do more with less.

Using this tool, we hope to encourage:
- Responsible coding habits.
- Better trade-offs between performance and sustainability.
- A culture of ecological awareness in the developer community.

## Features

- CPU energy measurement via **RAPL** (Intel)
- Optional GPU usage stub via **NVML**
- CPU tracking with **cgroups**
- CO₂ estimation based on user-provided **energy mix**
- Modular architecture for easy extension
>>>>>>> ce62203f7c46b0504ad55642b1ae56a9045d102d

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
