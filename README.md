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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ RUST_BACKTRACE=1 ./ecc ebpf_test.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmp8wBEIv/include", "-I/tmp/.tmp8wBEIv/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "ebpf_test.c", "-o", "ebpf_test.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] ebpf_test.c:40:84: error: use of undeclared identifier 'BPF_ANY'
   40 |     bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);
      |                                                                                    ^
1 error generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))

Stack backtrace:
   0: ecc_rs::bpf_compiler::do_compile
   1: ecc_rs::bpf_compiler::compile_bpf
   2: ecc_rs::main
   3: std::sys_common::backtrace::__rust_begin_short_backtrace
   4: std::rt::lang_start::{{closure}}
   5: std::rt::lang_start_internal
   6: main
   7: <unknown>
   8: __libc_start_main
   9: _start
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ RUST_BACKTRACE=1 ./ecc ebpf_test.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpOAYCW3/include", "-I/tmp/.tmpOAYCW3/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "ebpf_test.c", "-o", "ebpf_test.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
In file included from /usr/include/linux/bpf.h:11:
In file included from /usr/include/linux/types.h:9:
/usr/include/linux/posix_types.h:27:3: error: typedef redefinition with different types ('struct __kernel_fd_set' vs 'struct __kernel_fd_set')
   27 | } __kernel_fd_set;
      |   ^
./vmlinux.h:29688:3: note: previous definition is here
 29688 | } __kernel_fd_set;
       |   ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
In file included from /usr/include/linux/bpf.h:11:
In file included from /usr/include/linux/types.h:9:
In file included from /usr/include/linux/posix_types.h:36:
In file included from /usr/include/asm/posix_types.h:7:
In file included from /usr/include/asm/posix_types_64.h:18:
/usr/include/asm-generic/posix_types.h:68:22: error: typedef redefinition with different types ('unsigned int' vs '__kernel_ulong_t' (aka 'unsigned long'))
   68 | typedef unsigned int    __kernel_size_t;
      |                         ^
./vmlinux.h:28703:26: note: previous definition is here
 28703 | typedef __kernel_ulong_t __kernel_size_t;
       |                          ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
In file included from /usr/include/linux/bpf.h:11:
In file included from /usr/include/linux/types.h:9:
In file included from /usr/include/linux/posix_types.h:36:
In file included from /usr/include/asm/posix_types.h:7:
In file included from /usr/include/asm/posix_types_64.h:18:
/usr/include/asm-generic/posix_types.h:69:14: error: typedef redefinition with different types ('int' vs '__kernel_long_t' (aka 'long'))
   69 | typedef int             __kernel_ssize_t;
      |                         ^
./vmlinux.h:28575:25: note: previous definition is here
 28575 | typedef __kernel_long_t __kernel_ssize_t;
       |                         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
In file included from /usr/include/linux/bpf.h:11:
In file included from /usr/include/linux/types.h:9:
In file included from /usr/include/linux/posix_types.h:36:
In file included from /usr/include/asm/posix_types.h:7:
In file included from /usr/include/asm/posix_types_64.h:18:
/usr/include/asm-generic/posix_types.h:70:14: error: typedef redefinition with different types ('int' vs '__kernel_long_t' (aka 'long'))
   70 | typedef int             __kernel_ptrdiff_t;
      |                         ^
./vmlinux.h:28573:25: note: previous definition is here
 28573 | typedef __kernel_long_t __kernel_ptrdiff_t;
       |                         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
In file included from /usr/include/linux/bpf.h:11:
In file included from /usr/include/linux/types.h:9:
In file included from /usr/include/linux/posix_types.h:36:
In file included from /usr/include/asm/posix_types.h:7:
In file included from /usr/include/asm/posix_types_64.h:18:
/usr/include/asm-generic/posix_types.h:81:3: error: typedef redefinition with different types ('struct __kernel_fsid_t' vs 'struct __kernel_fsid_t')
   81 | } __kernel_fsid_t;
      |   ^
./vmlinux.h:29692:3: note: previous definition is here
 29692 | } __kernel_fsid_t;
       |   ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:60:2: error: redefinition of enumerator 'BPF_REG_0'
   60 |         BPF_REG_0 = 0,
      |         ^
