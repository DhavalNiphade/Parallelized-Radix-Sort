## Parallel Radix Sort

This folder houses all the required code in C++ required to compile and run the
Parallel Radix Sort program. For convenience, a shell script has been provided,
titled "*BuildMe.sh*" that can be run on a linux operating system to compile the
source files.

* **Building the program:**
  * You may choose to build the program manually using the following commands,
  depending on the environment.

    * **GNU**   
    mpic++ -std=c++1y -o output radix2.cpp utilities.cpp sortInts.cpp sortDoubles.cpp
    sortStrings.cpp

    * **Cray**  
    CC -hstd=c++14 -o output radix2.cpp utilities.cpp sortInts.cpp sortDoubes.cpp
    sortStrings.cpp

  * Else, simply use the **shell script** we've provided.
    * ./BuildMe.sh  

* **Running the program**
  * To run the program, simply enter the following line on your terminal and hit Enter

    * **GNU**  
    mpirun -n <_number of processes_\> **./output** <filename\> <flag\>

    * **CRAY**
    aprun -n <_number of processes_\> **./output** <filename\> <flag\>


### Folder Structure

* **RadixCC** - contains the cray version of the code. The cray compiler doesn't accept
_.cpp_ file extensions and thus requires them to be renamed to _.c++_  

* **Serial** - contains the serial implementation of the code we're comparing against.

* **Radix** - Contains the aforementioned two folders and the GNU version of the program.
