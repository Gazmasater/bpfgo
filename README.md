Попробуй выполнить следующие шаги:

Обнови список веток с удалённого репозитория:


git fetch --all
Переключись на удалённую ветку trace-monitor: Если ветка есть на удалённом репозитории, ты можешь создать локальную ветку, отслеживающую удалённую, с помощью команды:


git checkout -b trace-monitor origin/trace-monitor
Эта команда создаст локальную ветку trace-monitor, которая будет отслеживать

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git fetch --all
remote: Enumerating objects: 6, done.
remote: Counting objects: 100% (6/6), done.
remote: Compressing objects: 100% (6/6), done.
remote: Total 6 (delta 1), reused 0 (delta 0), pack-reused 0 (from 0)
Unpacking objects: 100% (6/6), 2.87 KiB | 735.00 KiB/s, done.
From https://github.com/Gazmasater/bpfgo
   f7e6fef..ea44eee  trace_monitor -> origin/trace_monitor
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git checkout -b trace-monitor origin/trace-monitor
fatal: 'origin/trace-monitor' is not a commit and a branch 'trace-monitor' cannot be created from it
