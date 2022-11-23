# General description
Repository containing my CMPS 240 (Operating Systems) Assignments.  

# Assignment 3
 - The assignment consists of writing a direct inter-process communication (IPC) mechanism using shared memory, following the worker/reducer model.  
 - Due date: `Friday, October 28 2022, 11:59 PM`  
 - For further details, see [Assignment_3.pdf](Assignment_3/Assignment_3.pdf)  

## Usage
### Selecting an input file
- Add the file to the input directory
- In src/main.c, modify `INPUT_FILE_NAME` to the file name (input.txt by default)

### Selecting words to be counted
- In src/main.c, modify `char *words[]` to contain the strings you want

### Enabling/Disabling verbose setting
- In Constants.h, set verbose to 0 (false) or 1 (true)

### Compiling
- Run the following command in the root directory
```sh
    make
```

### Running
- Run the following command in the root directory, where `N` is the number of worker processes.
```sh
    ./a.out <N>
```

# Assignment 4
 - The assignment consists of designing a synchronization model and testing it for multiple parameter values.  
 - Due date: `Monday, 28 November 2022, 11:59 PM`  
 - For further details, see [Assignment_4.pdf](Assignment_4/Assignment_4.pdf)  

# Platforms
Tested on Linux (Manjaro) and MacOS.

# License
Licensed under the [MIT](LICENSE) license.
