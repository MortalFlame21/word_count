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

    void operator+=(info_t o) {
        count.words += o.count.words;
        count.characters += o.count.characters;
        count.lines += o.count.lines;
        count.bytes += o.count.bytes;
    }
};

enum opts { WORDS, CHARACTERS, LINES, BYTES, VERBOSE, STD_IN, MAX_OPTS };

// constants
const std::bitset<opts::MAX_OPTS> OPT_COUNTS{(1 << opts::WORDS) |
                                             (1 << opts::CHARACTERS) |
                                             (1 << opts::LINES) | (1 << opts::BYTES)};

// globals
std::bitset<opts::MAX_OPTS> g_opts{};
std::deque<std::filesystem::path> g_paths{};

void set_opt(std::string_view opt) {
    if (opt == "all" || opt == "a")
        g_opts |= OPT_COUNTS;
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
        for (size_t i{}; i < str.size(); ++i)
            set_opt(str.substr(i, 1));
}

void parse_flags(int argc, char* const argv[]) {
    for (int i{1}; i < argc; ++i) {
        std::string_view arg{argv[i]};
        // only consider flags before "--", end of options.
        if (arg == "--")
            break;
        parse_opt(argv[i]);
    }

    if ((g_opts & OPT_COUNTS).none())
        g_opts |= OPT_COUNTS;
}

void parse_files(int argc, char* const argv[]) {
    namespace fs = std::filesystem;

    bool eoo_marker{};
    for (int i{1}; i < argc; ++i) {
        std::string arg{argv[i]};

        // parse next args as actual file name
        if (arg == "--" && !eoo_marker)
            eoo_marker = true;
        // after "--" parse all args as files
        // OR not seen "--" or before it, skip flags, flags begin with '-'.
        else if (eoo_marker || !arg.starts_with('-'))
            g_paths.push_back(arg);
    }

    if (g_paths.empty())
        g_opts.set(opts::STD_IN);
}

count_t count(const std::istream& in) {
    count_t ct{};
    // either read bytes from ifs or make user input via std::cin
    std::stringstream ss{};
    ss << in.rdbuf();
    auto str{ss.str()};

    bool in_word{};
    for (auto c : str) {
        c = static_cast<unsigned char>(c);
        if (std::isalnum(c) || std::ispunct(c)) {
            ++ct.characters;
            in_word = true;
        } else if (std::isspace(c)) {
            ++ct.characters;
            if (in_word) {
                ++ct.words;
                in_word = false;
            }
            if (c == '\n')
                ++ct.lines;
        }
    }
    if (in_word)
        ++ct.words;
    ct.bytes = static_cast<int>(str.size());

    return ct;
}

void print(const info_t& info) {
    std::cout << info.label;

    if (g_opts.test(opts::VERBOSE)) {
        std::cout << '\n';
        if (g_opts.test(opts::WORDS))
            std::cout << "\twords: " << info.count.words << '\n';
        if (g_opts.test(opts::CHARACTERS))
            std::cout << "\tcharacters: " << info.count.characters << '\n';
        if (g_opts.test(opts::LINES))
            std::cout << "\tnewlines: " << info.count.lines << '\n';
        if (g_opts.test(opts::BYTES))
            std::cout << "\tbytes: " << info.count.bytes << '\n';
    } else {
        std::cout << " | ";
        if (g_opts.test(opts::WORDS))
            std::cout << info.count.words << ' ';
        if (g_opts.test(opts::CHARACTERS))
            std::cout << info.count.characters << ' ';
        if (g_opts.test(opts::LINES))
            std::cout << info.count.lines << ' ';
        if (g_opts.test(opts::BYTES))
            std::cout << info.count.bytes;
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]) {
    parse_flags(argc, argv);
    parse_files(argc, argv);

    namespace fs = std::filesystem;

    std::vector<info_t> counts{};
    info_t sum_count{"total"};

    // print the files (if at all)
    for (auto& path : g_paths) {
        if (std::fstream ifs{path};
            (fs::is_regular_file(path) || fs::is_symlink(path)) && ifs) {
            counts.push_back({path.string(), count(ifs)});
            print(counts.back());
            sum_count += counts.back();
        } else {
            std::cout << "WARNING: " << path
                      << " is not a valid file path or bad file.\n";
        }
    }

    // print std-in (if at all)
    if (g_opts.test(opts::STD_IN))
        print({"std-in", count(std::cin)});

    // print total
    if (counts.size() > 1)
        print(sum_count);
}