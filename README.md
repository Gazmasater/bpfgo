sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept



gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
Actual=ct state established,related accept
Expected=ct state established,related accept
Actual=ct direction original accept
Expected=ct direction original accept
Actual=ct protocol tcp accept
Expected=ct protocol tcp accept
Actual=ct mark 1 accept
Expected=ct mark 1 accept
Actual=ct state invalid,established,related accept
Expected=ct state new,established,related accept
Actual=ct direction reply accept
Expected=ct direction reply accept
Actual=ct mark 255 accept
Expected=ct mark 255 accept
Actual=ct expiration 5s accept
Expected=ct expiration 5s accept
Actual=ct protocol udp accept
Expected=ct protocol udp accept
Actual=ct status assured accept
Expected=ct status assured accept
--- FAIL: Test_CtEncoder (0.00s)
    --- FAIL: Test_CtEncoder/Test_CtExprToString (0.00s)
        --- FAIL: Test_CtEncoder/Test_CtExprToString/ct_state_multi (0.00s)
            ct_test.go:218: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/ct_test.go:218
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ct state new,established,related accept"
                                        actual  : "ct state invalid,established,related accept"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ct state new,established,related accept
                                        +ct state invalid,established,related accept
                        Test:           Test_CtEncoder/Test_CtExprToString/ct_state_multi
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.008s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 

		{
			name: "ct state multi",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						// new = 0x01, established = 0x02, related = 0x04, итого mask = 0x07
						Data: []byte{0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state new,established,related accept",
		},







