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
3. Read the source, Luke. (The CPython source that is.)


# Building


I use `g++ 5.2.0` and `make`. Run `make` in this directory.


# Floating point numbers


Python's floating point numbers are all doubles. Marshalled doubles are
written in "little-endian" format, in particular 1 sign bit followed by 11
exponent bits and 52 mantissa bits. If your C/C++ implementation uses
a different floating-point format or your system is big-endian then the bits
will have to be massaged into the right format.


# Reverse engineering .pyc files


Facts about pyc files ([from here](http://nedbatchelder.com/blog/200804/the_structure_of_pyc_files.html)):

1. The first line is a 4-byte magic number.
2. The link says the next 4-bytes are a modification timestamp for the file.
    However I found that it was actually the next 8 bytes (and I'm not sure it was a
    timestamp) with CPython 3.5.
3. The rest is a marshalled code object -- a sequence of bytes.

In CPython 3.5, the first byte of marshalled objects defines a type that determines
how it should be unmarshalled, for example TYPE_CODE and TYPE_STRING.
TYPE_CODE, which corresponds to the code object in pyc files, has several
other marshalled objects in it. For example the first several bytes correspond to
32-bit integers, followed by a marshalled bytestring (of TYPE_STRING) which is
the actual bytecode. The CPython unmarshalling algorithm will recusrively call
its unmarshalling function on subsections of the marshalled code object.

In order to parse a pyc file, I first determined the types of each code object
section. Consider the following REPL snippet:

```python
>>> import marshal
>>> def foo(a, b): return a+b
...
>>> FLAG_REF = b'\x80'
>>> bytes([marshal.dumps(foo.__code__.co_code)[0] & ~REF_FLAG[0]])
b's'
```

The result of the last line is the byte that CPython uses to represent the type of
the marshalled object. For instance, `s` corresponds to TYPE_STRING.
The FLAG_REF stuff comes from looking at the CPython source.
It's an internal flag used to indicate that the upcoming object will appear
more than once, so that it should be stored in a list for reference,
obviating the need to unmarshal it fully more than once.
The upshot is that I can easily find the type byte for each code object
member and look at `marshal.c` to determine how to
unmarshal the whole code object. For example:

```python
>>> bytes([marshal.dumps(foo.__code__.co_consts)[0] & ~REF_FLAG[0]])
b')'
```

This tells me that the `co_consts` member is TYPE_SMALL_TUPLE.

In the end, unmarshalling a code object turns out to be about as complicated
as unmarshalling arbitrary Python objects, since it contains a bunch of other
marshalled objects.
