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


Как разместить твои функции:
main.go (минимальный):

go
Копировать код
package main

import "github.com/gaz358/myprog/workmate/internal/app"

func main() {
    app.Run()
}
internal/app/app.go:

go
Копировать код
package app

import (
    // ...
)

func Run() {
    cfg := config.Load()
    logg := initLogger(cfg)
    repo := memory.NewInMemoryRepo()
    uc := usecase.NewTaskUseCase(repo, cfg.TaskDuration)
    handler := phttp.NewHandler(uc)
    router := setupRouter(handler)
    server := newServer(cfg, router)
    runServer(server, logg, cfg.ShutdownTimeout)
}

// остальные функции: initLogger, setupRouter, newServer, runServer, parseLogLevel...
internal/app/router.go (если хочется вынести отдельно):

go
Копировать код
package app

import (
    // ...
)

func setupRouter(handler *phttp.Handler) http.Handler {
    r := chi.NewRouter()
    r.Mount("/tasks", handler.Routes())
    r.Get("/swagger/*", httpSwagger.WrapHandler)
    r.Get("/health", health.Handler)
    return r
}
Кратко:
Всё что не бизнес-логика и не обработчики HTTP — в /internal/app (или internal/server).

main.go минимальный, только вызывает Run/app.Init.

Все вспомогательные функции (инициализация роутера, сервера, логгера) — либо в app.go, либо разбить по app/router.go, app/logger.go (если функций много).

Итог:
main.go:
Только старт, максимум — вызов app.Run()

internal/app/app.go (или server.go):
Все glue-функции: сборка приложения, запуск, graceful shutdown, init логгера, init router, init сервера.

internal/app/router.go (опционально):
Только функции для сборки chi/mux/echo/..., если их много.

Если хочешь — могу расписать пример файлов полностью под твой проект, просто напиши!
Или дать makefile-шаблон запуска/деплоя для такого layout.



package app

import (
	"context"
	"net/http"
	"time"

	"github.com/gaz358/myprog/workmate/config"
	"github.com/gaz358/myprog/workmate/internal/delivery/health"
	"github.com/gaz358/myprog/workmate/internal/delivery/phttp"
	"github.com/gaz358/myprog/workmate/pkg/logger"
	"github.com/gaz358/myprog/workmate/repository/memory"
	"github.com/gaz358/myprog/workmate/usecase"

	httpSwagger "github.com/swaggo/http-swagger"
	"github.com/go-chi/chi/v5"
	"os"
	"os/signal"
)

// Точка входа для main.go
func Run() {
	cfg := config.Load()
	logg := initLogger(cfg)
	repo := memory.NewInMemoryRepo()
	uc := usecase.NewTaskUseCase(repo, cfg.TaskDuration)
	handler := phttp.NewHandler(uc)
	router := setupRouter(handler)
	server := newServer(cfg, router)
	runServer(server, logg, cfg.ShutdownTimeout)
}

func initLogger(cfg *config.Config) logger.TypeOfLogger {
	logger.SetLevel(parseLogLevel(cfg.LogLevel))
	return logger.Global().Named("main")
}

func parseLogLevel(level string) logger.LogLevel {
	switch level {
	case "debug":
		return logger.DebugLevel
	case "info":
		return logger.InfoLevel
	case "warn":
		return logger.WarnLevel
	case "error":
		return logger.ErrorLevel
	default:
		return logger.InfoLevel
	}
}

func setupRouter(handler *phttp.Handler) http.Handler {
	r := chi.NewRouter()
	r.Mount("/tasks", handler.Routes())
	r.Get("/swagger/*", httpSwagger.WrapHandler)
	r.Get("/health", health.Handler)
	return r
}

func newServer(cfg *config.Config, handler http.Handler) *http.Server {
	return &http.Server{
		Addr:    ":" + cfg.Port,
		Handler: handler,
	}
}

func runServer(srv *http.Server, logg logger.TypeOfLogger, shutdownTimeout time.Duration) {
	quit := make(chan os.Signal, 1)
	signal.Notify(quit, os.Interrupt)

	go func() {
		logg.Infow("starting server", "addr", srv.Addr)
		if err := srv.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			logg.Fatalw("server error", "err", err)
		}
	}()

	<-quit
	logg.Infow("shutting down server")

	ctx, cancel := context.WithTimeout(context.Background(), shutdownTimeout)
	defer cancel()
	if err := srv.Shutdown(ctx); err != nil {
		logg.Fatalw("shutdown error", "err", err)
	}
	logg.Infow("server stopped")
}










