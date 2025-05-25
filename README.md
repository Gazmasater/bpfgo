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

func (sui *dynsetEncoderTestSuite) Test_DynsetEncodeIR_Complex() {
	testData := []struct {
		name     string
		dynset   *expr.Dynset
		srcKey   string
		srcData  string
		expected string
	}{
		{
			name:   "add with log expr",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPAdd),
				SetName:   "myset",
				SrcRegKey: 1,
				Exprs: []expr.Any{
					&expr.Log{},
				},
			},
			srcKey:   "10.10.10.10",
			expected: "add @myset { 10.10.10.10 log }",
		},
		{
			name:   "add with timeout and counter",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPAdd),
				SetName:   "myset",
				SrcRegKey: 2,
				Timeout:   20 * time.Second,
				Exprs: []expr.Any{
					&expr.Counter{},
				},
			},
			srcKey:   "172.16.0.7",
			expected: "add @myset { 172.16.0.7 timeout 20s counter packets 0 bytes 0 }",
		},
		{
			name:   "update with map style (srcRegData)",
			dynset: &expr.Dynset{
				Operation:  uint32(DynSetOPUpdate),
				SetName:    "myset",
				SrcRegKey:  3,
				SrcRegData: 4,
			},
			srcKey:   "mapkey",
			srcData:  "mapval",
			expected: "update @myset { mapkey : mapval }",
		},
		{
			name:   "delete with counter and timeout",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPDelete),
				SetName:   "myset",
				SrcRegKey: 5,
				Timeout:   30 * time.Second,
				Exprs: []expr.Any{
					&expr.Counter{},
				},
			},
			srcKey:   "192.0.2.55",
			expected: "delete @myset { 192.0.2.55 timeout 30s counter packets 0 bytes 0 }",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			var reg regHolder
			reg.Set(regID(tc.dynset.SrcRegKey), regVal{
				HumanExpr: tc.srcKey,
			})
			if tc.dynset.SrcRegData != 0 && tc.srcData != "" {
				reg.Set(regID(tc.dynset.SrcRegData), regVal{
					HumanExpr: tc.srcData,
				})
			}

			ctx := &ctx{
				reg:  reg,
				rule: &nftables.Rule{},
			}

			enc := &dynsetEncoder{dynset: tc.dynset}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.String())
		})
	}
}

func Test_DynsetEncoder(t *testing.T) {
	suite.Run(t, new(dynsetEncoderTestSuite))
}





