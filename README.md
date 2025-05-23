package encoders

import (
	"testing"
	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dupEncodersTestSuite struct {
	suite.Suite
}

func (sui *dupEncodersTestSuite) Test_DupExprToString() {
	testData := []struct {
		name     string
		setup    func(ctx *ctx)
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "Dup to address",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(1), regVal{HumanExpr: "1.2.3.4"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegAddr: 1, // берём адрес из регистра 1
					},
				},
			},
			expected: "dup to 1.2.3.4",
		},
		{
			name: "Dup to address and device",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(1), regVal{HumanExpr: "10.1.1.1"})
				ctx.reg.Set(regID(2), regVal{HumanExpr: "eth0"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegAddr: 1,
						RegDev:  2,
					},
				},
			},
			expected: "dup to 10.1.1.1 device eth0",
		},
		{
			name: "Dup to device only",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(2), regVal{HumanExpr: "eth1"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegDev: 2,
					},
				},
			},
			expected: "dup",
		},
		{
			name: "Dup no params",
			setup: func(ctx *ctx) {
				// Ничего не делаем, нет адреса и девайса
			},
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
			c := newCtx() // или свой способ создания *ctx
			if t.setup != nil {
				t.setup(c)
			}
			enc := NewRuleExprEncoder(&t.exprs)
			enc.ctx = c
			str, err := enc.Format()
			sui.Require().NoError(err)
			sui.Require().Equal(t.expected, str)
		})
	}
}

func (sui *dupEncodersTestSuite) Test_DupExprToJSON() {
	testData := []struct {
		name     string
		setup    func(ctx *ctx)
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "Dup to address",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(1), regVal{Data: "1.2.3.4"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegAddr: 1,
					},
				},
			},
			expected: `{"dup":{"addr":"1.2.3.4"}}`,
		},
		{
			name: "Dup to address and device",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(1), regVal{Data: "10.1.1.1"})
				ctx.reg.Set(regID(2), regVal{Data: "eth0"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegAddr: 1,
						RegDev:  2,
					},
				},
			},
			expected: `{"dup":{"addr":"10.1.1.1","dev":"eth0"}}`,
		},
		{
			name: "Dup to device only",
			setup: func(ctx *ctx) {
				ctx.reg.Set(regID(2), regVal{Data: "eth1"})
			},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{
						RegDev: 2,
					},
				},
			},
			expected: `{"dup":{"dev":"eth1"}}`,
		},
		{
			name: "Dup no params",
			setup: func(ctx *ctx) {},
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{},
				},
			},
			expected: `{"dup":{"addr":null,"dev":null}}`,
		},
	}
	for _, t := range testData {
		sui.Run(t.name, func() {
			c := newCtx() // или свой способ создания *ctx
			if t.setup != nil {
				t.setup(c)
			}
			enc := NewRuleExprEncoder(&t.exprs)
			enc.ctx = c
			b, err := enc.MarshalJSON()
			sui.Require().NoError(err)
			sui.Require().JSONEq(t.expected, string(b))
		})
	}
}

// Подключаем сьют к тестам
func Test_DupEncoders(t *testing.T) {
	suite.Run(t, new(dupEncodersTestSuite))
}





