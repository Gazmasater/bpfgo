//go:build ignore

#include <linux/bpf.h>

#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>



SEC("sk_lookup/echo_dispatch")
int echo_dispatch(struct bpf_sk_lookup *ctx) {
	
bpf_print("lookup");
	

	return SK_PASS;
}

SEC("license") const char __license[] = "Dual BSD/GPL";