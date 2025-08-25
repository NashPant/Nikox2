# Niko c++ library

Nikopolis C++ library with python and Excel bidndings

## Description

Collection of C++ routines and analytics compiled over the years. Initially it had just Excel bindings via the old Excel 1997. Subsequently using Alessio's infrastructure updated.
pyNiko using pybind11 exposes most of the functions to python

## Getting Started

### Dependencies

* Describe any prerequisites, libraries, OS version, etc., needed before installing program.
* ex. Windows 10

### Installing

* How/where to download your program
* Any modifications needed to be made to files/folders

### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

### Python Help
From Visual c++17 one can debug, using Native option, but remember to have only 1 python session open and select ALL python processes. For whatever reason if you select 1 by one the available python processes and try to debug it won't stop.
IF however you have only 1 Jupyter notebook open and select all listed python processes then the debugger will stop in the C++ breakpoint.

## Authors

Contributors names and contact info

* Thanos Pantas
* Karim Sadli

ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History
* 0.4
	Reorganising Data Strutures into a seperate project and introdicing a simple DataTable class. Still not sure which best to use yet.
* 0.3
	* a. Reorganising the pyNiko module to include submodules and exposing a couple of time series libraries, like statsforecast and Lowess Regression.
* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is free to use. 

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)