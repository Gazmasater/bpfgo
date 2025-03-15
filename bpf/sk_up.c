//go:build ignore

#include <linux/bpf.h>

#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>




// When invoked BPF sk_lookup program can select a socket that will receive the incoming packet 
// by calling the bpf_sk_assign() BPF helper function.
// Hooks for a common attach point (BPF_SK_LOOKUP) exist for both TCP and UDP.
SEC("sk_lookup")
int echo_dispatch(struct bpf_sk_lookup *ctx)
{
	return SK_PASS;
}

SEC("license") const char __license[] = "Dual BSD/GPL";