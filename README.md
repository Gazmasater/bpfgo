

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

struct bpf_map_def SEC("maps") echo_ports = {
	.type		= BPF_MAP_TYPE_HASH,
	.max_entries	= 1024,
	.key_size	= sizeof(__u32), // port number
	.value_size	= sizeof(__u64), // key for echo_socket
};



// Map of socket to which the listening ports forward traffic (in our case only one)
struct bpf_map_def SEC("maps") echo_socket = {
	.type		= BPF_MAP_TYPE_SOCKMAP,
	.max_entries	= 1,
	.key_size	= sizeof(__u32), // key for the socket
	.value_size	= sizeof(__u64), // socket file descriptor
};




