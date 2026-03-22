#include <bitset>
#include <cctype>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// classes & enums
struct count_t {
    int words{};
    int characters{};
    int lines{};
    int bytes{};
};

struct info_t {
    std::string label{};
    count_t count{};
};

enum opts { WORDS, CHARACTERS, LINES, BYTES, VERBOSE, STD_IN, MAX_OPTS };

// constants
const std::bitset<opts::MAX_OPTS> OPT_ALL{(1 << opts::WORDS) | (1 << opts::CHARACTERS) |
                                          (1 << opts::LINES)};

// globals
std::bitset<opts::MAX_OPTS> g_opts{};
std::deque<std::filesystem::path> g_paths{};
std::vector<info_t> g_counts{};

void set_opt(std::string_view opt) {
    if (opt == "all" || opt == "a")
        g_opts |= OPT_ALL;
    else if (opt == "words" || opt == "w")
        g_opts.set(opts::WORDS);
    else if (opt == "chars" || opt == "c")
        g_opts.set(opts::CHARACTERS);
    else if (opt == "lines" || opt == "l")
        g_opts.set(opts::LINES);
    else if (opt == "bytes" || opt == "b")
        g_opts.set(opts::BYTES);
    else if (opt == "verbose" || opt == "v")
        g_opts.set(opts::VERBOSE);
    else if (opt == "std-in" || opt == "i")
        g_opts.set(opts::STD_IN);
}

void parse_opt(std::string_view str) {
    if (str.size() == 1)
        return; // nothing

    // parse whole word
    if (str.starts_with("--"))
        set_opt(str.substr(2, str.size() - 2));
    // parse individual characters of word
    else if (str.starts_with('-'))
        for (int i{}; i < str.size(); ++i)
            set_opt(str.substr(i, 1));
}

void parse_flags(int argc, char* const argv[]) {
    for (int i{1}; i < argc; ++i) {
        std::string_view arg{argv[i]};
        // only consider flags before "--"
        if (arg == "--")
            break;
        parse_opt(argv[i]);
    }

    if (g_opts.none())
        g_opts |= OPT_ALL;
}

void parse_files(int argc, char* const argv[]) {
    namespace fs = std::filesystem;

    bool parse_as_files{};
    for (int i{1}; i < argc; ++i) {
        std::string arg{argv[i]};

        // parse next args as actual file name
        if (arg == "--" && !parse_as_files)
            parse_as_files = true;
        // after "--" parse all args as files
        // OR not seen "--" or before it, skip flags, flags begin with '-'.
        else if (parse_as_files || !arg.starts_with('-'))
            g_paths.push_back(arg);
    }

    if (g_paths.empty())
        g_opts.set(opts::STD_IN);
}

count_t count(std::istream& in) {
    count_t ct{};
    // either read bytes from ifs or make user input via std::cin
    std::stringstream ss{};
    ss << in.rdbuf();
    auto str{ss.str()};

    for (bool in_word{}; const auto& c : str) {
        if (std::isalnum(c)) {
            ++ct.characters;
            in_word = true;
        } else if (std::isspace(c)) {
            ++ct.characters;
            if (in_word)
                ++ct.words;
            if (c == '\n')
                ct.lines;
            in_word = false;
        }
    }
    ct.bytes = str.size();

    return ct;
}

void print(const info_t& info) {
    if (g_opts.test(opts::VERBOSE)) {
        std::cout << info.label << '\n';
        std::cout << "\twords: " << info.count.words << '\n';
        std::cout << "\tcharacters: " << info.count.characters << '\n';
        std::cout << "\tnewlines: " << info.count.lines << '\n';
        std::cout << "\tbytes: " << info.count.bytes << '\n';
    } else {
        std::cout << info.label << " | " << info.count.words << ", "
                  << info.count.characters << ", " << info.count.lines << ", "
                  << info.count.bytes << '\n';
    }
}

int main(int argc, char* argv[]) {
    parse_flags(argc, argv);
    std::cout << g_opts << '\n';
    parse_files(argc, argv);

    namespace fs = std::filesystem;

    for (auto& path : g_paths) {
        if (std::fstream ifs{path}; !fs::is_regular_file(path) || !fs::is_symlink(path))
            print({path.string(), count(ifs)});
        else
            std::cout << "WARNING: " << path
                      << " is not a valid file path or bad file.\n";
    }

    if (g_opts.test(opts::STD_IN))
        print({"std-in", count(std::cin)});
}