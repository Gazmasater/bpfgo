./chromedriver-linux64/chromedriver --browser-binary=./chrome-linux64/chrome



package main

import (
Gazmasater
bpfgo

Code
Issues
Pull requests 2
Actions
Projects
Wiki

    bpfgo

/
in
main
	"context"
	"fmt"
	"log"
	"time"

	"github.com/chromedp/chromedp"
)

func main() {
	ctx, cancel := chromedp.NewExecAllocator(context.Background(),
		chromedp.Flag("headless", false), // запускаем с GUI
		chromedp.Flag("disable-blink-features", "AutomationControlled"),
	)
	defer cancel()

	ctx, cancel = chromedp.NewContext(ctx)
	defer cancel()

	var html string
	err := chromedp.Run(ctx,
		chromedp.Navigate("https://ozon.ru"),
		chromedp.Sleep(3*time.Second),
		chromedp.OuterHTML("html", &html),
	)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("HTML длина:", len(html))
}


 www.tractorhouse.com.














https://github.com/Morwran/nft-go/tree/main/internal/expr-encoders



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


func resolveHost(ip net.IP) string {

	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		if ip.IsLoopback() {
			host = "localhost"
		} else {
			host = pkg.ResolveIP(ip)

		}
	} else {
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor


____________________________________________________________________________________________


package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"github.com/chromedp/chromedp"
)

func main() {
	ctx, cancel := chromedp.NewExecAllocator(context.Background(),
		chromedp.Flag("headless", false),
		chromedp.Flag("disable-blink-features", "AutomationControlled"),
	)
	defer cancel()

	ctx, cancel = chromedp.NewContext(ctx)
	defer cancel()

	var html string
	err := chromedp.Run(ctx,
		chromedp.Navigate("https://ozon.ru"),

		// Ждём появления и кликаем по кнопке "Принять"
		chromedp.WaitVisible(`button.uw_af6`, chromedp.ByQuery),
		chromedp.Click(`button.uw_af6`, chromedp.ByQuery),

		// Подождать, чтобы баннер скрылся
		chromedp.Sleep(2*time.Second),

		// Получить HTML всей страницы
		chromedp.OuterHTML("html", &html),
	)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("HTML длина:", len(html))
}

___________________________________________________________________________________________

✅ Структура проекта
go
Копировать
Редактировать
task-service/
├── cmd/
│   └── server/
│       └── main.go
├── internal/
│   ├── delivery/
│   │   └── http/
│   │       └── task_handler.go
│   ├── domain/
│   │   └── model.go
│   │   └── repository.go
│   ├── usecase/
│   │   └── task_usecase.go
│   └── repository/
│       └── memory/
│           └── task_repo.go
├── go.mod
├── README.md
📄 domain/model.go
go
Копировать
Редактировать
package domain

import "time"

type Status string

const (
	StatusPending   Status = "PENDING"
	StatusRunning   Status = "RUNNING"
	StatusCompleted Status = "COMPLETED"
	StatusFailed    Status = "FAILED"
)

type Task struct {
	ID         string    `json:"id"`
	CreatedAt  time.Time `json:"created_at"`
	StartedAt  time.Time `json:"started_at,omitempty"`
	EndedAt    time.Time `json:"ended_at,omitempty"`
	Status     Status    `json:"status"`
	Result     string    `json:"result,omitempty"`
}
📄 domain/repository.go
go
Копировать
Редактировать
package domain

type TaskRepository interface {
	Create(*Task) error
	Update(*Task) error
	Delete(id string) error
	Get(id string) (*Task, error)
}
📄 usecase/task_usecase.go
go
Копировать
Редактировать
package usecase

import (
	"sync"
	"time"

	"task-service/internal/domain"

	"github.com/google/uuid"
)

type TaskUseCase struct {
	repo domain.TaskRepository
}

func NewTaskUseCase(repo domain.TaskRepository) *TaskUseCase {
	return &TaskUseCase{repo: repo}
}

func (uc *TaskUseCase) CreateTask() (*domain.Task, error) {
	task := &domain.Task{
		ID:        uuid.NewString(),
		CreatedAt: time.Now(),
		Status:    domain.StatusPending,
	}
	if err := uc.repo.Create(task); err != nil {
		return nil, err
	}
	go uc.run(task)
	return task, nil
}

