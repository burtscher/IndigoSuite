# IndigoSuite

### About the Suite
The Indigo suite contains scripts to compile and run the CUDA and OpenMP microbenchmarks

### Minimum Requirements
We assume an OpenMP-capable compiler with ThreadSanitizer support

We assume a CUDA-enabled GPU that supports compute capability 3.5 or higher

### Directory layout
    .
    ├── include                  # Header files
    ├── input                    # Input graphs
    ├── CUDA                     # CUDA microbenchmarks
    ├── OpenMP                   # OpenMP microbenchmarks
    ├── run_CUDA.py              # Script to run all CUDA codes with all inputs
    ├── run_OpenMP.py            # Script to run all OpenMP codes with all inputs
    ├── demo_CUDA.py             # Script to run a few CUDA tests as demo
    ├── demo_OpenMP.py           # Script to run a few OpenMP tests as demo
    └── README.md

### Usage
* Use the script `run_CUDA.py` to run all CUDA tests with all inputs
```
python3 run_CUDA.py threads_per_block number_of_blocks
```
* Use the script `run_OpenMP.py` to run all OpenMP tests with all inputs
```
python3 run_OpenMP.py number_of_threads
```
