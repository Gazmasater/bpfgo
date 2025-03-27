//go:build ignore

#include <linux/bpf.h>

#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>



SEC("sockops")
int bpf_sockops_cb(struct bpf_sock_ops *skops) {
	u32 op;
	op = skops->op;

	switch (op) {
	case BPF_SOCK_OPS_ACTIVE_ESTABLISHED_CB:
		bpf_sock_ops_establish_cb(skops, SOCK_TYPE_ACTIVE);
		break;
	case BPF_SOCK_OPS_PASSIVE_ESTABLISHED_CB:
		bpf_sock_ops_establish_cb(skops, SOCK_TYPE_PASSIVE);
		break;
	case BPF_SOCK_OPS_RTT_CB:
		bpf_sock_ops_rtt_cb(skops);
		break;
	case BPF_SOCK_OPS_STATE_CB:
		bpf_sock_ops_state_cb(skops);
		break;
	}

	return 0;
}


SEC("license") const char __license[] = "Dual BSD/GPL";