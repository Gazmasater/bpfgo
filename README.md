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
			name: "ct state new",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Ct{
						Key:      expr.CtKeySTATE,
						Register: 1,
					},
					&expr.Cmp{
						Register: 1,
						Op:       expr.CmpOpEq,
						Data:     []byte{0x01, 0, 0, 0, 0, 0, 0, 0}, // только new
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct state new accept",
		},

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
						Data: []byte{0x88, 0x13, 0x00, 0x00},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct expiration 5s accept",
		},
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
						Data: []byte{0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ct status assured accept",
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


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
Actual=ct state invalid accept
Expected=ct state new accept
Actual=ct state established,related accept
Expected=ct state established,related accept
Actual=ct direction original accept
Expected=ct direction original accept
Actual=ct protocol tcp accept
Expected=ct protocol tcp accept
Actual=ct mark 1 accept
Expected=ct mark 1 accept
Actual=ct direction reply accept
Expected=ct direction reply accept
Actual=ct mark 255 accept
Expected=ct mark 255 accept
Actual=ct expiration 5s accept
Expected=ct expiration 5s accept
Actual=ct protocol udp accept
Expected=ct protocol udp accept
Actual=ct status assured accept
Expected=ct status assured accept
--- FAIL: Test_CtEncoder (0.00s)
    --- FAIL: Test_CtEncoder/Test_CtExprToString (0.00s)
        --- FAIL: Test_CtEncoder/Test_CtExprToString/ct_state_new (0.00s)
            ct_test.go:219: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/ct_test.go:219
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ct state new accept"
                                        actual  : "ct state invalid accept"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ct state new accept
                                        +ct state invalid accept
                        Test:           Test_CtEncoder/Test_CtExprToString/ct_state_new
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.008s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 
