# Carbon
`Carbon` is a modular C project designed to **measure the energy consumption** of a program and **estimate its carbon footprint**, based on the local energy mix (provided in JSON format). It offers a more comprehensive view of performance than traditional tools like `time`, by addressing not only execution time but also **environmental impact**.

## Motivation

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

## Usage

### Build

```bash
make           # Build the main executable
make test      # Build unit tests
make clean     # Clean compiled files
```
