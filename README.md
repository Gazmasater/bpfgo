bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


Сообщение об ошибке указывает, что EUNOMIA_HOME не установлен, и программа не может найти путь к "eunomia data home". Попробуйте выполнить следующие шаги для решения проблемы:

Установите переменную окружения EUNOMIA_HOME:
Определите, где находится eunomia data, и укажите этот путь в переменной EUNOMIA_HOME. Например, в командной строке выполните:

bash
Копировать код
export EUNOMIA_HOME=/path/to/eunomia/data
Замените /path/to/eunomia/data на реальный путь.

Проверьте файл конфигурации Eunomia:
Возможно, путь к eunomia data задан в конфигурации или может быть уточнен в документации Eunomia.

Запустите с трассировкой ошибок: Если ошибка остается, включите трассировку для получения дополнительной информации:

bash
Копировать код
RUST_BACKTRACE=1 ./ecc ebpf_test.c

____________________________________________________________________________________________--

Используйте rustup: Наиболее распространенный способ установки Rust (включая Cargo) — это использование утилиты rustup. Для этого выполните команду в терминале:

bash
Копировать код
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
Эта команда загрузит и запустит скрипт установки rustup.

Следуйте инструкциям на экране: Во время установки вам будет предложено выбрать способ установки (обычно просто нажмите 1, чтобы продолжить с установкой по умолчанию).

Настройте окружение: После завершения установки добавьте cargo и rustc в ваш PATH, если это не было сделано автоматически. Вам может понадобиться выполнить следующую команду:

bash
Копировать код
source $HOME/.cargo/env
Или добавьте следующую строку в ваш файл ~/.bashrc или ~/.bash_profile (для других оболочек используйте соответствующие файлы конфигурации):

bash
Копировать код
export PATH="$HOME/.cargo/bin:$PATH"
Затем примените изменения:

bash
Копировать код
source ~/.bashrc
Проверьте установку: Убедитесь, что cargo и rustc установлены правильно, выполнив команды:

bash
Копировать код
cargo --version
rustc --version
Если вы видите версии cargo и rustc, установка прошла успешно, и вы можете использовать Cargo для управления вашими проектами на Rust!

____________________________________________________________________________________________________________

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$  RUST_BACKTRACE=1 ./ecc ebpf_test.c
thread 'main' panicked at src/config/mod.rs:100:87:
called `Result::unwrap()` on an `Err` value: eunomia data home not found, try setting `EUNOMIA_HOME`

Stack backtrace:
   0: ecc_rs::helper::get_eunomia_data_dir
   1: core::ops::function::FnOnce::call_once
   2: once_cell::imp::OnceCell<T>::initialize::{{closure}}
   3: once_cell::imp::initialize_or_wait
   4: once_cell::imp::OnceCell<T>::initialize
   5: <ecc_rs::config::CompileArgs as clap_builder::derive::Args>::augment_args
   6: clap_builder::derive::Parser::parse
   7: ecc_rs::main
   8: std::sys_common::backtrace::__rust_begin_short_backtrace
   9: std::rt::lang_start::{{closure}}
  10: std::rt::lang_start_internal
  11: main
  12: <unknown>
  13: __libc_start_main
  14: _start
stack backtrace:
   0: rust_begin_unwind
   1: core::panicking::panic_fmt
   2: core::result::unwrap_failed
   3: core::ops::function::FnOnce::call_once
   4: once_cell::imp::OnceCell<T>::initialize::{{closure}}
   5: once_cell::imp::initialize_or_wait
   6: once_cell::imp::OnceCell<T>::initialize
   7: <ecc_rs::config::CompileArgs as clap_builder::derive::Args>::augment_args
   8: clap_builder::derive::Parser::parse
   9: ecc_rs::main
note: Some details are omitted, run with `RUST_BACKTRACE=full` for a verbose backtrace.
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

_______________________________________________________________________________________________________________

Создайте директорию для Eunomia, если у вас её ещё нет. Например, вы можете создать её в вашей домашней папке:

bash
Копировать код
mkdir -p ~/eunomia_data
Установите переменную окружения EUNOMIA_HOME на путь к этой директории. Это можно сделать в командной строке:

bash
Копировать код
export EUNOMIA_HOME=~/eunomia_data
Запустите вашу программу снова:

bash
Копировать код
RUST_BACKTRACE=1 ./ecc ebpf_test.c
Если вы хотите, чтобы эта переменная окружения была установлена каждый раз при запуске терминала, вы можете добавить строку export EUNOMIA_HOME=~/eunomia_data в файл ~/.bashrc или ~/.bash_profile, а затем выполнить команду source ~/.bashrc, чтобы применить изменения.


