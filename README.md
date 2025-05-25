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

type hashEncoderTestSuite struct {
	suite.Suite
}

func (sui *hashEncoderTestSuite) Test_HashEncodeIR() {
	testData := []struct {
		name     string
		hash     *expr.Hash
		srcReg   uint32
		srcHuman string
		expected string
	}{

		{
			name: "jhash with src and offset",
			hash: &expr.Hash{
				Type:           expr.HashTypeJenkins, // jhash (jenkins)
				Modulus:        123,
				Seed:           0xbeef,
				Offset:         42,
				SourceRegister: 7,
				DestRegister:   8,
			},
			srcReg:   7,
			srcHuman: "ip saddr",
			expected: "symhashjhash ip saddr mod 123 seed 0xbeef offset 42", // если твой encoder так строит строку
		},
		{
			name: "simple symhash",
			hash: &expr.Hash{
				Type:         expr.HashTypeSym,
				Modulus:      10,
				Seed:         0x1234,
				Offset:       0,
				DestRegister: 1,
			},
			expected: "symhash mod 10 seed 0x1234",
		},
		{
			name: "symhash with offset",
			hash: &expr.Hash{
				Type:         expr.HashTypeSym,
				Modulus:      16,
				Seed:         0xdead,
				Offset:       7,
				DestRegister: 2,
			},
			expected: "symhash mod 16 seed 0xdead offset 7",
		},
		{
			name: "jhash with src",
			hash: &expr.Hash{
				Type:           expr.HashTypeJenkins, // используем jhash (jenkins)
				Modulus:        5,
				Seed:           0x1111,
				Offset:         0,
				SourceRegister: 3,
				DestRegister:   4,
			},
			srcReg:   3,
			srcHuman: "meta mark",
			expected: "symhashjhash meta mark mod 5 seed 0x1111", // если твой encoder так формирует строку
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			var reg regHolder
			if tc.srcReg != 0 {
				reg.Set(regID(tc.srcReg), regVal{
					HumanExpr: tc.srcHuman,
				})
			}
			ctx := &ctx{
				reg:  reg,
				rule: nil, // не нужен
			}

			enc := &hashEncoder{hash: tc.hash}
			_, err := enc.EncodeIR(ctx)
			if err != nil && err != ErrNoIR {
				sui.Require().NoError(err)
			}

			res, ok := ctx.reg.Get(regID(tc.hash.DestRegister))
			sui.Require().True(ok)
			sui.Require().Equal(tc.expected, res.HumanExpr)
		})
	}
}

func Test_HashEncoder(t *testing.T) {
	suite.Run(t, new(hashEncoderTestSuite))
}


sudo nft add rule ip test prerouting jhash ip saddr mod 123 seed 0xbeef offset 42

sudo nft add rule ip test prerouting symhash jhash ip saddr mod 123 seed 0xbeef offset 42

sudo nft add rule ip test prerouting symhash mod 10 seed 0x1234

sudo nft add rule ip test prerouting symhash mod 16 seed 0xdead offset 7

sudo nft add rule ip test prerouting jhash meta mark mod 5 seed 0x1111

sudo nft add rule ip test prerouting symhash jhash meta mark mod 5 seed 0x1111




gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip test prerouting jhash ip saddr mod 123 seed 0xbeef offset 42
Error: syntax error, unexpected newline
add rule ip test prerouting jhash ip saddr mod 123 seed 0xbeef offset 42
                                                                        ^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip test prerouting symhash jhash ip saddr mod 123 seed 0xbeef offset 42
Error: syntax error, unexpected jhash, expecting mod
add rule ip test prerouting symhash jhash ip saddr mod 123 seed 0xbeef offset 42
                                    ^^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 




