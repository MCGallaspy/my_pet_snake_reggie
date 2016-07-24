# What is this?

A C++ implementation of a Python 3.5 interpreter. It's very incomplete right now.

# Why?

First off, it's a bad idea to re-implement a project from scratch.  Moreover,
similar projects (like [CoCo](https://github.com/kentdlee/CoCo)) already exist.
But sometimes it's fun to act on bad ideas. ;)

Really there are two reasons:

1. Learn more about Python.
2. Undertake a non-trivial C++ project for practice.

# How will you do it?

1. Draw inspiration from existing projects.
2. Reference Python's [excellent documentation](https://docs.python.org/3/reference/index.html).

# Building

Using g++ 5.2.0:
`g++ PyObj.hpp instructions.hpp virtual_machine.hpp virtual_machine.cpp main.cpp -Wall -Werror -fno-rtti -std=c++14`
