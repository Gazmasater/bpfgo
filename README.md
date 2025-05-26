sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




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

func (sui *exthdrEncoderTestSuite) Test_ExthdrOnlyNumbers() {
	testData := []struct {
		name     string
		op       uint8
		typ      uint8
		expected string
	}{
		// IPv6 exthdr
		{"exthdr 0 (IPv6 Hop-by-Hop)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_HOPOPTS, "ip option 0 accept"},
		{"exthdr 43 (IPv6 Routing)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_ROUTING, "ip option 43 accept"},
		{"exthdr 44 (IPv6 Fragment)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_FRAGMENT, "ip option 44 accept"},
		{"exthdr 50 (IPv6 ESP)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_ESP, "ip option 50 accept"},
		{"exthdr 51 (IPv6 AH)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_AH, "ip option 51 accept"},
		{"exthdr 60 (IPv6 Destination Opts)", uint8(expr.ExthdrOpIpv6), unix.IPPROTO_DSTOPTS, "ip option 60 accept"},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			exprs := nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:     expr.ExthdrOp(tc.op),
						Type:   tc.typ,
						Offset: 0,
						Len:    0,
						Flags:  unix.NFT_EXTHDR_F_PRESENT,
					},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			}
			str, err := NewRuleExprEncoder(&exprs).Format()
			sui.Require().NoError(err)
			fmt.Printf("Expected=%s\n", tc.expected)
			sui.Require().Equal(tc.expected, str)
		})
	}
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}









