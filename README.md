Header-only getopt for C++
--------------------------

![C++03][cxx-badge]
![Boost License][license-badge]
[![Build Status][travis-badge]][travis-url]

Single-file, header-only library of a [POSIX getopt][posix-getopt] workalike.

```c++
#include <getopt.hpp>

// command -v -o arg ...
int main(int argc, char** argv)
{
    ext::getopt getopt;

    bool verbose = false;
    char const* output = "default";

    for (int opt; (opt = getopt(argc, argv, "vo:")) != -1; ) {
        switch (opt) {
          case 'v':
            verbose = true;
            break;

          case 'o':
            output = getopt.optarg;
            break;

          case '?':
            // Error
        }
    }

    argc -= getopt.optind;
    argv += getopt.optind;

    // Now you can access positional arguments: argv[0], ..., argv[argc - 1]
}
```

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-03-orange.svg
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/snsinfu/cxx-getopt.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/cxx-getopt
[posix-getopt]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html

## Installation

Just download [getopt.hpp][getopt.hpp] into your include directory.

License text is included in the header file and the license is permissive (the
Boost License), so you do not need to do anything on redistribution.

[getopt.hpp]: https://github.com/snsinfu/cxx-getopt/raw/master/getopt.hpp

## Testing

```console
git clone https://github.com/snsinfu/cxx-getopt.git
cd cxx-getopt/tests
make
```

## License

Boost Software License, Version 1.0.
