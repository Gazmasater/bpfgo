sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




func (sui *exthdrEncoderTestSuite) Test_Ip6Nexthdr60_LogCounterAccept() {
    testData := []struct {
        name     string
        exprs    nftables.Rule
        expected string
    }{
        {
            name: "ip6 nexthdr 60 log counter accept",
            exprs: nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Meta{Key: expr.MetaKeyL4PROTO, Register: 1}, // l4proto = nexthdr
                    &expr.Cmp{
                        Register: 1,
                        Op:       expr.CmpOpEq,
                        Data:     []byte{60}, // 60 - Destination Options Header
                    },
                    &expr.Log{Data: []byte("dstopt match\x00"), Key: 1}, // log prefix
                    &expr.Counter{},
                    &expr.Verdict{Kind: expr.VerdictAccept},
                },
            },
            expected: `meta l4proto 60 log prefix "dstopt match" counter packets 0 bytes 0 accept`,
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




sudo nft add rule inet test prerouting ip6 nexthdr 60 log prefix "dstopt match" counter accept

gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule inet test prerouting ip6 nexthdr 60 log prefix "dstopt match" counter accept
Error: syntax error, unexpected counter
add rule inet test prerouting ip6 nexthdr 60 log prefix dstopt match counter accept
                                                                     ^^^^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 

















