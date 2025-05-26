sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




const (
    IPv6NextHeaderOffset = 6
)


import (
    "github.com/google/nftables"
    "github.com/google/nftables/expr"
    "golang.org/x/sys/unix"
)

func (sui *encodersTestSuite) Test_Ip6NexthdrIpv6Opts() {
    exprs := nftables.Rule{
        Exprs: []expr.Any{
            &expr.Payload{
                DestRegister: 1,
                Base:         expr.PayloadBaseNetworkHeader, // network header (IPv6)
                Offset:       6, // nexthdr is at offset 6 in IPv6 header
                Len:          1,
            },
            &expr.Cmp{
                Register: 1,
                Op:       expr.CmpOpEq,
                Data:     []byte{unix.IPPROTO_DSTOPTS}, // use the proper constant for ipv6-opts
            },
            &expr.Verdict{Kind: expr.VerdictAccept},
        },
    }
    expected := "ip6 nexthdr ipv6-opts accept"
    str, err := NewRuleExprEncoder(&exprs).Format()
    sui.Require().NoError(err)
    sui.Require().Equal(expected, str)
}



















