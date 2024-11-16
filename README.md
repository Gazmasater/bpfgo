ls /sys/kernel/debug/tracing/events/syscalls/

Проверка состояния tracepoint'а
Чтобы узнать, включен ли tracepoint, можно проверить содержимое соответствующего файла enable:

bash
Копировать код
cat /sys/kernel/debug/tracing/events/syscalls/sys_exit_accept/enable
Если вывод 0, tracepoint отключен. Если 1, он активен.

Как включить tracepoint?
Вы можете включить tracepoint, записав 1 в файл enable. Для этого нужны права суперпользователя:

bash
Копировать код
echo 1 | sudo tee /sys/kernel/debug/tracing/events/syscalls/sys_exit_accept/enable
Теперь tracepoint активен, и его можно использовать с инструментами, такими как bpftrace.

Как проверить работу tracepoint'ов?
После включения tracepoint'а проверьте его активность. Например, вы можете использовать:

bpftrace:

bash
Копировать код
sudo bpftrace -e 'tracepoint:syscalls:sys_exit_accept { printf("HELLO sys_exit_accept\n"); }'
perf:

bash
Копировать код
sudo perf trace -e syscalls:sys_exit_accept
trace-cmd:

bash
Копировать код
sudo trace-cmd record -e syscalls:sys_exit_accept
sudo trace-cmd report
Зачем это нужно?
Иногда tracepoint'ы по умолчанию отключены, чтобы снизить нагрузку на систему. Их нужно явно включать, чтобы использовать для отладки или мониторинга.

Проверьте права на /sys/kernel/debug/
Если вы не можете записывать в файл enable, убедитесь, что у вас есть доступ к /sys/kernel/debug/. Если права недоступны:

Убедитесь, что отладка включена:

bash
Копировать код
sudo mount -t debugfs none /sys/kernel/debug
Проверьте, что у вас есть root-доступ.

Итог
Убедитесь, что tracepoint включен, записав 1 в файл enable, если это не было сделано ранее. Это стандартный способ активировать tracepoint'ы для дальнейшей работы.






