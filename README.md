# IndigoSuite

### About the Suite
Indigo is a suite of input-dependent graph-code patterns. The inputs and code patterns are automatically generated.

### Minimum Requirements
We assume an OpenMP-capable compiler with ThreadSanitizer support and a CUDA-enabled GPU with compute capability 3.5 or higher.

### How to Generate the Suite
* Run the `generate_suite.py` script in the `Generator` directory to create the suite.
```
$ chmod +x generate_suite.py
$ ./generate_suite.py
```
* The suite is written into the `/Generators/IndigoSuite_VERSION` directory.

### Managing the Code and Input Generation
* The `configure.txt` file in the `Generator` directory determines which subset of the codes and inputs will be generated.
* The `configure_examples.txt` file in the same directory lists several examples.
