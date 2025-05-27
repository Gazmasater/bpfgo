sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept





sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



package encoders

import (
	"fmt"
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type exthdrEncoderTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderTestSuite) Test_ExthdrExistsAccept_WithAliases() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "exthdr dst exists accept (alias)",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:     expr.ExthdrOpIpv6,        // IPv6 extension header
						Type:   unix.IPPROTO_DSTOPTS,     // 60 = dst
						Offset: 0,
						Len:    0,
						Flags:  unix.NFT_EXTHDR_F_PRESENT, // "exists"
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "exthdr dst exists accept",
		},
		{
			name: "exthdr frag exists accept (alias)",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:     expr.ExthdrOpIpv6,        // IPv6 extension header
						Type:   44,                       // 44 = frag
						Offset: 0,
						Len:    0,
						Flags:  unix.NFT_EXTHDR_F_PRESENT, // "exists"
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "exthdr frag exists accept",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			str, err := NewRuleExprEncoder(&tc.exprs).Format()
			sui.Require().NoError(err)
			fmt.Printf("Expected=%s\n", tc.expected)
			fmt.Printf("IR=%s\n", str)
			sui.Require().Equal(tc.expected, str)
		})
	}
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}


unix.IPPROTO_FRAGMENT


