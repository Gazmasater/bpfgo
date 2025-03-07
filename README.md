gcc udp_server.c -o udp_server

ls /sys/fs/cgroup

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ls /sys/fs/cgroup
cgroup.controllers      cpu.pressure           init.scope        memory.reclaim                 sys-kernel-debug.mount
cgroup.max.depth        cpuset.cpus.effective  io.cost.model     memory.stat                    sys-kernel-tracing.mount
cgroup.max.descendants  cpuset.cpus.isolated   io.cost.qos       memory.zswap.writeback         system.slice
cgroup.pressure         cpuset.mems.effective  io.pressure       misc.capacity                  user.slice
cgroup.procs            cpu.stat               io.prio.class     misc.current
cgroup.stat             cpu.stat.local         io.stat           proc-sys-fs-binfmt_misc.mount
cgroup.subtree_control  dev-hugepages.mount    memory.numa_stat  sys-fs-fuse-connections.mount
cgroup.threads          dev-mqueue.mount       memory.pressure   sys-kernel-config.mount
