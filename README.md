sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log



package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dupEncoderTestSuite struct {
	suite.Suite
}

func (sui *dupEncoderTestSuite) Test_DupExprToString() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "dup to address",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("10.1.2.3")},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "dup to 10.1.2.3",
		},
		{
			name: "dup to address and device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
					&expr.Immediate{Register: 2, Data: []byte("lo")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expected: "dup to 192.168.1.10 device lo",
		},
		{
			name: "oifname ip daddr counter log",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Meta{Key: expr.MetaKeyOIFNAME, Register: 1},
					&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte("lo")},
					&expr.Payload{DestRegister: 1, Base: 0, Offset: 16, Len: 4},
					&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{192, 168, 1, 10}},
					&expr.Counter{},
					&expr.Log{},
				},
			},
			expected: "oifname lo ip daddr 192.168.1.10 counter packets 0 bytes 0 log",
		},
	}

	for _, t := range testData {
		sui.Run(t.name, func() {
			str, err := NewRuleExprEncoder(&t.exprs).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(t.expected, str)
		})
	}
}

func Test_DupEncoder(t *testing.T) {
	suite.Run(t, new(dupEncoderTestSuite))
}

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
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.008s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 



{
    name: "ip daddr cmp",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Payload{
                DestRegister: 1,
                Base:         expr.PayloadBaseNetworkHeader, // обычно = 0
                Offset:       16, // ip daddr offset
                Len:          4,  // IPv4
            },
            &expr.Cmp{
                Op:       expr.CmpOpEq,
                Register: 1,
                Data:     []byte{192, 168, 1, 10},
            },
        },
    },
    expected: "ip daddr 192.168.1.10",
},














