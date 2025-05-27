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



func (sui *exthdrEncoderTestSuite) Test_ExthdrDstAndFragExistsAccept() {
    testData := []struct {
        name     string
        exprs    nftables.Rule
        expected string
    }{
        {
            name: "exthdr dst exists exthdr frag exists accept",
            exprs: nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Exthdr{
                        Type:  60,                        // dst
                        Flags: unix.NFT_EXTHDR_F_PRESENT, // exists
                    },
                    &expr.Exthdr{
                        Type:  44,                        // frag
                        Flags: unix.NFT_EXTHDR_F_PRESENT, // exists
                    },
                    &expr.Verdict{
                        Kind: expr.VerdictAccept,
                    },
                },
            },
            expected: "exthdr dst exists exthdr frag exists accept",
        },
    }

    for _, tc := range testData {
        sui.Run(tc.name, func() {
            ir, err := NewRuleExprEncoder(&tc.exprs).Format()
            sui.Require().NoError(err)
            sui.Require().Equal(tc.expected, ir)
        })
    }
}


sudo nft add rule ip6 test prerouting exthdr dst exists exthdr frag exists accept






