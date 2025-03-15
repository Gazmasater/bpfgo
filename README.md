

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


1. Проверка cgroupPath
Перед выполнением кода убедись, что путь /sys/fs/cgroup/net_cls/my_cgroup существует и поддерживает net_cls. Выполни в терминале:


ls -la /sys/fs/cgroup/net_cls/my_cgroup
Если директории нет, попробуй создать её вручную:


sudo mkdir -p /sys/fs/cgroup/net_cls/my_cgroup
Затем проверь, что она поддерживает net_cls:


sudo mount -t cgroup -o net_cls none /sys/fs/cgroup/net_cls
2. Проверь, что net_cls.classid устанавливается корректно
Перед привязкой BPF-программы попробуй вручную установить classid:


echo "0x100001" | sudo tee /sys/fs/cgroup/net_cls/my_cgroup/net_cls.classid
Если это выдаёт ошибку "Permission denied", попробуй выполнить:

sudo chmod 777 /sys/fs/cgroup/net_cls/my_cgroup/net_cls.classid
А затем снова установи classid.

3. Проверь tc (Traffic Control)
Убедись, что clsact был успешно добавлен:


sudo tc qdisc show dev lo
Ожидаемый вывод должен включать строку qdisc clsact:


qdisc clsact ffff: dev lo parent ffff:fff1
Если clsact не был добавлен, попробуй удалить и повторно создать его:


sudo tc qdisc del dev lo clsact
sudo tc qdisc add dev lo clsact
4. Дескриптор cgroup (fd)
При получении дескриптора файла убедись, что он не равен -1. Добавь отладочный вывод:


fmt.Printf("cgroupFd: %d\n", cgroupFd)
if cgroupFd <= 0 {
    log.Fatalf("Invalid cgroup file descriptor: %d", cgroupFd)
}
Если cgroupFd <= 0, значит, os.Open() не открыл файл. Проверь, что cgroupPath существует.

5. Проверь программу BPF
Если все вышеописанные шаги не помогли, возможно, ошибка в objs.EchoDispatch. Проверь:


if objs.EchoDispatch == nil {
    log.Fatalf("BPF program EchoDispatch is nil")
}
Также попробуй загрузить программу вручную:


Если программа отсутствует, попробуй загрузить её через bpftool:


bpftool prog load bpf_program.o /sys/fs/bpf/echo_dispatch
6. Логирование ошибки
Измени обработку ошибки так, чтобы получить больше деталей:


err = link.RawAttachProgram(link.RawAttachProgramOptions{
    Program: objs.EchoDispatch,   
    Attach:  ebpf.AttachSkLookup, 
    Target:  cgroupFd,            
})
if err != nil {
    log.Fatalf("failed to attach sk_lookup: %v\nDetailed error: %+v", err, err)
}
Вывод
Если проблема сохраняется, определи, на каком шаге она возникает:

Создаётся ли cgroup?
Корректно ли устанавливается net_cls.classid?
Добавляется ли clsact?
Получается ли дескриптор cgroupFd?
Не nil ли objs.EchoDispatch?



































