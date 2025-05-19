if (BPF_CORE_READ_INTO(info.comm, conn_info, comm) < 0)
    goto cleanup;

az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
[sudo] password for gaz358: 
2025/05/19 17:57:49 failed to load bpf objects: field TraceSendtoExit: program trace_sendto_exit: load program: bad CO-RE relocation: invalid func unknown#195896080 (
    

