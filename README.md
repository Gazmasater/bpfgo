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
	"time"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dynsetEncoderTestSuite struct {
	suite.Suite
}

func (sui *dynsetEncoderTestSuite) Test_DynsetEncodeIR() {
	testData := []struct {
		name     string
		dynset   *expr.Dynset
		srcKey   string
		timeout  time.Duration
		op       DynSetOP
		expected string
	}{
		{
			name:     "add",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPAdd), SetName: "myset", SrcRegKey: 1},
			srcKey:   "10.0.0.1",
			expected: "add @myset { 10.0.0.1 }",
		},
		{
			name:     "add with timeout",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPAdd), SetName: "myset", SrcRegKey: 2, Timeout: 10 * time.Second},
			srcKey:   "192.168.1.10",
			timeout:  10 * time.Second,
			expected: "add @myset { 192.168.1.10 timeout 10s }",
		},
		{
			name:     "update",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPUpdate), SetName: "myset", SrcRegKey: 3},
			srcKey:   "testkey",
			expected: "update @myset { testkey }",
		},
		{
			name:     "delete",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPDelete), SetName: "myset", SrcRegKey: 4},
			srcKey:   "remove_this",
			expected: "delete @myset { remove_this }",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			// Настраиваем regHolder с нужным regVal по srcRegKey
			var reg regHolder
			reg.Set(regID(tc.dynset.SrcRegKey), regVal{
				HumanExpr: tc.srcKey,
			})

			ctx := &ctx{
				reg:  reg,
				rule: &nftables.Rule{},
			}

			enc := &dynsetEncoder{dynset: tc.dynset}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func Test_DynsetEncoder(t *testing.T) {
	suite.Run(t, new(dynsetEncoderTestSuite))
}




