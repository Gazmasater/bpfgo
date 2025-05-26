sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept

sudo nft list ruleset


package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type exthdrEncoderTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderTestSuite) Test_ExthdrDstExistsAccept() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "exthdr dst exists accept",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:     expr.ExthdrOpIpv6,
						Type:   60, // 60 — Destination Options Header (dst)
						Offset: 0,
						Len:    0,
						Flags:  unix.NFT_EXTHDR_F_PRESENT,
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "ip option 60 accept",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			str, err := NewRuleExprEncoder(&tc.exprs).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, str)
		})
	}
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_ExthdrEncoder (0.00s)
    --- FAIL: Test_ExthdrEncoder/Test_ExthdrDstExistsAccept (0.00s)
        --- FAIL: Test_ExthdrEncoder/Test_ExthdrDstExistsAccept/exthdr_dst_exists_accept (0.00s)
            exthdr_test.go:44: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/exthdr_test.go:44
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ip option 60 accept"
                                        actual  : "reset ip option 60 accept"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ip option 60 accept
                                        +reset ip option 60 accept
                        Test:           Test_ExthdrEncoder/Test_ExthdrDstExistsAccept/exthdr_dst_exists_accept
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.0













