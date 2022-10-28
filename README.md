# Assignment 3
Repository containing my CMPS 240 (Operating Systems) Assignment 3.  
Due date: `October 28, 2022 at 23:59 PM`

## General description
The assignment consists of writing a direct inter-process communication (IPC) mechanism using shared memory, following the worker/reducer model.
For further details, see [Assignment_3.pdf](Assignment_3.pdf)  

## Usage
### Selecting an input file
- Add the file to the input directory
- In src/main.c, modify `INPUT_FILE_NAME` to the file name (input.txt by default)

### Selecting words to be counted
- In src/main.c, modify `char *words[]` to contain the strings you want

### Enabling/Disabling verbose setting
- In Constants.h, set verbose to 0 (false) or 1 (true)

### Compiling
- Run one of the following commands in the root directory
```sh
    make
```
or
```sh
    gcc -o a.out src/reducer.c src/splitter.c src/worker.c src/main.c
```

### Running
- Run the following command in the root directory, where `N` is the number of worker processes.
```sh
    a.out <N>
```
## Platforms
Tested on Linux (Manjaro) and MacOS.

## License
Licensed under the [MIT](LICENSE) license.
