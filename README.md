sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log



sudo nft add table inet test
sudo nft add chain inet test mychain '{ type filter hook input priority 0; }'
sudo nft add rule inet test mychain tcp option 5

sudo nft add rule inet test mychain ip option @10,1,8 set someval
sudo nft add rule inet test mychain reset ip option @4,2,1
sudo nft add rule inet test mychain tcp option 5
sudo nft add rule inet test mychain ip option @7,1,2
sudo nft add rule inet test mychain ip option @4,8,5 set custom
sudo nft add rule inet test mychain reset tcp option @7,2,4




package encoders

import (
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type exthdrEncoderIRTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderIRTestSuite) Test_ExthdrExprToIR() {
	testData := []struct {
		name        string
		exthdr      *expr.Exthdr
		regSetup    func(*ctx)
		expected    string
		expectedErr string
	}{
		{
			name: "DestRegister exthdr",
			exthdr: &expr.Exthdr{
				DestRegister: 1,
				Type:         5,
				Offset:       0,
				Len:          4,
				Op:           expr.ExthdrOpTcpopt,
				Flags:        unix.NFT_EXTHDR_F_PRESENT,
			},
			expected: "tcp option 5",
		},
		{
			name: "SourceRegister exthdr set",
			exthdr: &expr.Exthdr{
				SourceRegister: 2,
				Type:           10,
				Offset:         1,
				Len:            8,
				Op:             expr.ExthdrOpIpv6,
			},
			regSetup: func(ctx *ctx) {
				ctx.reg.Set(regID(2), regVal{HumanExpr: "someval"})
			},
			expected: "ip option @10,1,8 set someval",
		},
		{
			name: "reset exthdr",
			exthdr: &expr.Exthdr{
				Type:   4,
				Op:     expr.ExthdrOpIpv6,
				Offset: 2,
				Len:    1,
			},
			expected: "reset ip option @4,2,1",
		},
		{
			name: "SourceRegister missing in ctx",
			exthdr: &expr.Exthdr{
				SourceRegister: 7,
				Type:           4,
				Op:             expr.ExthdrOpIpv6,
				Offset:         1,
				Len:            2,
			},
			expectedErr: "has no expression",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{reg: regHolder{}}
			if tc.regSetup != nil {
				tc.regSetup(ctx)
			}
			enc := &exthdrEncoder{extdhdr: tc.exthdr}
			ir, err := enc.EncodeIR(ctx)
			if tc.expectedErr != "" {
				sui.Require().Error(err)
				sui.Require().Contains(err.Error(), tc.expectedErr)
			} else {
				if tc.exthdr.DestRegister != 0 {
					sui.Require().ErrorIs(err, ErrNoIR)
					val, ok := ctx.reg.Get(regID(tc.exthdr.DestRegister))
					sui.Require().True(ok)
					sui.Require().Contains(val.HumanExpr, tc.expected)
					sui.Require().Nil(ir)
				} else {
					sui.Require().NoError(err)
					sui.Require().Equal(tc.expected, ir.Format())
				}
			}
		})
	}
}

func Test_ExthdrEncoderIR(t *testing.T) {
	suite.Run(t, new(exthdrEncoderIRTestSuite))
}

sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting tcp option 5

sudo nft add rule ip test prerouting ip option @10,1,8 set someval

sudo nft add rule ip test prerouting reset ip option @4,2,1




