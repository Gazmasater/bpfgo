sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test






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
					&expr.Immediate{Register: 2, Data: []byte("eth0")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expected: "dup to 192.168.1.10 device eth0",
		},
		{
			name: "dup only device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 2, Data: []byte("br-lan")},
					&expr.Dup{RegDev: 2},
				},
			},
			expected: "dup",
		},
		{
			name: "dup no params",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{},
				},
			},
			expected: "dup",
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


