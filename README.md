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

type ruleExprEncoderTestSuite struct {
	suite.Suite
}

func (sui *ruleExprEncoderTestSuite) Test_IpDaddrRuleToString() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "ip daddr only",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					// Сравниваем поле ip daddr с 192.168.1.10
					&expr.Payload{
						DestRegister: 1,
						Base:         expr.PayloadBaseNetworkHeader, // 0
						Offset:       16,  // daddr offset
						Len:          4,   // IPv4
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
		{
			name: "ip daddr with dup",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Payload{
						DestRegister: 1,
						Base:         expr.PayloadBaseNetworkHeader,
						Offset:       16,
						Len:          4,
					},
					&expr.Cmp{
						Op:       expr.CmpOpEq,
						Register: 1,
						Data:     []byte{10, 1, 2, 3},
					},
					&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "ip daddr 10.1.2.3 dup to 192.168.1.10",
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

func Test_RuleExprEncoder(t *testing.T) {
	suite.Run(t, new(ruleExprEncoderTestSuite))
}












