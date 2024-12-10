struct bpf_map_def SEC("maps") perf_event_array = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .key_size = sizeof(u32),
    .value_size = sizeof(u32),
    .max_entries = 0,  // Устанавливается автоматически.
};

SEC("tracepoint/syscalls/sys_enter_open")
int tracepoint_handler(struct tracepoint__syscalls__sys_enter_open *ctx) {
    char msg[] = "sys_open called";
    bpf_perf_event_output(ctx, &perf_event_array, BPF_F_CURRENT_CPU, &msg, sizeof(msg));
    return 0;
}
