# Projet S6 - Carcassonne

### Description

This project is the second programming project at ENSEIRB.
Project period : 23/03/2018 - 27/04/2018

Jalon 1: until commit #22


### Authors

* BEN ABDELJELIL Hadhami [hbenabdelje@enseirb-matmeca.fr](mailto:hbenabdelje@enseirb-matmeca.fr)
* JOLIVET Arthur [ajolivet@enseirb-matmeca.fr](mailto:ajolivet@enseirb-matmeca.fr)
* OSTERTAG Marie [mostertag@enseirb-matmeca.fr](mailto:mostertag@enseirb-matmeca.fr)
* ROSEAU Baptiste [broseau@enseirb-matmeca.fr](mailto:broseau@enseirb-matmeca.fr)


### How to build

#### Requirements

To build project, the following commands must be installed and reachable from `PATH`:
- make
- gcc

Optional commands for an alternative way to build
- cmake
- doxygen

#### Instructions

You can build the project from `Makefile`.
The following commands are available :
```
make [help]  #To show the available commands
make build   #To build the project
make test    #To build and start the tests
make memory  #To build and start the tests with valgrind check
make install #To install the project in /install
make clean   #To remove objects files, executables and installation
make run     #To run program once installed (make install before required)
make docs    #To generate documentation with doxygen in /doc (open /doc/html/index.html in your browser)
```
    
You can also build the project from `CMakeLists.txt`.
```
Two shell files automatically build and clean project:
- cmake_build.sh: Creates 3 repositories and build the project
    - /build      cmake generated files and project binaries
    - /bin        Only executables (server, clients libraries and tests)
    - /bin        Documentation generated by Doxygen
- cmake_clean.sh: Cleans everything built by cmake_build.sh
```

### Code conventions

- Put a space after a comma and between operators
- Put structures names, variables and functions in lower case with underscores to separate words
- Put functions/structures start bracket on the line after the prototype
    (small structures, getters, setters can be inline though)
- Put statements/loops start bracket on the same line separated by a space
- Don't add spaces between function name and the argument list in parenthesis
- You don't have to put brackets for one-line statements
- Put two lines between each function in headers
- Put one line between each function in implementation
- Use Javadoc style documentation
- Write `NOTE`, `TODO` or `FIXME` in uppercase in comments when you respectively need to notice, do or fix something
- Don't put spaces after an open parenthesis / before a close parenthesis
