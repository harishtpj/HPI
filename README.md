# HPI

[![using-c++](https://img.shields.io/badge/Using-C++%20-blue)](https://visualstudio.microsoft.com/vs/features/cplusplus/)
[![c++-version](https://img.shields.io/badge/C++-%2023-lightgreen)](https://isocpp.org/)
[![msvc++-version](https://img.shields.io/badge/MSVC-v19.36-purple)](https://visualstudio.microsoft.com/vs/features/cplusplus/)

A simple, procedural Psuedocode Interpreter

# About
HPI is a simple, procedural interpreter for the psuedocode designed by me. It is designed to be as easy to use 
as possible, with a lesser learning curve and is written in **C++**. Its syntax is derived mostly from 
languages such as **Python** and **Pascal**. This project is based on the **JLox** [interpreter](https://craftinginterpreters.com/) of crafting interpreters.

## Why "HPI"
It simply means - **H**arish's **P**suedocode **I**nterpreter.

# Notable features
- Simple and easy to learn syntax
- Newline-terminated statements
- Support for anonymous functions
- Support for dynamic input

# Requirements
This interpreter just depends only on the C++(v23) Standard Library.

# Installation
The easiest way to install this project is to download the source code and build it. Also, executables for 
major platforms has been provided in [releases](https://github.com/harishtpj/HPI/releases) page.

If you're going to use the precompiled binaries, make sure that you've added their location to your **PATH** 
environment variable for system-wide access.

# Usage
To run your first program, fire up you favourite text editor and copy the following lines to `hello.hpi` file:

```python
# A Simple Hello, World! program

println "Hello, world!"
```

Then open a new terminal in the same directory as the file and run:
```bash
$ hpi hello.hpi
```

For windows users: just run the same command.

You'll see the following output on your terminal:

```
Hello, world!
```

# Support
This project has a documentation page on [GitHub](https://github.com/harishtpj/HPI/docs). If you've any issues 
or suggestions, you can [open an issue](https://github.com/harishtpj/HPI/issues) on GitHub.

# Contributing
Contributors are welcome. If you suggest any improvement in this project, please create a 
pull request on GitHub. 

# Authors
This project is currently maintained by **M.V.Harish Kumar**.

# License
#### Copyright © 2023 [M.V.Harish Kumar](https://github.com/harishtpj). <br>
#### This project is [MIT](https://github.com/harishtpj/HPI/blob/master/LICENSE) licensed.

# Roadmap
- [x] Create a basic, functional interpreter
- [ ] Make a web editor for HPI
- [ ] Refine syntax and logic

# Project status
This project is complete with its syntax and functionality. However changes will be made to the functionality 
and logic of this project.
