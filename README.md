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


package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

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
			name: "ct state",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // established,related (битовая маска)
					},
				},
			},
			expected: "ct state established,related",
		},
		{
			name: "ct direction",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeyDIRECTION,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{0x00}, // original
					},
				},
			},
			expected: "ct direction original",
		},
		{
			name: "ct protocol",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeyPROTOCOL,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{6}, // TCP
					},
				},
			},
			expected: "ct protocol tcp",
		},
		{
			name: "ct mark",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeyMARK,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{0x01, 0x00, 0x00, 0x00},
					},
				},
			},
			expected: "ct mark 1",
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

func Test_CtEncoder(t *testing.T) {
	suite.Run(t, new(ctEncoderTestSuite))
}


