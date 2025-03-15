

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("sk_lookup")
int echo_dispatch(struct bpf_sk_lookup *ctx)
{
    __u8 proto ; 


bpf_probe_read_user(&proto,sizeof(proto),&ctx->protocol);
     __u32 srcIP=bpf_ntohl(ctx->local_ip4);
     __u16 srcPort=bpf_ntohs(ctx->local_port);


    const char *proto_str;
    if (proto == IPPROTO_TCP) {
        proto_str = "TCP";
    } else if (proto == IPPROTO_UDP) {
        proto_str = "UDP";
    } else {
        proto_str = "UNKNOWN";
    }

 bpf_printk("sk_lookup srcIP=%d.%d.%d.%d    srcPORT=%d: Protocol: %d\n", 
    (srcIP>>24)&0xff,
    (srcIP>>16)&0xff,
    (srcIP>>8)&0xff,
    (srcIP)&0xff,
     srcPort,
     proto);
    return SK_PASS;
}

    bpf_printk("DEBUG: ctx->protocol=%d, ctx->local_ip4=%u, ctx->local_port=%u\n", 
        ctx->protocol, ctx->local_ip4, ctx->local_port);


🔧 Правильный способ загрузки и прикрепления sk_lookup
1️⃣ Проверьте, загружена ли программа

sudo bpftool prog show
Если программа загружена, запомните её ID.

2️⃣ Создайте cgroup
sk_lookup работает через cgroup, поэтому нужно создать каталог и смонтировать его, если он ещё не существует:


sudo mkdir -p /sys/fs/cgroup/sk_lookup
sudo mount -t cgroup2 none /sys/fs/cgroup/sk_lookup
Затем привяжите текущий процесс к cgroup:


echo $$ | sudo tee /sys/fs/cgroup/sk_lookup/cgroup.procs
3️⃣ Прикрепите sk_lookup к cgroup
Используйте bpftool cgroup attach, а не net attach:


sudo bpftool cgroup attach /sys/fs/cgroup/sk_lookup sk_lookup id <ID_ПРОГРАММЫ>
Где <ID_ПРОГРАММЫ> — это ID вашей загруженной программы из bpftool prog show.

4️⃣ Проверьте, привязана ли программа

sudo bpftool cgroup show /sys/fs/cgroup/sk_lookup
Если программа не отображается, значит, она не была успешно привязана.



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool prog load sk_up.o /sys/fs/bpf/test_prog type sk_lookup
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool prog show | grep sk_lookup | awk '{print $1}'
80:
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ echo $$ | sudo tee /sys/fs/cgroup/sk_lookup/cgroup.procs
3190
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ echo $$ | sudo tee /sys/fs/cgroup/sk_lookup/cgroup.procs
3190
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo tc qdisc add dev lo clsact
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo tc filter add dev lo ingress bpf obj ./sk_up.o sec sk_lookup
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo tc filter add dev lo egress bpf obj ./sk_up.o sec sk_lookup
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool cgroup attach /sys/fs/cgroup/sk_lookup sk_lookup id 80            
Error: failed to attach program
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool cgroup attach /sys/fs/cgroup/sk_lookup sk_lookup id 3190
Error: get by id (3190): No such file or directory
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool cgroup attach /sys/fs/cgroup/sk_lookup sk_lookup id 80
Error: failed to attach program
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ 



























