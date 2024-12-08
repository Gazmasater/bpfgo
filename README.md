struct bpf_map_def 
{
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
	__uint(max_entries, 1024);
} output SEC(".maps");
