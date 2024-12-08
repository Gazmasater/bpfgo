struct bpf_map_def SEC("maps") output = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .max_entries = 1024,
};
