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
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"time"
)

type dynsetEncoderTestSuite struct {
	suite.Suite
}

func (sui *dynsetEncoderTestSuite) Test_DynsetEncodeIR() {
	testData := []struct {
		name     string
		dynset   *expr.Dynset
		ctx      *ctx
		expected string
	}{
		{
			name: "add to set",
			dynset: &expr.Dynset{
				Operation:  DynSetOPAdd,
				SetName:    "myset",
				SrcRegKey:  1,
				Timeout:    0,
			},
			ctx: &ctx{
				rule: &RuleStub{},
				reg:  map[regID]regVal{1: {HumanExpr: "10.0.0.1"}},
			},
			expected: "add @myset { 10.0.0.1 }",
		},
		{
			name: "add to set with timeout",
			dynset: &expr.Dynset{
				Operation:  DynSetOPAdd,
				SetName:    "myset",
				SrcRegKey:  1,
				Timeout:    5 * time.Second,
			},
			ctx: &ctx{
				rule: &RuleStub{},
				reg:  map[regID]regVal{1: {HumanExpr: "192.168.0.2"}},
			},
			expected: "add @myset { 192.168.0.2 timeout 5s }",
		},
		{
			name: "update set",
			dynset: &expr.Dynset{
				Operation:  DynSetOPUpdate,
				SetName:    "myset",
				SrcRegKey:  1,
			},
			ctx: &ctx{
				rule: &RuleStub{},
				reg:  map[regID]regVal{1: {HumanExpr: "testkey"}},
			},
			expected: "update @myset { testkey }",
		},
		{
			name: "delete from set",
			dynset: &expr.Dynset{
				Operation:  DynSetOPDelete,
				SetName:    "myset",
				SrcRegKey:  1,
			},
			ctx: &ctx{
				rule: &RuleStub{},
				reg:  map[regID]regVal{1: {HumanExpr: "192.168.1.100"}},
			},
			expected: "delete @myset { 192.168.1.100 }",
		},
	}

	for _, tt := range testData {
		sui.Run(tt.name, func() {
			enc := &dynsetEncoder{dynset: tt.dynset}
			node, err := enc.EncodeIR(tt.ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tt.expected, node.String())
		})
	}
}

// Простой stub для ctx и Rule (можешь заменить на свои типы, если они другие)
type RuleStub struct{}

func (r *RuleStub) String() string { return "" }

type regVal struct {
	HumanExpr string
}

type ctx struct {
	rule *RuleStub
	reg  map[regID]regVal
}

func (c *ctx) regID(id int) regVal {
	return c.reg[regID(id)]
}

type regID int

func (r regID) String() string { return "" }

func Test_DynsetEncoder(t *testing.T) {
	suite.Run(t, new(dynsetEncoderTestSuite))
}





