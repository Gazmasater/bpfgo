#define BPF_CORE_READ(src,a,__VA_ARGS__...) ({ ___type((src), a, ## __VA_ARGS__) __r; BPF_CORE_READ_INTO(&__r, (src), a, ## __VA_ARGS__); __r; })
BPF_CORE_READ() is used to simplify BPF CO-RE relocatable read, especially
when there are few pointer chasing steps.
E.g., what in non-BPF world (or in BPF w/ BCC) would be something like:
int x = s->a.b.c->d.e->f->g;
can be succinctly achieved using BPF_CORE_READ as:
int x = BPF_CORE_READ(s, a.b.c, d.e, f, g);

BPF_CORE_READ will decompose above statement into 4 bpf_core_read (BPF
CO-RE relocatable bpf_probe_read_kernel() wrapper) calls, logically
equivalent to:

const void *__t = s->a.b.c;
__t = __t->d.e;
__t = __t->f;
return __t->g;
Equivalence is logical, because there is a heavy type casting/preservation
involved, as well as all the reads are happening through
bpf_probe_read_kernel() calls using __builtin_preserve_access_index() to
emit CO-RE relocations.

N.B. Only up to 9 "field accessors" are supported, which should be more
than enough for any practical purpose.
Expands to:

({ typeof((conn_info)->sock_addr) __r; ({ bpf_probe_read_kernel((void *)(&__r), sizeof(*(&__r)), (const void *)__builtin_preserve_access_index(&((typeof(((conn_info))))(((conn_info))))->sock_addr)); }); __r; })



struct sockaddr_in *addr;
addr = BPF_CORE_READ(conn_info, sock_addr);
if (!addr) {
    bpf_printk("Failed to read sock_addr using BPF_CORE_READ\n");
    return 0;
}

