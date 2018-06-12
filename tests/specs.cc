/*
 * Copyright snsinfu 2018.
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <vector>

#include <catch.hpp>

#include "../getopt.hpp"


TEST_CASE("getopt parses nothing if no options are there")
{
    char argv_0[] = "cat";
    char argv_1[] = "a.dat";
    char argv_2[] = "b.dat";
    char* argv_array[] = {argv_0, argv_1, argv_2, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "abc")) != -1; ) {
        observed_opts.push_back(ch);
    }

    CHECK(observed_opts.size() == 0);

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 2);
    CHECK(argv[0] == std::string("a.dat"));
    CHECK(argv[1] == std::string("b.dat"));
}

TEST_CASE("getopt recognizes options")
{
    char argv_0[] = "command";
    char argv_1[] = "-b";
    char argv_2[] = "-a";
    char argv_3[] = "c.dat";
    char* argv_array[] = {argv_0, argv_1, argv_2, argv_3, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "abc")) != -1; ) {
        observed_opts.push_back(ch);
    }

    CHECK(observed_opts.size() == 2);
    CHECK(observed_opts.at(0) == 'b');
    CHECK(observed_opts.at(1) == 'a');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 1);
    CHECK(argv[0] == std::string("c.dat"));
}

TEST_CASE("getopt recognizes coalesced options")
{
    char argv_0[] = "command";
    char argv_1[] = "-abc";
    char* argv_array[] = {argv_0, argv_1, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "abc")) != -1; ) {
        observed_opts.push_back(ch);
    }

    CHECK(observed_opts.size() == 3);
    CHECK(observed_opts.at(0) == 'a');
    CHECK(observed_opts.at(1) == 'b');
    CHECK(observed_opts.at(2) == 'c');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt recognizes option argument")
{
    char argv_0[] = "command";
    char argv_1[] = "-o";
    char argv_2[] = "output";
    char argv_3[] = "-v";
    char* argv_array[] = {argv_0, argv_1, argv_2, argv_3, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;
    std::vector<char*> observed_optargs;

    for (int ch; (ch = getopt(argc, argv, "o:v")) != -1; ) {
        observed_opts.push_back(ch);
        observed_optargs.push_back(getopt.optarg);
    }

    CHECK(observed_opts.size() == 2);
    CHECK(observed_opts.at(0) == 'o');
    CHECK(observed_optargs.at(0) == std::string("output"));
    CHECK(observed_opts.at(1) == 'v');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt recognizes coalesced option argument")
{
    char argv_0[] = "command";
    char argv_1[] = "-vooutput";
    char* argv_array[] = {argv_0, argv_1, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;
    std::vector<char*> observed_optargs;

    for (int ch; (ch = getopt(argc, argv, "o:v")) != -1; ) {
        observed_opts.push_back(ch);
        observed_optargs.push_back(getopt.optarg);
    }

    CHECK(observed_opts.size() == 2);
    CHECK(observed_opts.at(0) == 'v');
    CHECK(observed_opts.at(1) == 'o');
    CHECK(observed_optargs.at(1) == std::string("output"));

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt does not treat single hyphen as an option")
{
    char argv_0[] = "paste";
    char argv_1[] = "-s";
    char argv_2[] = "-";
    char argv_3[] = "foo.dat";
    char* argv_array[] = {argv_0, argv_1, argv_2, argv_3, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "s")) != -1; ) {
        observed_opts.push_back(ch);
    }

    CHECK(observed_opts.size() == 1);
    CHECK(observed_opts.at(0) == 's');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 2);
    CHECK(argv[0] == std::string("-"));
    CHECK(argv[1] == std::string("foo.dat"));
}

TEST_CASE("getopt stops option parsing at double hyphen")
{
    char argv_0[] = "ls";
    char argv_1[] = "-1";
    char argv_2[] = "--";
    char argv_3[] = "-1";
    char* argv_array[] = {argv_0, argv_1, argv_2, argv_3, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "1")) != -1; ) {
        observed_opts.push_back(ch);
    }

    CHECK(observed_opts.size() == 1);

    argc -= getopt.optind;
    argv += getopt.optind;

    // The double hyphen itself is parsed out so we should not see it here.
    CHECK(argc == 1);
    CHECK(argv[0] == std::string("-1"));
}

TEST_CASE("getopt gracefully reports unrecognized option")
{
    char argv_0[] = "sort";
    char argv_1[] = "-rs";
    char* argv_array[] = {argv_0, argv_1, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_results;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "r")) != -1; ) {
        observed_results.push_back(ch);
        observed_opts.push_back(getopt.optopt);
    }

    CHECK(observed_results.size() == 2);
    CHECK(observed_results.at(0) == 'r');
    CHECK(observed_opts.at(0) == 'r');
    CHECK(observed_results.at(1) == '?');
    CHECK(observed_opts.at(1) == 's');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt gracefully reports missing option argument")
{
    char argv_0[] = "head";
    char argv_1[] = "-n";
    char* argv_array[] = {argv_0, argv_1, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_results;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, "n:")) != -1; ) {
        observed_results.push_back(ch);
        observed_opts.push_back(getopt.optopt);
    }

    CHECK(observed_results.size() == 1);
    CHECK(observed_results.at(0) == '?');
    CHECK(observed_opts.at(0) == 'n');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt returns special char for missing argument if instructed")
{
    char argv_0[] = "head";
    char argv_1[] = "-n";
    char* argv_array[] = {argv_0, argv_1, 0};

    int argc = static_cast<int>(sizeof(argv_array) / sizeof(*argv_array)) - 1;
    char** argv = argv_array;

    cxx::getopt getopt;
    std::vector<int> observed_results;
    std::vector<int> observed_opts;

    for (int ch; (ch = getopt(argc, argv, ":n:")) != -1; ) {
        observed_results.push_back(ch);
        observed_opts.push_back(getopt.optopt);
    }

    CHECK(observed_results.size() == 1);
    CHECK(observed_results.at(0) == ':');
    CHECK(observed_opts.at(0) == 'n');

    argc -= getopt.optind;
    argv += getopt.optind;

    CHECK(argc == 0);
}

TEST_CASE("getopt gracefully handles empty argv")
{
    char* argv_array[] = {0};

    int argc = 0;
    char** argv = argv_array;

    cxx::getopt getopt;
    CHECK(getopt(argc, argv, "") == -1);
}

TEST_CASE("getopt gracefully handles null argv")
{
    cxx::getopt getopt;
    CHECK(getopt(0, 0, "") == -1);
}
