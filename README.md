# ucol
cli tool for selecting columns from a text file

### introduction

In the spirit of small and focused command-line programs that comprise
the (*)nix shell,
`ucol` provides a simple way to select columns from text data.

### example

```
> ls -l

total 40
-rw-r--r--   1 bob  staff   1068 Jun 12 05:38 LICENSE
-rw-------   1 bob  staff     81 Jun 12 05:45 Makefile
-rw-r--r--@  1 bob  staff    226 Jun 12 19:03 README.md
-rw-------@  1 bob  staff   4539 Jun 12 18:42 ucol.c

> ls -l | ucol 5 1 5

 total
1068 -rw-r--r-- 1068
81 -rw------- 81
1045 -rw-r--r--@ 1045
4539 -rw-------@ 4539


> ls -l | ucol 5 1 5 | tail -n +2

1068 -rw-r--r-- 1068
81 -rw------- 81
1032 -rw-r--r--@ 1032
4539 -rw-------@ 4539
```

Here `ucol` participates, with other cli tools,
to extract a few columns
from the results of the `ls` command.
Note that the columns can be specified in any order,
and can be specified more than once.

### syntax
```
ucol [-dDno] column-numbers [filename]
```

### options
```
  -dc       use 'c' as input column delimiter
            (default whitespace)
  -Dc       use 'c' as output column delimiter
            (default space)
  -n        allow null columns

            Normally, when multiple column delimiters are encountered in
            sequence, they are treated as a single delimiter. If null columns
            are allowed, each column delimiter starts a new column, and
            sequential delimiters indicate zero-length columns.

  -oFile    output filename
```
