bpf2go -o trace_bpf.go trace_bpf.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ bpf2go -o trace_bpf.go trace.c
flag provided but not defined: -o
Usage: bpf2go [options] <ident> <source file> [-- <C flags>]

ident is used as the stem of all generated Go types and functions, and
must be a valid Go identifier.

source is a single C file that is compiled using the specified compiler
(usually some version of clang).

You can pass options to the compiler by appending them after a '--' argument
or by supplying -cflags. Flags passed as arguments take precedence
over flags passed via -cflags. Additionally, the program expands quotation
marks in -cflags. This means that -cflags 'foo "bar baz"' is passed to the
compiler as two arguments "foo" and "bar baz".

The program expects GOPACKAGE to be set in the environment, and should be invoked
via go generate. The generated files are written to the current directory.

Some options take defaults from the environment. Variable name is mentioned
next to the respective option.

Options:

  -cc binary
        binary used to compile C to BPF ($BPF2GO_CC) (default "clang")
  -cflags string
        flags passed to the compiler, may contain quoted arguments ($BPF2GO_CFLAGS)
  -go-package string
        package for output go file (default as ENV GOPACKAGE)
  -makebase directory
        write make compatible depinfo files relative to directory ($BPF2GO_MAKEBASE)
  -no-global-types
        Skip generating types for map keys and values, etc.
  -no-strip
        disable stripping of DWARF
  -output-dir string
        target directory of generated files (defaults to current directory)
  -output-stem string
        alternative stem for names of generated files (defaults to ident)
  -output-suffix string
        suffix in generated file names such as _test (default based on $GOFILE)
  -strip binary
        binary used to strip DWARF from compiled BPF ($BPF2GO_STRIP)
  -tags value
        Comma-separated list of Go build tags to include in generated files
  -target string
        clang target(s) to compile for (comma separated) (default "bpfel,bpfeb")
  -type Name
        Name of a type to generate a Go declaration for, may be repeated

Supported targets:
        bpf
        bpfel
        bpfeb
        386
        amd64
        arm
        arm64
        loong64
        mips
        ppc64
        ppc64le
        riscv64
        s390x
Error: flag provided but not defined: -o

