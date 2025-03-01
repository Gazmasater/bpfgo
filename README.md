import (
	"fmt"
	"strings"
)

func main() {
	eventComm := [64]int8{'h', 'e', 'l', 'l', 'o', 0, 0, 0} // пример данных

	// Преобразование
	commStr := strings.TrimRight(string(eventComm[:]), "\x00")

	fmt.Println(commStr) // "hello"
}
