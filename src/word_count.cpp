#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

enum opts { ALL, WORDS, CHARACTERS, LINES, BYTES, VERBOSE, STD_IN, MAX_OPTS };
std::bitset<MAX_OPTS> g_opts{};
std::fstream g_fs{};

void set_opt(char opt) {
    switch (opt) {
    case 'a':
        g_opts.set(opts::ALL);
        return;
    case 'w':
        g_opts.set(opts::WORDS);
        return;
    case 'c':
        g_opts.set(opts::CHARACTERS);
        return;
    case 'l':
        g_opts.set(opts::LINES);
        return;
    case 'b':
        g_opts.set(opts::BYTES);
        return;
    case 'v':
        g_opts.set(opts::VERBOSE);
        return;
    case 'i':
        g_opts.set(opts::STD_IN);
        return;
    default:
        return;
    }
}

void set_opt(std::string_view opt) {
    std::cout << opt << "\n";
    if (opt == "all")
        g_opts.set(opts::ALL);
    else if (opt == "words")
        g_opts.set(opts::WORDS);
    else if (opt == "chars")
        g_opts.set(opts::CHARACTERS);
    else if (opt == "lines")
        g_opts.set(opts::LINES);
    else if (opt == "bytes")
        g_opts.set(opts::BYTES);
    else if (opt == "verbose")
        g_opts.set(opts::VERBOSE);
    else if (opt == "std-in")
        g_opts.set(opts::STD_IN);
}

void set_opt_char(std::string_view opts) {
    for (auto& c : opts)
        set_opt(c);
}

void parse_opt(std::string_view str) {
    std::cout << str << '\n';
    if (str.size() == 1)
        return; // nothing

    if (str.substr(0, 2) == "--")
        set_opt(str.substr(2, str.size() - 2));
    else if (str[0] == '-')
        set_opt_char(str.substr(1, str.size() - 1));
}

void parse_flags(int argc, char *const argv[]) {
    for (int i{1}; i < argc - 1; ++i)
        parse_opt(argv[i]);

    if (g_opts.none())
        g_opts.set(opts::ALL);
}

void parse_file(int argc, char *const argv[]) {
    std::string f{argv[argc - 1]};
    g_fs.open(f);

    if (std::filesystem::is_directory(f))
        throw std::runtime_error(f + " is a directory!\n");
    else if (!std::filesystem::exists(f))
        throw std::runtime_error(f + " does not exist!\n");
    else if (!g_fs)
        throw std::runtime_error(f + " failed to open!\n");
    else // assume it is not a regular file
        throw std::runtime_error(f + " needs to be a regular file!\n");
}

void count() {}

int main(int argc, char *argv[]) {
    parse_flags(argc, argv);
    parse_file(argc, argv);
}