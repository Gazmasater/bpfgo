sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




func (sui *exthdrEncoderTestSuite) Test_ExthdrAliases() {
    testData := []struct {
        name     string
        op       uint8
        typ      uint8
        expected string
    }{
        // IPv6
        {"exthdr dst",     expr.ExthdrOpIpv6, unix.IPPROTO_DSTOPTS,  "exthdr dst exists accept"},
        {"exthdr hop",     expr.ExthdrOpIpv6, unix.IPPROTO_HOPOPTS,  "exthdr hop exists accept"},
        {"exthdr routing", expr.ExthdrOpIpv6, unix.IPPROTO_ROUTING,  "exthdr routing exists accept"},
        {"exthdr frag",    expr.ExthdrOpIpv6, unix.IPPROTO_FRAGMENT, "exthdr frag exists accept"},
        {"exthdr ah",      expr.ExthdrOpIpv6, unix.IPPROTO_AH,       "exthdr ah exists accept"},
        {"exthdr esp",     expr.ExthdrOpIpv6, unix.IPPROTO_ESP,      "exthdr esp exists accept"},

        // IPv4
        {"exthdr rr",      expr.ExthdrOpIpv4, 7,    "exthdr rr exists accept"},
        {"exthdr ts",      expr.ExthdrOpIpv4, 68,   "exthdr ts exists accept"},
        {"exthdr ts2",     expr.ExthdrOpIpv4, 148,  "exthdr ts exists accept"}, // Алиас тот же
        {"exthdr ssrr",    expr.ExthdrOpIpv4, 137,  "exthdr ssrr exists accept"},
        {"exthdr lsrr",    expr.ExthdrOpIpv4, 131,  "exthdr lsrr exists accept"},
    }

    for _, tc := range testData {
        sui.Run(tc.name, func() {
            exprs := nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Exthdr{
                        Op:     tc.op,
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
            sui.Require().Equal(tc.expected, str)
        })
    }
}













