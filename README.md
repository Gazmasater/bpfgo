// Замените map с int на uint32 для pidCache
pidCache := make(map[uint32]*bpfTraceInfo)

// Далее, в коде просто используйте uint32 для обращения к элементам карты
pidCache[event.Pid] = &event

// И удаляйте с тем же типом ключа
delete(pidCache, event.Pid)



