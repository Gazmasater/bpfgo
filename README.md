sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type bitwiseEncoderTestSuite struct {
	suite.Suite
}

func (sui *bitwiseEncoderTestSuite) Test_IpDaddrBitwise24() {
	rule := nftables.Rule{
		Table: &nftables.Table{Name: "test"},
		Exprs: []expr.Any{
			// 1. ip daddr (payload, offset 16, len 4)
			&expr.Payload{
				DestRegister: 1,
				Base:         expr.PayloadBaseNetworkHeader,
				Offset:       16,
				Len:          4,
			},
			// 2. bitwise (255.255.255.0 - маска /24)
			&expr.Bitwise{
				SourceRegister: 1,
				DestRegister:   1,
				Len:            4,
				Mask:           []byte{255, 255, 255, 0},
				Xor:            []byte{0, 0, 0, 0},
			},
			// 3. cmp (192.168.1.0)
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{192, 168, 1, 0},
			},
			// 4. accept
			&expr.Verdict{
				Kind: expr.VerdictAccept,
			},
		},
	}

	expected := "ip daddr 192.168.1.0/24 accept"
	str, err := NewRuleExprEncoder(&rule).Format()
	sui.Require().NoError(err)
	sui.Require().Equal(expected, str)
}

func Test_BitwiseEncoder(t *testing.T) {
	suite.Run(t, new(bitwiseEncoderTestSuite))
}

gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_BitwiseEncoder (0.00s)
    --- FAIL: Test_BitwiseEncoder/Test_IpDaddrBitwise24 (0.00s)
        bitwise_test.go:50: 
                Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:50
                Error:          Not equal: 
                                expected: "ip daddr 192.168.1.0/24 accept"
                                actual  : "@nh,16,4/0xffffff00 192.168.1.0 accept"
                            
                                Diff:
                                --- Expected
                                +++ Actual
                                @@ -1 +1 @@
                                -ip daddr 192.168.1.0/24 accept
                                +@nh,16,4/0xffffff00 192.168.1.0 accept
                Test:           Test_BitwiseEncoder/Test_IpDaddrBitwise24
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.007s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 







