# DataArt Summer practice 2021

## C++ demo task

###Main goal

Speed up the program. Test condition: 4 threads, 512M file.

###Program description

Input file contains constant-size (4096B including null-termitation character) lines of letters add digits.
The program swap case for letters and add one to each digit except 9 which turns into 0. Output lines are saved in output file.

##Repo Content

###generate_file.py

Creates input file: input.data (512M by default) 
###process_file.py

Does required job. Can be used for consistency check and for comparison c++ and python code performance.
###main.cpp

Source code of c++ soluton
###CMakeLists.txt

File for project building

##Build the project

###Windows

Option 1: Google: how to build cmake project on windows;

Option 2: Create a project in your beloved IDE. Add main.cpp in the project;

Option3: [Download and install Clion IDE] Open CMateLists.txt as a project in Clion. 

###Linux

Go to console,
cd to project directory,
run commands:

mkdir build

cd build

cmake ../

make

You may probably need pthread library to be included


##Tips and tricks

* Get rid of unnecessary data copying including implicit (vector.push_bak, string constructor, copy while passing var s in functions)
* Improve process_line algorithm. Get rid of if-statements, for example. 
* Make data distribution per thread more cache-friendly. Data for each thread should lay down in memory in
  one continuous peace to minimise cache misses.
    
* Try to map file in memory (linux: mmap() windows: https://docs.microsoft.com/en-us/windows/win32/memory/file-mapping)

