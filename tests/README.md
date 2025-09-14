
======
BUILD GSL EXTERNALLY (WINDOWS ONLY. ASSUMING SIMILAR PROCESS IN LINUX)
========================================================================


Assuming you have a VCPKG_ROOT environmental variable which points to where vcpkg is installed.
Open up a powershell from visual studio 2022 from Tools -> Command Line-> Developer Power Shell
For x64 build open up explicitly "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
and make sure that you see Microsoft (R) C/C++ Optimizing Compiler Version xx.xx.xxx for x64 when running cl

If you add this script in the powershell 7 it will load by default x64.
Import-Module "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath "C:\Program Files\Microsoft Visual Studio\2022\Community" -SkipAutomaticLocation -DevCmdArguments '-arch=x64'

It can be verified by typing 
cl  <go> in the powershell command prompt, you will seee:
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35214 for x64


Steps to build
1) Clone:
git clone https://github.com/ampl/gsl.git
cd gs1
2) Create Build Directory:
mkdir build
cd build
3) Configure CMake: Use Ninja and Vcpkg toolchain, disabling AMPL bindings and tests:
this builds default 32
cmake.. -G "Ninja" -DNO_AMPL_BINDINGS=1 -DGSL_DISABLE_TESTS=1 -DCMAKE_TOOLCHAIN_FILE=$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake for 64

you need to open up powershell for 64 as above and make sure it is x64
cmake .. -G "Ninja" -DNO_AMPL_BINDINGS=1 -DGSL_DISABLE_TESTS=1 -DCMAKE_TOOLCHAIN_FILE="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows

4) Ninja it
ninja
GSL_ROOT od Cmake should point to build directory above