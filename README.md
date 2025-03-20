lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

3️⃣ Если namespace не виден в ip netns list
Используйте ls -l /proc/*/ns/net и найдите, какой PID связан с новым namespace:


ls -l /proc/*/ns/net
Затем выберите нужный PID и выполните:


nsenter --net=/proc/PID/ns/net bpftool net list
