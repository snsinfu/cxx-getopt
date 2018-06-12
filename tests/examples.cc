/*
 * Copyright snsinfu 2018.
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <vector>

#include <catch.hpp>

#include "../getopt.hpp"


TEST_CASE("example: sed")
{
    char argv_0[] = "sed";
    char argv_1[] = "-i.bak";
    char argv_2[] = "-E";
    char argv_3[] = "/^#/d";
    char* argv_array[] = {argv_0, argv_1, argv_2, argv_3, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;

    std::string opt_extension;
    bool opt_extended = false;

    for (int ch; (ch = getopt(argc, argv, "i:E")) != -1; ) {
        switch (ch) {
          case 'i':
            opt_extension = getopt.optarg;
            break;

          case 'E':
            opt_extended = true;
            break;

          default:
            CHECK(false);
        }
    }

    CHECK(opt_extension == ".bak");
    CHECK(opt_extended);

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 1);
    CHECK(argv[0] == std::string(argv_3));
}
