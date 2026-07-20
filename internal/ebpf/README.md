# eBPF source layout

`trace.bpf.c` is the sole bpf2go entry point. It includes every file below as
one BPF translation unit, so all probes retain access to the shared maps and
the ABI remains identical to the generated Go bindings.

- `abi.h`: event structs, syscall argument structs, constants, and kernel headers.
- `maps.h`: BPF maps and the program licence.
- `helpers.h`: verifier-safe helpers shared by probes.
- `probes/`: syscall and network tracepoint handlers grouped by responsibility.

Regenerate the Go bindings and object from the project root:

```sh
go generate ./...
```
