sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept


sudo nft add rule ip test prerouting add @myset { 10.0.0.1 }

sudo nft add rule ip test prerouting add @myset { 192.168.1.10 timeout 10s }

sudo nft add rule ip test prerouting update @myset { testkey }



package encoders

import (
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type bitwiseEncoderTestSuite struct {
	suite.Suite
}

func (sui *bitwiseEncoderTestSuite) Test_BitwiseEncodeIR() {
	testData := []struct {
		name     string
		srcReg   uint32
		dstReg   uint32
		mask     []byte
		xor      []byte
		srcExpr  expr.Any
		srcHuman string
		expected string
	}{
		{
			name:     "bitwise AND mask",
			srcReg:   1,
			dstReg:   2,
			mask:     []byte{0xf0},
			xor:      nil,
			srcExpr:  nil,
			srcHuman: "ip daddr",
			expected: "(ip daddr) & 0xf0",
		},
		{
			name:     "bitwise AND+XOR mask",
			srcReg:   3,
			dstReg:   4,
			mask:     []byte{0xff},
			xor:      []byte{0x0f},
			srcExpr:  nil,
			srcHuman: "meta mark",
			expected: "((meta mark) & 0xff) ^ 0xf",
		},
		{
			name:     "bitwise with OR (edge)",
			srcReg:   5,
			dstReg:   6,
			mask:     []byte{0xf0},
			xor:      []byte{0xf0},
			srcExpr:  nil,
			srcHuman: "payload 2",
			expected: "((payload 2) & 0xf0) ^ 0xf0",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			var reg regHolder
			reg.Set(regID(tc.srcReg), regVal{
				HumanExpr: tc.srcHuman,
				Expr:      tc.srcExpr,
			})
			ctx := &ctx{
				reg:  reg,
				rule: nil,
			}

			bitwise := &expr.Bitwise{
				SourceRegister: tc.srcReg,
				DestRegister:   tc.dstReg,
				Mask:           tc.mask,
				Xor:            tc.xor,
				Len:            uint32(len(tc.mask)),
			}
			enc := &bitwiseEncoder{bitwise: bitwise}
			_, err := enc.EncodeIR(ctx)
			// Архитектурно допускаем отсутствие IR
			if err != nil && err != ErrNoIR {
				sui.Require().NoError(err)
			}
			res, ok := ctx.reg.Get(regID(tc.dstReg))
			sui.Require().True(ok)
			sui.Require().Equal(tc.expected, res.HumanExpr)
		})
	}
}

func Test_BitwiseEncoder(t *testing.T) {
	suite.Run(t, new(bitwiseEncoderTestSuite))
}








