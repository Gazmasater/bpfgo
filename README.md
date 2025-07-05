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

package memory

import (
	"context"
	"errors"
	"sync"

	"github.com/gaz358/myprog/workmate/domain"
	"your_project/logger" // Укажи правильный импорт
)

type InMemoryRepo struct {
	mu    sync.RWMutex
	tasks map[string]*domain.Task
}

func NewInMemoryRepo() *InMemoryRepo {
	return &InMemoryRepo{tasks: make(map[string]*domain.Task)}
}

func (r *InMemoryRepo) Create(ctx context.Context, task *domain.Task) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	if _, exists := r.tasks[task.ID]; exists {
		logger.WarnKV(ctx, "[memory] Create: task already exists", "task_id", task.ID)
		return errors.New("task already exists")
	}
	tCopy := *task
	r.tasks[task.ID] = &tCopy
	logger.InfoKV(ctx, "[memory] Create: task created", "task_id", task.ID)
	return nil
}

func (r *InMemoryRepo) Update(ctx context.Context, task *domain.Task) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	if _, exists := r.tasks[task.ID]; !exists {
		logger.WarnKV(ctx, "[memory] Update: task not found", "task_id", task.ID)
		return domain.ErrNotFound
	}
	tCopy := *task
	r.tasks[task.ID] = &tCopy
	logger.InfoKV(ctx, "[memory] Update: task updated", "task_id", task.ID)
	return nil
}

func (r *InMemoryRepo) Delete(ctx context.Context, id string) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	if _, ok := r.tasks[id]; !ok {
		logger.WarnKV(ctx, "[memory] Delete: task not found", "task_id", id)
		return domain.ErrNotFound
	}
	delete(r.tasks, id)
	logger.InfoKV(ctx, "[memory] Delete: task deleted", "task_id", id)
	return nil
}

func (r *InMemoryRepo) Get(ctx context.Context, id string) (*domain.Task, error) {
	r.mu.RLock()
	defer r.mu.RUnlock()
	t, ok := r.tasks[id]
	if !ok {
		logger.WarnKV(ctx, "[memory] Get: task not found", "task_id", id)
		return nil, domain.ErrNotFound
	}
	tCopy := *t
	logger.InfoKV(ctx, "[memory] Get: task retrieved", "task_id", id)
	return &tCopy, nil
}

func (r *InMemoryRepo) List(ctx context.Context) ([]*domain.Task, error) {
	r.mu.RLock()
	defer r.mu.RUnlock()
	result := make([]*domain.Task, 0, len(r.tasks))
	for _, task := range r.tasks {
		tCopy := *task
		result = append(result, &tCopy)
	}
	logger.InfoKV(ctx, "[memory] List: tasks listed", "count", len(result))
	return result, nil
}







package usecase

import (
	"context"
	"sync"
	"time"

	"github.com/gaz358/myprog/workmate/domain"
	"github.com/google/uuid"
	"your_project/logger" // Укажи актуальный импорт логгера
)

type TaskUseCase struct {
	repo      domain.TaskRepository
	duration  time.Duration
	cancelMap map[string]context.CancelFunc
	mu        sync.Mutex
}

func NewTaskUseCase(repo domain.TaskRepository, duration time.Duration) *TaskUseCase {
	return &TaskUseCase{
		repo:      repo,
		duration:  duration,
		cancelMap: make(map[string]context.CancelFunc),
	}
}

func (uc *TaskUseCase) CreateTask(ctx context.Context) (*domain.Task, error) {
	task := &domain.Task{
		ID:        uuid.NewString(),
		CreatedAt: time.Now(),
		Status:    domain.StatusPending,
	}
	if err := uc.repo.Create(ctx, task); err != nil {
		logger.ErrorKV(ctx, "[usecase] CreateTask: failed to create", "err", err, "task_id", task.ID)
		return nil, err
	}
	logger.InfoKV(ctx, "[usecase] CreateTask: created", "task_id", task.ID)

	taskCtx, cancel := context.WithCancel(context.Background())
	uc.mu.Lock()
	uc.cancelMap[task.ID] = cancel
	uc.mu.Unlock()

	copy := *task
	go uc.run(taskCtx, &copy)

	return task, nil
}

func (uc *TaskUseCase) run(ctx context.Context, task *domain.Task) {
	task.Status = domain.StatusRunning
	task.StartedAt = time.Now()
	_ = uc.repo.Update(ctx, task)

	logger.InfoKV(ctx, "[usecase] run: started", "task_id", task.ID)
	select {
	case <-ctx.Done():
		task.Status = domain.StatusCancelled
		task.Result = "Canceled"
		task.EndedAt = time.Now()
		task.Duration = task.EndedAt.Sub(task.StartedAt).String()
		_ = uc.repo.Update(ctx, task)
		logger.InfoKV(ctx, "[usecase] run: canceled", "task_id", task.ID)
	case <-time.After(uc.duration):
		task.Status = domain.StatusCompleted
		task.EndedAt = time.Now()
		task.Duration = task.EndedAt.Sub(task.StartedAt).String()
		task.Result = "OK"
		_ = uc.repo.Update(ctx, task)
		logger.InfoKV(ctx, "[usecase] run: completed", "task_id", task.ID)
	}

	uc.mu.Lock()
	delete(uc.cancelMap, task.ID)
	uc.mu.Unlock()
}

func (uc *TaskUseCase) GetTask(ctx context.Context, id string) (*domain.Task, error) {
	task, err := uc.repo.Get(ctx, id)
	if err != nil {
		logger.WarnKV(ctx, "[usecase] GetTask: not found", "task_id", id, "err", err)
		return nil, err
	}
	logger.InfoKV(ctx, "[usecase] GetTask: found", "task_id", id)
	return task, nil
}

func (uc *TaskUseCase) DeleteTask(ctx context.Context, id string) error {
	uc.mu.Lock()
	if cancel, ok := uc.cancelMap[id]; ok {
		cancel()
		delete(uc.cancelMap, id)
		logger.InfoKV(ctx, "[usecase] DeleteTask: canceled before delete", "task_id", id)
	}
	uc.mu.Unlock()
	err := uc.repo.Delete(ctx, id)
	if err != nil {
		logger.WarnKV(ctx, "[usecase] DeleteTask: not found", "task_id", id, "err", err)
		return err
	}
	logger.InfoKV(ctx, "[usecase] DeleteTask: deleted", "task_id", id)
	return nil
}

func (uc *TaskUseCase) ListTasks(ctx context.Context) ([]*domain.Task, error) {
	tasks, err := uc.repo.List(ctx)
	if err != nil {
		logger.ErrorKV(ctx, "[usecase] ListTasks: failed", "err", err)
		return nil, err
	}
	logger.InfoKV(ctx, "[usecase] ListTasks: listed", "count", len(tasks))
	return tasks, nil
}

func (uc *TaskUseCase) CancelTask(ctx context.Context, id string) error {
	uc.mu.Lock()
	cancel, ok := uc.cancelMap[id]
	uc.mu.Unlock()
	if !ok {
		logger.WarnKV(ctx, "[usecase] CancelTask: not found", "task_id", id)
		return domain.ErrNotFound
	}
	cancel()
	logger.InfoKV(ctx, "[usecase] CancelTask: canceled", "task_id", id)
	return nil
}











