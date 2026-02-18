package main

import (
	"fmt"
	"net"
	"os"
	"os/signal"
	"runtime"
	"sync/atomic"
	"syscall"
	"time"
)

/*
syscall_flood (NO ENV/FLAGS)

Правь конфиг ниже и запускай:
  go run .

Цель: нагрузка именно по syscall'ам (sendto/sendmsg), 1 пакет = 1 syscall.
*/

const (
	dstAddr = "127.0.0.1:9999"

	payloadSize = 64 // bytes
	senders     = 8  // goroutines (parallel senders)
	writeBufMB  = 16 // SO_SNDBUF in MB

	// 0 = работать до Ctrl+C
	runSeconds = 10
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())

	raddr, err := net.ResolveUDPAddr("udp", dstAddr)
	if err != nil {
		panic(err)
	}

	conn, err := net.DialUDP("udp", nil, raddr)
	if err != nil {
		panic(err)
	}
	defer conn.Close()

	_ = conn.SetWriteBuffer(writeBufMB * 1024 * 1024)

	payload := make([]byte, payloadSize)

	var sent uint64
	var errs uint64

	stopSig := make(chan os.Signal, 1)
	signal.Notify(stopSig, os.Interrupt, syscall.SIGTERM)

	done := make(chan struct{})

	for i := 0; i < senders; i++ {
		go func() {
			for {
				select {
				case <-done:
					return
				default:
				}

				_, e := conn.Write(payload)
				if e != nil {
					atomic.AddUint64(&errs, 1)

					// если упёрлись в буферы/ядро — чуть притормозим, чтобы не захлебнуться ошибками
					time.Sleep(200 * time.Microsecond)
					continue
				}
				atomic.AddUint64(&sent, 1)
			}
		}()
	}

	tick := time.NewTicker(1 * time.Second)
	defer tick.Stop()

	var lastSent uint64
	var lastErr uint64
	start := time.Now()

	fmt.Printf("[syscall_flood] dst=%s size=%d senders=%d runSeconds=%d\n",
		dstAddr, payloadSize, senders, runSeconds)

	for {
		select {
		case <-tick.C:
			s := atomic.LoadUint64(&sent)
			e := atomic.LoadUint64(&errs)

			fmt.Printf("[syscall_flood] pps=%d err/s=%d total=%d errs=%d\n",
				s-lastSent, e-lastErr, s, e)

			lastSent, lastErr = s, e

			if runSeconds > 0 && time.Since(start) >= time.Duration(runSeconds)*time.Second {
				close(done)
				fmt.Printf("[done] total=%d errs=%d\n", s, e)
				return
			}

		case <-stopSig:
			close(done)
			s := atomic.LoadUint64(&sent)
			e := atomic.LoadUint64(&errs)
			fmt.Printf("[stop] total=%d errs=%d\n", s, e)
			return
		}
	}
}
