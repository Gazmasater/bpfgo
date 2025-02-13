struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");



struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); // number of CPUs
} trace_events SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); // количество CPU
} trace_events SEC(".maps.export");


    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, conn_info, sizeof(*conn_info));

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128);
    __uint(pinning, LIBBPF_PIN_BY_NAME);
} trace_events SEC(".maps");

bpftool map create /sys/fs/bpf/trace_events type perf_event_array key 4 value 4 entries 128
