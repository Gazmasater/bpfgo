sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


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

func (sui *exthdrEncoderTestSuite) Test_ExthdrDstExistsAccept_WithAlias() {
	exprs := nftables.Rule{
		Exprs: []expr.Any{
			&expr.Exthdr{
				Op:     expr.ExthdrOpIpv6,        // IPv6 extension header
				Type:   unix.IPPROTO_DSTOPTS,     // 60 (Destination Options Header, = dst)
				Offset: 0,
				Len:    0,
				Flags:  unix.NFT_EXTHDR_F_PRESENT, // "exists"
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	}
	expected := "exthdr dst exists accept" // Алиас, как в nft list ruleset!
	str, err := NewRuleExprEncoder(&exprs).Format()
	sui.Require().NoError(err)
	sui.Require().Equal(expected, str)
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}











