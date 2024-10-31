bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


[{
	"resource": "/home/gaz358/myprog/bpfgo/test.c",
	"owner": "C/C++: IntelliSense",
	"code": "158",
	"severity": 8,
	"message": "expression must be an lvalue or a function designator",
	"source": "C/C++",
	"startLineNumber": 85,
	"startColumn": 44,
	"endLineNumber": 85,
	"endColumn": 57
}]


=> [build 12/14] RUN make install-libbpf                                         32.7s
 => ERROR [build 13/14] RUN make                                                   0.6s
------
 > [build 13/14] RUN make:
#0 0.287 rm -f .output/main.*
#0 0.288 make -C kernel main
#0 0.290 make[1]: Entering directory '/app/kernel'
#0 0.305   MKDIR    ../.output
#0 0.308   CC       ../.output/main.bpf.o
#0 0.321 In file included from main.bpf.c:3:
#0 0.321 vmlinux.h:5: warning: ignoring '#pragma clang attribute' [-Wunknown-pragmas]
#0 0.321     5 | #pragma clang attribute push (__attribute__((preserve_access_index)), apply_to = record)
#0 0.321       | 
#0 0.628 In file included from main.bpf.c:3:
#0 0.628 vmlinux.h:150444: warning: ignoring '#pragma clang attribute' [-Wunknown-pragmas]
#0 0.628 150444 | #pragma clang attribute pop
#0 0.628        | 
#0 0.628 main.bpf.c:5:10: fatal error: bpf/bpf_core_read.h: No such file or directory
#0 0.628     5 | #include "bpf/bpf_core_read.h"
#0 0.628       |          ^~~~~~~~~~~~~~~~~~~~~
#0 0.628 compilation terminated.
#0 0.637 make[1]: *** [Makefile:127: ../.output/main.bpf.o] Error 1
#0 0.637 make[1]: Leaving directory '/app/kernel'
#0 0.638 make: *** [Makefile:26: build-bpf] Error 2
------
Dockerfile:33
--------------------
  31 |     
  32 |     RUN make install-libbpf
  33 | >>> RUN make
  34 |     RUN make testunit
  35 |     
--------------------
ERROR: failed to solve: process "/bin/sh -c make" did not complete successfully: exit code: 2
gaz358@gaz358-BOD-WXX9:~/myprog/trayce_agent$ 