func (uc *TaskUseCase) run(task *domain.Task) {
	task.Status = domain.StatusRunning
	task.StartedAt = time.Now()
	time.Sleep(3 * time.Minute)
	task.Status = domain.StatusCompleted
	task.EndedAt = time.Now()
	task.Result = "OK"
	_ = uc.repo.Update(task)
}

func (uc *TaskUseCase) GetTask(id string) (*domain.Task, error) {
	return uc.repo.Get(id)
}

func (uc *TaskUseCase) DeleteTask(id string) error {
	return uc.repo.Delete(id)
}
📄 repository/memory/task_repo.go
go
Копировать
Редактировать
package memory

import (
	"errors"
	"sync"

	"task-service/internal/domain"
)

type InMemoryRepo struct {
	mu    sync.RWMutex
	tasks map[string]*domain.Task
}

func NewInMemoryRepo() *InMemoryRepo {
	return &InMemoryRepo{tasks: make(map[string]*domain.Task)}
}

func (r *InMemoryRepo) Create(t *domain.Task) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	r.tasks[t.ID] = t
	return nil
}

func (r *InMemoryRepo) Update(t *domain.Task) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	if _, ok := r.tasks[t.ID]; !ok {
		return errors.New("not found")
	}
	r.tasks[t.ID] = t
	return nil
}

func (r *InMemoryRepo) Delete(id string) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	delete(r.tasks, id)
	return nil
}

func (r *InMemoryRepo) Get(id string) (*domain.Task, error) {
	r.mu.RLock()
	defer r.mu.RUnlock()
	t, ok := r.tasks[id]
	if !ok {
		return nil, errors.New("not found")
	}
	return t, nil
}
📄 delivery/http/task_handler.go
go
Копировать
Редактировать
package http

import (
	"encoding/json"
	"net/http"
	"task-service/internal/usecase"

	"github.com/go-chi/chi/v5"
)

type Handler struct {
	uc *usecase.TaskUseCase
}

func NewHandler(uc *usecase.TaskUseCase) *Handler {
	return &Handler{uc: uc}
}

func (h *Handler) Routes() http.Handler {
	r := chi.NewRouter()
	r.Post("/", h.create)
	r.Get("/{id}", h.get)
	r.Delete("/{id}", h.delete)
	return r
}

func (h *Handler) create(w http.ResponseWriter, r *http.Request) {
	task, err := h.uc.CreateTask()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	writeJSON(w, task)
}

func (h *Handler) get(w http.ResponseWriter, r *http.Request) {
	id := chi.URLParam(r, "id")
	task, err := h.uc.GetTask(id)
	if err != nil {
		http.Error(w, "task not found", http.StatusNotFound)
		return
	}
	writeJSON(w, task)
}

func (h *Handler) delete(w http.ResponseWriter, r *http.Request) {
	id := chi.URLParam(r, "id")
	_ = h.uc.DeleteTask(id)
	w.WriteHeader(http.StatusNoContent)
}

func writeJSON(w http.ResponseWriter, v interface{}) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(v)
}
📄 cmd/server/main.go
go
Копировать
Редактировать
package main

import (
	"log"
	"net/http"
	"task-service/internal/delivery/http"
	"task-service/internal/repository/memory"
	"task-service/internal/usecase"
)

func main() {
	repo := memory.NewInMemoryRepo()
	uc := usecase.NewTaskUseCase(repo)
	handler := http.NewHandler(uc)

	log.Println("Listening on :8080")
	log.Fatal(http.ListenAndServe(":8080", handler.Routes()))
}


# 1. Создать задачу
curl -X POST http://localhost:8080/

# 2. Получить статус (замените ID на свой)
curl http://localhost:8080/88b5c9cf-2f4d-4a0d-871a-fc10c3b3ff82

# 3. Удалить задачу
curl -X DELETE http://localhost:8080/88b5c9cf-2f4d-4a0d-871a-fc10c3b3ff82

________________________________________________________________________________________________

ctx := context.Background() // Можно объявить в начале теста, если его ещё нет


package phttp

import (
	"encoding/json"
	"errors"
	"log"
	"net/http"
	"strconv"

	"github.com/gaz358/myprog/workmate/domain"
	"github.com/gaz358/myprog/workmate/pkg/logger"
	"github.com/gaz358/myprog/workmate/usecase"
	"github.com/go-chi/chi/v5"
)

type ErrorResponse struct {
	Message string `json:"message" example:"something went wrong"`
}

var _ = domain.Task{}

