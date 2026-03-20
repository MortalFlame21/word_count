# Word Count

Print word, character count, newline, byte count from a line. A UNIX `wc` subset clone.

## Usage

```shell
make

./word_count <OPTIONS> <FILE>
```

The `<OPTIONS>` are used to select which counts are printed, they are printed in respective order within the list.

With no count `<OPTIONS>`, `-a`, is by default.

If `-i` is included with count `<OPTIONS>`, standard input will be used.

`<FILE>` is the last argument. In conjunction with `-i`, standard input will be used upon `<FILE>`, then standard input. If omitted, standard input is thus used.

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
test.txt 1, 1, 1, 1
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
./word_count -i test.txt
test.txt 1, 1, 1, 1
> _
```
