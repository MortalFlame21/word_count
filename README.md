# Word Count

Print word count, character count, newline count, byte count from a file. A UNIX `wc` subset clone.

## Requirements

Although simple, this program needs a C++20 compliant compiler and GNU make 4.x+ to compile.

## Usage

```shell
make

./build/word_count <OPTIONS> <FILE>
```

The `<OPTIONS>` are used to select which counts are printed, they are printed in respective order within the list.

With no count `<OPTIONS>`, `-ai`, is by default.

If `-i` is included with count `<OPTIONS>`, standard input will be used.

`<FILE>` is a list of existing file paths. In conjunction with `-i`, files will be printed then standard input. If omitted, standard input is thus used.

To mark end of `<OPTIONS>`. Use `--`, subsequent arguments will be treated as `<FILE>`.

```shell
<OPTIONS>:
    -a, --all
        Print all options.
    -w, --words
        Print the word counts.
    -c, --chars
        Print the character counts.
    -l, --lines
        Print the newline counts.
    -b, --bytes
        Print the byte counts.
    -v, --verbose
        Print the option name.
    -i, --std-in
        Read standard input.
```

### Example

```shell
./word_count test.txt
test.txt | 1, 1, 1, 1
```

```shell
./word_count -v test.txt
test.txt
    words: 1
    characters: 1
    newlines: 1
    bytes: 1
```

```shell
./word_count -vi .clang-format ~/.viminfo
.clang-format
        words: 22
        characters: 210
        newlines: 13
        bytes: 210
/home/user/.viminfo
        words: 1239
        characters: 12077
        newlines: 495
        bytes: 12077
hello world!std-in
        words: 2
        characters: 12
        newlines: 0
        bytes: 12
total
        words: 1261
        characters: 12287
        newlines: 508
        bytes: 12287
```