type Handler struct {
	uc  *usecase.TaskUseCase
	log logger.TypeOfLogger
}

func NewHandler(uc *usecase.TaskUseCase) *Handler {
	l := logger.Global().Named("http")
	return &Handler{
		uc:  uc,
		log: l,
	}
}

func (h *Handler) Routes() http.Handler {
	r := chi.NewRouter()
	r.Post("/", h.create)
	r.Get("/{id}", h.get)
	r.Get("/all", h.list)

	r.Delete("/{id}", h.delete)
	r.Put("/{id}/cancel", h.cancel)
	r.Get("/health", h.Health) // health на корне API
	r.Get("/filter", h.filter)

	return r
}

// @Summary      Создать новую задачу
// @Description  Инициализирует задачу со статусом Pending и возвращает её с сгенерированным ID
// @Tags         tasks
// @Produce      json
// @Success      200  {object}  domain.Task         "Задача успешно создана"
// @Failure      500  {object}  ErrorResponse  "Внутренняя ошибка сервера"
// @Router       /tasks [post]
func (h *Handler) create(w http.ResponseWriter, r *http.Request) {
	h.log.Infow("create task request", "method", r.Method, "path", r.URL.Path)

	task, err := h.uc.CreateTask()
	if err != nil {
		h.log.Errorw("failed to create task", "error", err)
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	h.log.Infow("task created", "id", task.ID)
	taskCopy := *task
	writeJSON(w, taskCopy)
}

// @Summary      Получить задачу по ID
// @Description  Возвращает задачу по её идентификатору
// @Tags         tasks
// @Produce      json
// @Param        id   path      string            true  "ID задачи"
// @Success      200  {object}  domain.Task        "Задача найдена"
// @Failure      404  {object}  phttp.ErrorResponse  "Задача не найдена"
// @Failure      500  {object}  phttp.ErrorResponse  "Внутренняя ошибка сервера"
// @Router       /tasks/{id} [get]
func (h *Handler) get(w http.ResponseWriter, r *http.Request) {
	id := chi.URLParam(r, "id")
	h.log.Infow("get task request", "method", r.Method, "path", r.URL.Path, "id", id)

	task, err := h.uc.GetTask(id)
	if err != nil {
		if errors.Is(err, domain.ErrNotFound) {
			h.log.Warnw("not found", "id", id)
			w.WriteHeader(http.StatusNotFound)
			writeJSON(w, ErrorResponse{Message: "not found"})
			return
		}

		h.log.Errorw("failed to get task", "id", id, "error", err)
		w.WriteHeader(http.StatusInternalServerError)
		writeJSON(w, ErrorResponse{Message: err.Error()})
		return
	}

	h.log.Infow("task retrieved", "id", task.ID)
	taskCopy := *task
	writeJSON(w, taskCopy)
}

// @Summary      Удалить задачу по ID
// @Description  Удаляет задачу из системы по её идентификатору
// @Tags         tasks
// @Param        id   path      string            true  "ID задачи"
// @Success      204  "No Content"
// @Failure      500  {object}  phttp.ErrorResponse  "Внутренняя ошибка сервера"
// @Router       /tasks/{id} [delete]
func (h *Handler) delete(w http.ResponseWriter, r *http.Request) {
	id := chi.URLParam(r, "id")
	h.log.Infow("delete task request", "method", r.Method, "path", r.URL.Path, "id", id)

	err := h.uc.DeleteTask(id)
	if err != nil {
		if errors.Is(err, domain.ErrNotFound) {
			h.log.Warnw("not found", "id", id)
			w.WriteHeader(http.StatusNotFound)
			writeJSON(w, ErrorResponse{Message: "not found"})
			return
		}

		h.log.Errorw("failed to delete task", "id", id, "error", err)
		w.WriteHeader(http.StatusInternalServerError)
		writeJSON(w, ErrorResponse{Message: err.Error()})
		return
	}

	h.log.Infow("task deleted", "id", id)
	w.WriteHeader(http.StatusNoContent)
}

func writeJSON(w http.ResponseWriter, v interface{}) {
	w.Header().Set("Content-Type", "application/json")
	_ = json.NewEncoder(w).Encode(v)
}

// @Summary      Получить список всех задач
// @Tags         tasks
// @Produce      json
// @Success      200  {array}  domain.TaskListItem
// @Failure      500  {object}  ErrorResponse
// @Router       /tasks/all [get]
func (h *Handler) list(w http.ResponseWriter, r *http.Request) {
	tasks, err := h.uc.ListTasks()
	if err != nil {
		h.log.Errorw("failed to list tasks", "error", err)
		writeJSON(w, ErrorResponse{Message: err.Error()})
		w.WriteHeader(http.StatusInternalServerError)
		return
	}

	var result []map[string]interface{}
	for _, t := range tasks {
		item := map[string]interface{}{
			"id":     t.ID,
			"status": t.Status,
		}
		if t.Status == domain.StatusCompleted {
			item["duration"] = t.EndedAt.Sub(t.StartedAt).String()
		}
		result = append(result, item)
	}

	writeJSON(w, result)
}

// @Summary      Отменить задачу
// @Description  Прерывает выполнение задачи, если она ещё не завершена
// @Tags         tasks
// @Param        id   path      string  true  "ID задачи"
// @Success      200  {object}  map[string]string  "Задача отменена"
// @Failure      404  {object}  ErrorResponse       "Задача не найдена"
// @Failure      500  {object}  ErrorResponse       "Внутренняя ошибка"
// @Router       /tasks/{id}/cancel [put]
func (h *Handler) cancel(w http.ResponseWriter, r *http.Request) {
	id := chi.URLParam(r, "id")
	h.log.Infow("cancel task request", "method", r.Method, "path", r.URL.Path, "id", id)

	err := h.uc.CancelTask(id)
	if err != nil {
		if errors.Is(err, domain.ErrNotFound) {
			h.log.Warnw("not found", "id", id)
			w.WriteHeader(http.StatusNotFound)
			writeJSON(w, ErrorResponse{Message: "not found"})
			return
		}
		h.log.Errorw("failed to cancel task", "id", id, "error", err)
		w.WriteHeader(http.StatusInternalServerError)
		writeJSON(w, ErrorResponse{Message: err.Error()})
		return
	}

	h.log.Infow("task canceled", "id", id)
	writeJSON(w, map[string]string{"status": "canceled"})
}

// @Summary      Healthcheck
// @Description  Проверка доступности сервиса
// @Tags         health
// @Produce      plain
// @Success      200 {string} string "ok"
// @Router       /health [get]
func (h *Handler) Health(w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(http.StatusOK)
	if _, err := w.Write([]byte("ok")); err != nil {
		log.Printf("error writing response: %v", err)
	}
}

// filter godoc
// @Summary      Фильтр и пагинация задач
// @Description  Фильтрует задачи по id, status, возвращает пагинацию
// @Tags         tasks
// @Produce      json
// @Param        id     query     string  false  "ID задачи (точное совпадение)"
// @Param        status query     string  false  "Статус задачи" Enums(pending, running, completed, failed, canceled)
// @Param        limit  query     int     false  "Максимум задач в ответе (default=10)"
// @Param        offset query     int     false  "Сдвиг (default=0)"
// @Success      200    {array}   domain.TaskListItem
// @Failure      500    {object}  ErrorResponse
// @Router       /tasks/filter [get]
func (h *Handler) filter(w http.ResponseWriter, r *http.Request) {
	id := r.URL.Query().Get("id")
	status := r.URL.Query().Get("status")

	// Дефолтные значения
	limit := 10
	offset := 0

	// Разбор параметров с обработкой ошибок
	if l := r.URL.Query().Get("limit"); l != "" {
		if v, err := strconv.Atoi(l); err == nil && v > 0 {
			limit = v
		}
	}
	if o := r.URL.Query().Get("offset"); o != "" {
		if v, err := strconv.Atoi(o); err == nil && v >= 0 {
			offset = v
		}
	}

	tasks, err := h.uc.ListTasks()
	if err != nil {
		h.log.Errorw("failed to list tasks", "error", err)
		writeJSON(w, ErrorResponse{Message: err.Error()})
		w.WriteHeader(http.StatusInternalServerError)
		return
	}

	// Фильтрация
	filtered := make([]*domain.Task, 0)
	for _, t := range tasks {
		if id != "" && t.ID != id {
			continue
		}
		if status != "" && string(t.Status) != status {
			continue
		}
		filtered = append(filtered, t)
	}

	// Пагинация
	if offset > len(filtered) {
		offset = len(filtered)
	}
	end := offset + limit
	if end > len(filtered) {
		end = len(filtered)
	}
	paged := filtered[offset:end]

	// Можно преобразовать к TaskListItem, если требуется
	writeJSON(w, paged)
}




