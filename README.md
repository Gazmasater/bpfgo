gcc udp_server.c -o udp_server

ip addr show

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ss -tulpn
Netid     State      Recv-Q     Send-Q         Local Address:Port          Peer Address:Port     Process     
udp       UNCONN     0          0                    0.0.0.0:40040              0.0.0.0:*                    
udp       UNCONN     0          0                 127.0.0.54:53                 0.0.0.0:*                    
udp       UNCONN     0          0              127.0.0.53%lo:53                 0.0.0.0:*                    
udp       UNCONN     0          0                    0.0.0.0:5353               0.0.0.0:*                    
udp       UNCONN     0          0                       [::]:40733                 [::]:*                    
udp       UNCONN     0          0                       [::]:5353                  [::]:*                    
tcp       LISTEN     0          4096              127.0.0.54:53                 0.0.0.0:*                    
tcp       LISTEN     0          4096           127.0.0.53%lo:53                 0.0.0.0:*                    
tcp       LISTEN     0          4096               127.0.0.1:631                0.0.0.0:*                    
tcp       LISTEN     0          4096                   [::1]:631                   [::]:*                    
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
