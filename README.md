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
test case of the solutions. If everything is working correctly, the following
command should print "OK" for each puzzle and then exit with status 0:

```bash
# Run the test suite.
$ ./src/cryptopals
```