./vmlinux.h:996:2: note: previous definition is here
  996 |         BPF_REG_0 = 0,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:61:2: error: redefinition of enumerator 'BPF_REG_1'
   61 |         BPF_REG_1,
      |         ^
./vmlinux.h:997:2: note: previous definition is here
  997 |         BPF_REG_1 = 1,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:62:2: error: redefinition of enumerator 'BPF_REG_2'
   62 |         BPF_REG_2,
      |         ^
./vmlinux.h:998:2: note: previous definition is here
  998 |         BPF_REG_2 = 2,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:63:2: error: redefinition of enumerator 'BPF_REG_3'
   63 |         BPF_REG_3,
      |         ^
./vmlinux.h:999:2: note: previous definition is here
  999 |         BPF_REG_3 = 3,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:64:2: error: redefinition of enumerator 'BPF_REG_4'
   64 |         BPF_REG_4,
      |         ^
./vmlinux.h:1000:2: note: previous definition is here
 1000 |         BPF_REG_4 = 4,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:65:2: error: redefinition of enumerator 'BPF_REG_5'
   65 |         BPF_REG_5,
      |         ^
./vmlinux.h:1001:2: note: previous definition is here
 1001 |         BPF_REG_5 = 5,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:66:2: error: redefinition of enumerator 'BPF_REG_6'
   66 |         BPF_REG_6,
      |         ^
./vmlinux.h:1002:2: note: previous definition is here
 1002 |         BPF_REG_6 = 6,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:67:2: error: redefinition of enumerator 'BPF_REG_7'
   67 |         BPF_REG_7,
      |         ^
./vmlinux.h:1003:2: note: previous definition is here
 1003 |         BPF_REG_7 = 7,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:68:2: error: redefinition of enumerator 'BPF_REG_8'
   68 |         BPF_REG_8,
      |         ^
./vmlinux.h:1004:2: note: previous definition is here
 1004 |         BPF_REG_8 = 8,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:69:2: error: redefinition of enumerator 'BPF_REG_9'
   69 |         BPF_REG_9,
      |         ^
./vmlinux.h:1005:2: note: previous definition is here
 1005 |         BPF_REG_9 = 9,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:70:2: error: redefinition of enumerator 'BPF_REG_10'
   70 |         BPF_REG_10,
      |         ^
./vmlinux.h:1006:2: note: previous definition is here
 1006 |         BPF_REG_10 = 10,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:71:2: error: redefinition of enumerator '__MAX_BPF_REG'
   71 |         __MAX_BPF_REG,
      |         ^
./vmlinux.h:1007:2: note: previous definition is here
 1007 |         __MAX_BPF_REG = 11,
      |         ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:77:8: error: redefinition of 'bpf_insn'
   77 | struct bpf_insn {
      |        ^
./vmlinux.h:45043:8: note: previous definition is here
 45043 | struct bpf_insn {
       |        ^
In file included from ebpf_test.c:3:
In file included from /tmp/.tmpOAYCW3/include/bpf/bpf.h:26:
/usr/include/linux/bpf.h:89:8: error: redefinition of 'bpf_lpm_trie_key'
   89 | struct bpf_lpm_trie_key {
      |        ^
./vmlinux.h:45942:8: note: previous definition is here
 45942 | struct bpf_lpm_trie_key {
       |        ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))

Stack backtrace:
   0: ecc_rs::bpf_compiler::do_compile
   1: ecc_rs::bpf_compiler::compile_bpf
   2: ecc_rs::main
   3: std::sys_common::backtrace::__rust_begin_short_backtrace
   4: std::rt::lang_start::{{closure}}
   5: std::rt::lang_start_internal
   6: main
   7: <unknown>
   8: __libc_start_main
   9: _start

   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

   [160] STRUCT 'task_struct' size=9728 vlen=253

   [{
	"resource": "/home/gaz358/myprog/bpfgo/ebpf_test.c",
	"owner": "C/C++: IntelliSense",
	"code": "833",
	"severity": 8,
	"message": "pointer or reference to incomplete type \"struct task_struct\" is not allowed",
	"source": "C/C++",
	"startLineNumber": 78,
	"startColumn": 24,
	"endLineNumber": 78,
	"endColumn": 37
}]

#########################################################################################

    const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);




