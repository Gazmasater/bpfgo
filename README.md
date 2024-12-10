Структура events в данном контексте — это карта (map) типа BPF_MAP_TYPE_RINGBUF. Она используется для хранения данных, которые будут переданы из ядра в пользовательское пространство.

Полный код для структуры карты выглядит так:

Определение карты events
c
Копировать код
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);    // Тип карты: Ring Buffer
    __uint(max_entries, 8 * 1024);         // Размер буфера: 8 КБ
} events SEC(".maps");                      // Помещаем карту в секцию .maps
Структура для передачи данных
Кроме самой карты, вам потребуется структура данных, которую вы будете отправлять через эту карту. Пример:

c
Копировать код
struct event_t {
    u32 pid;          // ID процесса
    char comm[16];    // Имя команды (процесса)
    u32 src_ip;       // IP-адрес источника
    u16 sport;        // Исходный порт
};
Эта структура описывает данные, которые будут записаны в events и затем считаны в пользовательском пространстве.

Итоговый пример кода
Объявление карты и структуры
c
Копировать код
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 8 * 1024);
} events SEC(".maps");

struct event_t {
    u32 pid;
    char comm[16];
    u32 src_ip;
    u16 sport;
};
Логика записи в Ring Buffer
c
Копировать код
SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (conn_info->comm[0] != 'u' || conn_info->comm[1] != 'd') {
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        struct event_t *event = bpf_ringbuf_reserve(&events, sizeof(struct event_t), 0);
        if (!event) {
            return 0; // Не удалось выделить место в буфере
        }

        event->pid = pid;
        __builtin_memcpy(event->comm, conn_info->comm, sizeof(conn_info->comm));
        event->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        event->sport = bpf_ntohs(addr.sin_port);

        bpf_ringbuf_submit(event, 0);
    }

    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);

    return 0;
}
Теперь структура events и данные, которые она обрабатывает, определены и готовы к использованию.


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0x2295 symtab 0x22d0 relocs 0x2318 sh_offset 0x2318
thread '<unnamed>' panicked at ecli-lib/src/runner/task_manager/mod.rs:252:22:
called `Result::unwrap()` on an `Err` value: Export map named `events` found, but no export type is provided
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace



Ошибка сообщает, что экспортируемая карта events была найдена, но для нее не задан тип экспорта. Это происходит, когда используемая утилита ожидает явного указания типа экспорта для карты, но его нет в вашем коде или конфигурации.

Причины ошибки
Отсутствие типа экспорта карты events. Убедитесь, что карта events правильно экспортируется.
Неправильное определение карты. Например, если секция .maps используется неправильно или есть несовместимость с утилитой ecli.
Некорректный package.json или конфигурация. Возможно, карта events ожидает определенного типа в конфигурации ecli.
Решение
1. Проверьте определение карты events
Убедитесь, что карта определена корректно:

c
Копировать код
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 8 * 1024);
} events SEC(".maps");
2. Обновите экспорт карты
Если утилита ecli требует явного указания экспорта, добавьте атрибуты:

c
Копировать код
SEC("maps/events")
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 8 * 1024);
} events;
3. Проверьте package.json
Убедитесь, что в файле package.json указана правильная конфигурация экспорта. Для карты events необходимо задать тип экспорта:

json
Копировать код
{
  "maps": {
    "events": {
      "type": "ringbuf",
      "max_entries": 8192
    }
  }
}
4. Запустите с переменной RUST_BACKTRACE
Для получения более подробной информации об ошибке запустите команду с включенным backtrace:

bash
Копировать код
RUST_BACKTRACE=1 sudo ./ecli run package.json
5. Обновите или исправьте ecli
Если проблема сохраняется, возможно, используемая утилита ecli требует конкретных изменений или обновлений. Проверьте документацию ecli и убедитесь, что версия поддерживает текущую конфигурацию.

Если проблема остается
Сообщите дополнительную информацию:

Версию ecli.
Полный package.json.
Фрагменты вашего BPF-кода.









