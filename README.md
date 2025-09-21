# Niko c++ library

Nikopolis C++ library with python and Excel bidndings

## Description

Collection of C++ routines and analytics compiled over the years. Initially it had just Excel bindings via the old Excel 1997. Subsequently using Alessio's infrastructure updated.
pyNiko using pybind11 exposes most of the functions to python

## Getting Started

### Setting Up vcpkg for automatic library management

1. Clone (or update) the vcpkg repo
2. Go to the vcpkg folder and run bootstrap-vcpkg.bat
3. Check the latest hash with git log -1 --pretty=format:%H
    Add this to vckpg.json , for example:
  {
  "name": "myproject",
  "version-string": "0.1.0",
  "builtin-baseline": "4d3b16ef4e22b7a2f8459dbe85c709d221e9d83e",
  "dependencies": [
    "boost",
    "fmt"
  ]
}
4. IN CmakePresets.json add the following :
  "cacheVariables": {
  "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
  "VCPKG_MANIFEST_MODE": "ON"
}  
1. Step 1: Disable Visual Studio's vcpkg Integration
   Disable VS's built-in vcpkg
cd "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg"
.\vcpkg.exe integrate remove

 Step 2: Integrate Your Project vcpkg
bash# Integrate your local vcpkg
cd C:\development\Nikopolis\ThirdPArty\vcpkg
.\vcpkg.exe integrate install

Step 3. Change CmakePresets.json
"CMAKE_TOOLCHAIN_FILE": "C:/development/Nikopolis/ThirdPArty/vcpkg/scripts/buildsystems/vcpkg.cmake"

So now everytime you open a new VS instance it will use your local vcpkg and not the built in one.
How come though the environment variable VCPKG_ROOT is still pointing to the built in vcpkg ?


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

*Old vckpg baseline hash* :  "builtin-baseline": "6ecbbbdf31cba47aafa7cf6189b1e73e10ac61f8",

Older manifest hash: "builtin-baseline": "d5ec528843d29e3a52d745a64b469f810b2cedbf",

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