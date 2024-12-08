1. Убедитесь, что пользователь имеет доступ к событиям производительности
Проверка возможности чтения событий производительности: Выполните следующую команду для проверки:

bash
Копировать код
perf stat ls
Если команда выполнена успешно, значит ваш пользователь может работать с событиями производительности. Если возникают ошибки, вам нужно предоставить дополнительные права или запустить команду от имени root.

Проверьте, что perf_event доступен: Убедитесь, что модуль ядра для работы с событиями производительности загружен:

bash
Копировать код
lsmod | grep perf
Если модуль не загружен, загрузите его:

bash
Копировать код
sudo modprobe perf
2. Проверка версии ядра
Определите текущую версию ядра:

bash
Копировать код
uname -r
Сравните с минимальной необходимой версией ядра: Для работы с BPF_MAP_TYPE_PERF_EVENT_ARRAY требуется ядро 4.4 или выше. Если у вас версия ядра ниже, обновите ядро до более новой версии.

3. Проверка поддержки eBPF и карты BPF_MAP_TYPE_PERF_EVENT_ARRAY
Проверьте конфигурацию ядра: Ваша система должна поддерживать BPF и нужные типы карт. Выполните следующую команду:

bash
Копировать код
zcat /proc/config.gz | grep CONFIG_BPF
Убедитесь, что следующие параметры включены:

CONFIG_BPF=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_EVENTS=y
CONFIG_PERF_EVENTS=y
Если файл /proc/config.gz недоступен: Вы можете попробовать загрузить конфигурацию ядра:

bash
Копировать код
sudo cat /boot/config-$(uname -r) | grep CONFIG_BPF
Проверьте поддержку карты BPF_MAP_TYPE_PERF_EVENT_ARRAY: Выполните следующую команду:

bash
Копировать код
bpftool feature probe
Найдите секцию, связанную с поддержкой карт (map_type), и убедитесь, что BPF_MAP_TYPE_PERF_EVENT_ARRAY поддерживается вашим ядром.

4. Установите необходимые библиотеки
Убедитесь, что на вашей системе установлены следующие пакеты:

bash
Копировать код
sudo apt-get install -y linux-tools-$(uname -r) linux-headers-$(uname -r) libbpf-dev
Если вы работаете с eBPF-программами на C, убедитесь, что clang и llvm также установлены:

bash
Копировать код
sudo apt-get install -y clang llvm
5. Проверка вывода dmesg для отладки
Если ошибка все еще возникает, выполните:

bash
Копировать код
dmesg | grep bpf
Лог ядра может дать дополнительную информацию о проблеме, например, о несовместимости или неправильной конфигурации карты.

6. Проверка программы
Убедитесь, что вы используете правильный код для определения карты:

c
Копировать код
struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 1024);
} output SEC(".maps");
Если вы используете более старые версии, где __uint и SEC(".maps") недоступны, убедитесь, что структура карты объявлена правильно. Для старого подхода может использоваться:

c
Копировать код
struct bpf_map_def SEC("maps") output = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .max_entries = 1024,
};
Если проблема сохраняется
Предоставьте вывод следующих команд:

Версия ядра:
bash
Копировать код
uname -r
Поддержка карт:
bash
Копировать код
bpftool feature probe
Логи ядра:
bash
Копировать код
dmesg | grep bpf
Это поможет определить источник проблемы.



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ perf stat ls
Error:
Access to performance monitoring and observability operations is limited.
Consider adjusting /proc/sys/kernel/perf_event_paranoid setting to open
access to performance monitoring and observability operations for processes
without CAP_PERFMON, CAP_SYS_PTRACE or CAP_SYS_ADMIN Linux capability.
More information can be found at 'Perf events and tool security' document:
https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html
perf_event_paranoid setting is 4:
  -1: Allow use of (almost) all events by all users
      Ignore mlock limit after perf_event_mlock_kb without CAP_IPC_LOCK
>= 0: Disallow raw and ftrace function tracepoint access
>= 1: Disallow CPU event access
>= 2: Disallow kernel profiling
To make the adjusted perf_event_paranoid setting permanent preserve it
in /etc/sysctl.conf (e.g. kernel.perf_event_paranoid = <setting>)
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

Ошибка, которую вы видите, связана с ограничениями на использование инструментов мониторинга производительности, таких как perf, для обычных пользователей. Параметр perf_event_paranoid контролирует доступ к этим операциям, и на вашей системе он установлен на значение 4, что ограничивает доступ.

Для исправления этой проблемы нужно изменить значение параметра perf_event_paranoid, чтобы разрешить использование инструментов производительности для обычных пользователей.

Шаги для решения проблемы:
Измените значение perf_event_paranoid:

Для того чтобы временно разрешить доступ, выполните следующую команду:

bash
Копировать код
sudo sysctl -w kernel.perf_event_paranoid=1
Это разрешит доступ к CPU событиям, но ограничит доступ к более низкоуровневым операциям, таким как фреймворк трассировки функций.

Для перманентных изменений:

Чтобы сохранить это изменение после перезагрузки системы, добавьте настройку в файл /etc/sysctl.conf:

Откройте файл с помощью текстового редактора:

bash
Копировать код
sudo nano /etc/sysctl.conf
Добавьте в конец файла строку:

bash
Копировать код
kernel.perf_event_paranoid = 1
После этого примените изменения:

bash
Копировать код
sudo sysctl -p
Проверьте, изменился ли параметр:

Для проверки текущего значения параметра выполните команду:

bash
Копировать код
cat /proc/sys/kernel/perf_event_paranoid
Вы должны увидеть значение 1 (или меньше, если вы разрешили более высокий уровень доступа).

Дополнительное изменение прав (если требуется):

Если вы все еще сталкиваетесь с ошибками доступа, возможно, потребуется предоставить дополнительные права. В некоторых случаях, чтобы использовать инструменты производительности, может потребоваться запуск от имени пользователя с правами CAP_SYS_ADMIN (например, с использованием sudo).

Важно, чтобы ваша система поддерживала нужные привилегии или разрешения для работы с событиями производительности.

Дальнейшие шаги:
После того как вы измените параметр perf_event_paranoid, попробуйте снова выполнить команду:

bash
Копировать код
perf stat ls
Если все прошло успешно, вы должны увидеть список доступных событий производительности.

Если проблема продолжит возникать, проверьте логи системы с помощью dmesg или дополнительных настроек безопасности, например, SELinux или AppArmor, которые могут ограничивать доступ к этим операциям.







