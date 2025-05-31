
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force




_______________________________________________________________________________________________


struct trace_info {
    struct in_addr  srcIP;     // 4 байта
    struct in_addr  dstIP;     // 4 байта
    struct in6_addr srcIP6;    // 16 байт
    struct in6_addr dstIP6;    // 16 байт
    __u32           pid;       // 4 байта
    __u32           proto;     // 4 байта
    __u16           sport;     // 2 байта
    __u16           dport;     // 2 байта
    __u16           family;    // 2 байта
    __u8            sysexit;   // 1 байт
    __u8            state;     // 1 байт
    char            comm[32];  // 32 байта
} __attribute__((packed));


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)
/home/gaz358/myprog/bpfgo/trace.c:192:38: warning: taking address of packed member 'dstIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  192 |         if (BPF_CORE_READ_USER_INTO(&info.dstIP6,
      |                                      ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:362:8: note: expanded from macro 'BPF_CORE_READ_USER_INTO'
  362 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:250:27: note: expanded from macro 'bpf_core_read_user'
  250 |         bpf_probe_read_user(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                  ^~
/home/gaz358/myprog/bpfgo/trace.c:302:38: warning: taking address of packed member 'srcIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  302 |         if (BPF_CORE_READ_USER_INTO(&info.srcIP6,
      |                                      ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:362:8: note: expanded from macro 'BPF_CORE_READ_USER_INTO'
  362 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:250:27: note: expanded from macro 'bpf_core_read_user'
  250 |         bpf_probe_read_user(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                  ^~
/home/gaz358/myprog/bpfgo/trace.c:418:33: warning: taking address of packed member 'dstIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  418 |         if (BPF_CORE_READ_INTO(&info.dstIP6, &sa6, sin6_addr.in6_u.u6_addr32) < 0)
      |                                 ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
/home/gaz358/myprog/bpfgo/trace.c:534:33: warning: taking address of packed member 'srcIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  534 |         if (BPF_CORE_READ_INTO(&info.srcIP6, &sa6, sin6_addr.in6_u.u6_addr32) < 0)
      |                                 ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
/home/gaz358/myprog/bpfgo/trace.c:619:29: warning: taking address of packed member 'srcIP' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  619 |         BPF_CORE_READ_INTO(&info.srcIP.s_addr, ctx, saddr);
      |                             ^~~~~~~~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
/home/gaz358/myprog/bpfgo/trace.c:620:29: warning: taking address of packed member 'dstIP' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  620 |         BPF_CORE_READ_INTO(&info.dstIP.s_addr, ctx, daddr);
      |                             ^~~~~~~~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
/home/gaz358/myprog/bpfgo/trace.c:635:29: warning: taking address of packed member 'dstIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  635 |         BPF_CORE_READ_INTO(&info.dstIP6, ctx, daddr_v6);
      |                             ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
/home/gaz358/myprog/bpfgo/trace.c:636:29: warning: taking address of packed member 'srcIP6' of class or structure 'trace_info' may result in an unaligned pointer value [-Waddress-of-packed-member]
  636 |         BPF_CORE_READ_INTO(&info.srcIP6, ctx, saddr_v6);
      |                             ^~~~~~~~~~~
/usr/include/bpf/bpf_core_read.h:352:8: note: expanded from macro 'BPF_CORE_READ_INTO'
  352 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ^~~
/usr/include/bpf/bpf_core_read.h:342:60: note: expanded from macro '___core_read'
  342 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |                                                                   ^~~
/usr/include/bpf/bpf_core_read.h:336:14: note: expanded from macro '___core_read0'
  336 |         ___read(fn, dst, ___type(src), src, a);
      |                     ^~~
/usr/include/bpf/bpf_core_read.h:317:34: note: expanded from macro '___read'
  317 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |                                         ^~~
/usr/include/bpf/bpf_core_read.h:246:29: note: expanded from macro 'bpf_core_read'
  246 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                    ^~
8 warnings generated.
Compiled /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.go
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 










