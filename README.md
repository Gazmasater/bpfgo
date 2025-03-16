
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

	progFD := objs.EchoDispatch.FD()

	// Получаем файловый дескриптор сетевого пространства имен (target_fd)
	// Для примера используем сетевое пространство по умолчанию
	targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()


	// Создаем ссылку для привязки программы с типом BPF_SK_LOOKUP
	err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  int(targetFD.Fd()),  // Файловый дескриптор целевого объекта (например, сетевого пространства имен)
		Program: myBpfProgram,        // Указатель на загруженную программу BPF
		Attach:  ebpf.AttachSkLookup, // Тип привязки, соответствующий BPF_SK_LOOKUP
		Flags:   0,                   // Необязательные флаги
		Anchor:  Anchor{},            // Если необходимо, можете указать якорь (можно оставить пустым)
	})
	
	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}


[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UnusedVar",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UnusedVar"
		}
	},
	"severity": 8,
	"message": "declared and not used: progFD",
	"source": "compiler",
	"startLineNumber": 39,
	"startColumn": 2,
	"endLineNumber": 39,
	"endColumn": 8,
	"tags": [
		1
	]
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "NoNewVar",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "NoNewVar"
		}
	},
	"severity": 8,
	"message": "no new variables on left side of :=",
	"source": "compiler",
	"startLineNumber": 51,
	"startColumn": 2,
	"endLineNumber": 57,
	"endColumn": 4
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredName"
		}
	},
	"severity": 8,
	"message": "undefined: myBpfProgram",
	"source": "compiler",
	"startLineNumber": 53,
	"startColumn": 12,
	"endLineNumber": 53,
	"endColumn": 24
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredName"
		}
	},
	"severity": 8,
	"message": "undefined: Anchor",
	"source": "compiler",
	"startLineNumber": 56,
	"startColumn": 12,
	"endLineNumber": 56,
	"endColumn": 18
}]







