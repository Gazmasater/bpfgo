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
					// Кладём адрес в регистр 1 (например, 10.1.2.3)
					&expr.Immediate{Register: 1, Data: []byte{10, 1, 2, 3}},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "dup to 10.1.2.3",
		},
		{
			name: "dup to address and device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					// Кладём адрес в регистр 1
					&expr.Immediate{Register: 1, Data: []byte{192, 168, 100, 200}},
					// Кладём строку (имя интерфейса) в регистр 2
					&expr.Immediate{Register: 2, Data: []byte("eth10")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expected: "dup to 192.168.100.200 device eth10",
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

func (sui *dupEncoderTestSuite) Test_DupExprToJSON() {
	testData := []struct {
		name    string
		exprs   nftables.Rule
		expJson string
	}{
		{
			name: "dup to address",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte{10, 1, 2, 3}},
					&expr.Dup{RegAddr: 1},
				},
			},
			expJson: `{"dup":{"addr":"10.1.2.3","dev":null}}`,
		},
		{
			name: "dup to address and device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte{192, 168, 100, 200}},
					&expr.Immediate{Register: 2, Data: []byte("eth10")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expJson: `{"dup":{"addr":"192.168.100.200","dev":"eth10"}}`,
		},
		{
			name: "dup only device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 2, Data: []byte("br-lan")},
					&expr.Dup{RegDev: 2},
				},
			},
			expJson: `{"dup":{"addr":null,"dev":"br-lan"}}`,
		},
		{
			name: "dup no params",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{},
				},
			},
			expJson: `{"dup":{"addr":null,"dev":null}}`,
		},
	}

	for _, t := range testData {
		sui.Run(t.name, func() {
			b, err := NewRuleExprEncoder(&t.exprs).MarshalJSON()
			sui.Require().NoError(err)
			sui.Require().JSONEq(t.expJson, string(b))
		})
	}
}

func Test_DupEncoder(t *testing.T) {
	suite.Run(t, new(dupEncoderTestSuite))
}





