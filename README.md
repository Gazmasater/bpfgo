sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log



gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_DupEncoder (0.00s)
    --- FAIL: Test_DupEncoder/Test_DupExprToString (0.00s)
        --- FAIL: Test_DupEncoder/Test_DupExprToString/oifname_ip_daddr_counter_log (0.00s)
            dup_test.go:62: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dup_test.go:62
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "oifname lo ip daddr 192.168.1.10 counter packets 0 bytes 0 log"
                                        actual  : "oifname lo @ll,16,4 3232235786 counter packets 0 bytes 0 log"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -oifname lo ip daddr 192.168.1.10 counter packets 0 bytes 0 log
                                        +oifname lo @ll,16,4 3232235786 counter packets 0 bytes 0 log
                        Test:           Test_DupEncoder/Test_DupExprToString/oifname_ip_daddr_counter_log
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.007s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 
