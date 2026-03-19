# Word Count

Print word, character count, newline, byte count from a line. A UNIX `wc` subset clone.

## Usage

```shell
make

./word_count <OPTIONS> <FILE>
```

The `<OPTIONS>` are used to select which counts are printed, they are in respective order within the list

With no count `<OPTIONS>`, `-a`, is by default.

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
test.txt 1 | 1 | 1 | 1
```

```shell
./word_count -v test.txt
test.txt
    words: 1
    characters: 1
    newlines: 1
    bytes: 1
```
