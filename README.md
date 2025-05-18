__u32 src = 0;
BPF_CORE_READ(src, ctx, local_ip4);
info.srcIP.s_addr = src;

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "3364",
	"severity": 8,
	"message": "operator -> or ->* applied to \"__u32\" (aka \"unsigned int\") instead of to a pointer type",
	"source": "C/C++",
	"startLineNumber": 536,
	"startColumn": 5,
	"endLineNumber": 536,
	"endColumn": 18
}]



