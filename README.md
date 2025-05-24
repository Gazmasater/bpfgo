sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept




package encoders

import (
	"fmt"
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

// Объявляем константы битов CT state (если вдруг их нет в expr)

type ctEncoderTestSuite struct {
	suite.Suite
}

func (sui *ctEncoderTestSuite) Test_CtExprToString() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "ct state new",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{byte(CtStateBitNEW), 0, 0, 0, 0, 0, 0, 0}, // только new
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state new accept",
		},
		{
			name: "ct state established,related",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{byte(CtStateBitESTABLISHED | CtStateBitRELATED), 0, 0, 0, 0, 0, 0, 0},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state established,related accept",
		},
		{
			name: "ct state new,established,related",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{byte(CtStateBitNEW | CtStateBitESTABLISHED | CtStateBitRELATED), 0, 0, 0, 0, 0, 0, 0},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state new,established,related accept",
		},
		{
			name: "ct state invalid",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{byte(CtStateBitINVALID), 0, 0, 0, 0, 0, 0, 0}, // только invalid
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state invalid accept",
		},
		{
			name: "ct state new,established,invalid",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{byte(CtStateBitNEW | CtStateBitESTABLISHED | CtStateBitINVALID), 0, 0, 0, 0, 0, 0, 0},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state new,established,invalid accept",
		},
	}

	for _, t := range testData {
		sui.Run(t.name, func() {
			str, err := NewRuleExprEncoder(&t.exprs).Format()
			sui.Require().NoError(err)
			fmt.Printf("Actual=%s\n", str)
			fmt.Printf("Expected=%s\n", t.expected)
			sui.Require().Equal(t.expected, str)
		})
	}
}

func Test_CtEncoder(t *testing.T) {
	suite.Run(t, new(ctEncoderTestSuite))
}




