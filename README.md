import (
	"fmt"
	"strings"
	"unsafe"
)

func int8ToString(arr [64]int8) string {
	// Приведение массива к []byte
	bytes := *(*[]byte)(unsafe.Pointer(&arr))
	// Обрезаем нулевые символы (конец строки)
	return strings.TrimRight(string(bytes), "\x00")
}

func main() {
	eventComm := [64]int8{'h', 'e', 'l', 'l', 'o', 0, 0, 0} // пример данных
	commStr := int8ToString(eventComm)
	fmt.Println(commStr) // "hello"
}
