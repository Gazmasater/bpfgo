struct bpf_map_def SEC("maps") events = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .max_entries = 1024,
};


BPF_PERF_OUTPUT(events); // карта для передачи данных в юзерспейс


