# eyepatch
Creates a patching standard from a list of lighting fixtures.

**This project is part of the inventor ecosystem, which you can read about [here]().**

## Installation
You can install from source, from an install script, or from Homebrew.

### Binary
**1.** Go to the releases page to find latest binary

**2.** Run it
```
$ ./[binaryname]
```
replace binary name with the name of the downloaded binary

### Homebrew
**1.** Just use:
```
$ brew install eyepatch
```

### Source
**Here be dragons! You will likely not get a stable release. Be careful.**

**1.** Download the source from Github.

**2.** Unzip the downloaded file.
```
$ unzip eyepatch-master.zip
```

**3.** CD into the directory and compile `main.cpp` with:
```
$ cd eyepatch-master
$ gcc main.cpp -o eyepatch
```

**4.** Move the `eyepatch` executable to a sensible location in your $PATH.
```
$ sudo mv ./eyepatch /usr/local/bin/eyepatch
```

**5.** Check you can run it with `eyepatch`.
```
$ cd ~
$ eyepatch
```
