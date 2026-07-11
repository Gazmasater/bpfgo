# eBPF compatibility results — older kernels — 2026-07-11

Object: `bpf_x86_bpfel.o`

Architecture: `x86_64`

Compiler: Ubuntu Clang 18.1.3

Verifier host: Ubuntu kernel `6.8.0-134-generic`

| Target Ubuntu kernel BTF | CO-RE relocation | Programs accepted | Maximum processed instructions |
|---|---:|---:|---:|
| `4.15.0-213-generic` | PASS | 26/26 | 73,338 |
| `4.18.0-25-generic` | PASS | 26/26 | 73,338 |
| `5.0.0-65-generic` | PASS | 26/26 | 73,338 |
| `5.3.0-76-generic` | PASS | 26/26 | 73,338 |
| `5.4.0-91-generic` | PASS | 26/26 | 73,338 |
| `5.8.0-63-generic` | PASS | 26/26 | 73,338 |

The BTF files were obtained from the Aqua Security BTFHub Archive. The largest
program was `trace_sendmmsg_exit`. No CO-RE relocation or verifier rejection
occurred.

These results prove relocation compatibility with each target kernel BTF. The
relocated programs were verified by the running `6.8.0-134-generic` kernel.
Complete runtime compatibility still requires booting each target kernel and
attaching all tracepoints.

Detailed per-program output is stored in `verifier-logs.tar.xz`. Kernel images
and BTF files are intentionally excluded from the repository.

Ubuntu 5.11 generic BTF entries in the archive were marked as failed and were
therefore not reported as tested.
