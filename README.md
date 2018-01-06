These are my solutions to the puzzles on
[cryptopals.com](http://cryptopals.com/), implemented in C++. This repository
probably isn't useful to you, unless you want to compare solutions.

This is a standard autotools project. There is a linking dependency against
OpenSSL. Compile it like this:

```bash
# Compile the code.
$ ./autogen.sh
$ ./configure
$ make
```

This will produce an executable at `./src/cryptopals` which is essentially a
test case of the solutions. There are a few different modes of operation:

```bash
# Run all tests.
$ ./src/cryptopals

# Run all tests from set 1.
$ ./src/cryptopals 1

# Run just the test for set 1 problem 6.
$ ./src/cryptopals 1 6
```
