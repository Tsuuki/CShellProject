# Shellter : a bash that welcome you !

This is the shellter project : a bash made for beginners !

## Features

This shell have the following command implemented : 
 * `auhors` : Display a picture of the authors
 * `alias` : manage shell-internal aliases
    * See `alias -h` or `alias --help` for more information
 * `cd` : Change directory
 * `echo` : Echo the text in parameter
 * `exit` : Exit shell
 * environment varibles : manage environment variables
     * `addenv`: Add the environment variable. Use : TEST=TEST
     * `printenv` : Print all the environment variables
     * `delenv` : Delete the environment variable in parameter
 * `fg` : Make the number of the sleeped process in foreground
 * `history` : Print the history of all commands
    * You can use arrow key to navigate in command history
 * `pwd` : Print the working directory

All other commands aren't build-in commands such as 

```
ls
cat
grep
etc...
```

With the following operators and redirections : `&& & || | > >> < << ;`



## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

You will need

```
Git - pull the project
Linux environment - launch the project
```

And all the following package (you can use the make install when you pulled the project)

```
lcov
doxygen
graphviz 
groff
```

### Installing

Shellter is easy to use, all you have to do is clone the project with the following command

```
git clone https://github.com/Tsuuki/CShellProject.git
```
Be sure to use the install command from the make to have all the required packages

```
make install
```

## Running the project

Go inside the CSHellProject folder with 

```
cd CSHellProject
```

and execute the main sh file with 

```
./bin/shellter
```

or if you want to use the batch mode : 

```
./bin/shellter -c "Your command"
```

## Running the tests

To run the test coverage you can use 

```
make gcov
```

or if you want the full package (code, gcov and doc) use 

```
make package
```

## Built With

* [C](http://devdocs.io/c/) - Language

## Authors

* **Quentin Dell** - *Developer* - [Arcahyxe](https://github.com/Arcahyxe)
* **Jordan Hiertz** - *Developer* - [Tsuuki](https://github.com/Tsuuki)

## License

Shellter is Copyright (C) 2018 under MIT License

## Acknowledgments

* This is a school project
* We learnt a lot of new things
* We tried our best to make the better project !

## Getting lost ?

Run the following command

```
make man
```

```
man shellter
```