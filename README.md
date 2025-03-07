gcc udp_server.c -o udp_server

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ss -tulpn
Netid State   Recv-Q  Send-Q   Local Address:Port    Peer Address:Port Process                               
udp   UNCONN  0       0           127.0.0.54:53           0.0.0.0:*                                          
udp   UNCONN  0       0        127.0.0.53%lo:53           0.0.0.0:*                                          
udp   UNCONN  0       0            127.0.0.1:33333        0.0.0.0:*     users:(("udp_server",pid=5072,fd=3)) 
udp   UNCONN  0       0              0.0.0.0:58765        0.0.0.0:*                                          
udp   UNCONN  0       0              0.0.0.0:5353         0.0.0.0:*                                          
udp   UNCONN  0       0                 [::]:52277           [::]:*                                          
udp   UNCONN  0       0                 [::]:5353            [::]:*                                          
tcp   LISTEN  0       4096     127.0.0.53%lo:53           0.0.0.0:*                                          
tcp   LISTEN  0       1            127.0.0.1:12345        0.0.0.0:*     users:(("nc",pid=4897,fd=3))         
tcp   LISTEN  0       4096         127.0.0.1:631          0.0.0.0:*                                          
tcp   LISTEN  0       4096        127.0.0.54:53           0.0.0.0:*                                          
tcp   LISTEN  0       4096             [::1]:631             [::]:*     
