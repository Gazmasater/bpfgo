
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor

____________________________________________________________________________________________________
myprog

// file: int8string_bench_test.go
package main

import "testing"

// makeTestArray создаёт [32]int8], где первые байты содержат ASCII-символы,
// а оставшиеся — нули.
func makeTestArray() [32]int8 {
	var arr [32]int8
	s := "Hello, Benchmark!"
	for i := 0; i < len(s) && i < len(arr); i++ {
		arr[i] = int8(s[i])
	}
	return arr
}

func BenchmarkInt8ToString(b *testing.B) {
	arr := makeTestArray()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = Int8ToString(arr)
	}
}

_______________________________________________________________________________________--

newprogr

package main

import "unsafe"

// Int8ToStringOptimized — оптимизированная версия:
// один проход для поиска первого нулевого байта и zero-copy конверсия.
func Int8ToStringOptimized(arr [32]int8) string {
    // 1) Найти индекс первого нулевого байта (или 32, если его нет).
    var length int
    for length = 0; length < len(arr); length++ {
        if arr[length] == 0 {
            break
        }
    }
    // 2) Если строка пустая (первый байт нулевой), вернуть "".
    if length == 0 {
        return ""
    }
    // 3) Превратить [32]int8 ↦ [32]byte и сделать zero-copy через unsafe.String.
    slice := (*[32]byte)(unsafe.Pointer(&arr[0]))
    return unsafe.String(&slice[0], length)
}




// file: int8string_bench_test.go
package main

import "testing"

// makeTestArray создаёт [32]int8], где первые байты содержат
// ASCII-символы строки "Hello, Benchmark!", а остальные — нули.
func makeTestArray() [32]int8 {
	var arr [32]int8
	s := "Hello, Benchmark!"
	for i := 0; i < len(s) && i < len(arr); i++ {
		arr[i] = int8(s[i])
	}
	return arr
}

// BenchmarkOriginalInt8ToString измеряет вашу (оригинальную) версию Int8ToString.
// Предполагается, что функция называется именно Int8ToString.
func BenchmarkOriginalInt8ToString(b *testing.B) {
	arr := makeTestArray()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = Int8ToString(arr)
	}
}

// BenchmarkOptimizedInt8ToString измеряет оптимизированную версию.
// Переименуйте вашу оптимизированную функцию в Int8ToStringOptimized,
// чтобы оба бенчмарка вызывали разные реализации.
func BenchmarkOptimizedInt8ToString(b *testing.B) {
	arr := makeTestArray()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = Int8ToStringOptimized(arr)
	}
}



go test -bench=. -benchmem




