# Karplus–Strong String Synthesis Algorithm

![CMake](https://img.shields.io/badge/CMake-3.20+-brightgreen)
![C++23](https://img.shields.io/badge/C++-17-blue)

## Table of Contents
1. [Project Description](#-project-description)
2. [Requirements](#-requirements)
3. [Building the Project](#-building-the-project)
4. [Usage](#-usage)
5. [Examples](#-examples)
6. [Project Structure](#-project-structure)

## Project Description
Implementation of the Karplus-Strong algorithm for string sound synthesis. This project allows you to:
- Generate individual notes (A4, E4, B4, F#, C#)
- Save results as properly named WAV files
- Create musical chords
- Play generated audio directly (requires SoX)

To play a sound put in the consol:
```bash
cd build/
play "filename"
```
## Requirements
- **CMake** version 3.20+
- **C++ compiler** with C++17 support
- **Build system** Make
- **SoX** (for audio playback - optional)

## Building the Project

### Manual Build:
```bash
mkdir build
cd build
cmake ..
make
./Karplus_app
play "filename"
```