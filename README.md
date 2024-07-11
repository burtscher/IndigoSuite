# IndigoSuite

### About the Suite
Indigo is a suite of input-dependent graph-code patterns. The inputs and code patterns are automatically generated.

### Minimum Requirements
We assume an OpenMP-capable compiler with ThreadSanitizer support and a CUDA-enabled GPU with compute capability 3.5 or higher.

### How to Generate the Suite
* Run the `generate_suite.py` script in the `Generator` directory to create the suite.
```
$ cd Generators
$ ./generate_suite.py
```
* The suite is written into the `/Generators/IndigoSuite_VERSION` directory.

### Managing the Code and Input Generation
* The `configure.txt` file in the `Generator` directory determines which subset of the codes and inputs will be generated.
* The `configure_examples.txt` file in the same directory lists several examples.

### Citing Indigo

If you use Indigo, please cite the following publication.

* Yiqian Liu, Noushin Azami, Corbin Walters, and Martin Burtscher. "The Indigo Program-Verification Microbenchmark Suite of Irregular Parallel Code Patterns." Proceedings of the 2022 IEEE International Symposium on Performance Analysis of Systems and Software, pp. 24-34. May 2022.
[[doi]](https://doi.org/10.1109/ISPASS55109.2022.00003)
[[pdf]](http://cs.txstate.edu/~mb92/papers/ispass22.pdf)
[[pptx]](http://www.cs.txstate.edu/~mb92/research/IndigoSuite/ispass22.pptx)
[[video]](http://www.cs.txstate.edu/~mb92/research/IndigoSuite/YiqianISPASS22.mp4)

You may also be interested in the follow-on suites [Indigo2](https://cs.txstate.edu/~burtscher/research/Indigo2Suite/) and [Indigo3](https://github.com/burtscher/Indigo3Suite).

*This work has been supported in part by the National Science Foundation under Grant No. 1955367 as well as by an equipment donation from NVIDIA Corporation.*
