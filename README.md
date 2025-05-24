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
	package encoders

import (
	"fmt"
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
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state established,related accept",
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
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct direction original accept",
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
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct protocol tcp accept",
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
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct mark 1 accept",
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
suite.Run(t, new(dupEncoderTestSuite))
}



1. ct state с несколькими статусами
nft:


ct state new,established,related accept
Go:


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
				Data:     []byte{0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state new,established,related accept",
},
2. ct direction reply
nft:


ct direction reply accept
Go:


{
	name: "ct direction reply",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyDIRECTION,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{0x01}, // reply
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct direction reply accept",
},
3. ct mark с более сложным значением
nft:


ct mark 255 accept
Go:


{
	name: "ct mark 255",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyMARK,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{0xff, 0x00, 0x00, 0x00},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct mark 255 accept",
},
4. ct expiration
nft:


ct expiration 5s accept
Go:


{
	name: "ct expiration 5s",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyEXPIRATION,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				// 5000 мс (5 секунд) — в LE uint32
				Data:     []byte{0x88, 0x13, 0x00, 0x00},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct expiration 5s accept",
},
5. ct protocol udp
nft:


ct protocol udp accept
Go:


{
	name: "ct protocol udp",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyPROTOCOL,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{17}, // UDP
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct protocol udp accept",
},
6. ct status
Если твой энкодер поддерживает status:


ct status assured accept
Go:


{
	name: "ct status assured",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATUS,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				// 0x04 — ASSURED (по bitmask, как и для state)
				Data:     []byte{0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct status assured accept",
},



// {
		// 	name: "ct state multi",
		// 	exprs: nftables.Rule{
		// 		Exprs: []expr.Any{
		// 			&expr.Ct{
		// 				Key:      expr.CtKeySTATE,
		// 				Register: 1,
		// 			},
		// 			&expr.Cmp{
		// 				Register: 1,
		// 				Op:       expr.CmpOpEq,
		// 				// new = 0x01, established = 0x02, related = 0x04, итого mask = 0x07
		// 				Data: []byte{0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		// 			},
		// 			&expr.Verdict{Kind: expr.VerdictAccept},
		// 		},
		// 	},
		// 	expected: "ct state new,established,related accept",
		// },


  sudo nft add rule ip test prerouting ct state new,established,related accept







