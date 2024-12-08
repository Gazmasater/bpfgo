struct bpf_map_def SEC("maps") events = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .max_entries = 1024,
};


BPF_PERF_OUTPUT(events); // карта для передачи данных в юзерспейс

[{
	"resource": "/home/gaz358/myprog/bpfgo/tracepoint.c",
	"owner": "C/C++: IntelliSense",
	"code": "70",
	"severity": 8,
	"message": "incomplete type \"struct bpf_map_def\" is not allowed",
	"source": "C/C++",
	"startLineNumber": 166,
	"startColumn": 32,
	"endLineNumber": 166,
	"endColumn": 38
}]
