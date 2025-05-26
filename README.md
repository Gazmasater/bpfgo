sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




        {
            name: "ip6 nexthdr 60 counter log accept",
            exprs: nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Meta{Key: expr.MetaKeyL4PROTO, Register: 1},
                    &expr.Cmp{
                        Register: 1,
                        Op:       expr.CmpOpEq,
                        Data:     []byte{60}, // 60 — Destination Options Header
                    },
                    &expr.Counter{},
                    &expr.Log{Data: []byte("dstopt match\x00"), Key: 1}, // log с префиксом
                    &expr.Verdict{Kind: expr.VerdictAccept},
                },
            },
            expected: `meta l4proto 60 counter packets 0 bytes 0 log prefix "dstopt match" accept`,
        },




sudo nft add rule inet test prerouting ip6 nexthdr 60 counter log prefix "dstopt match" accept


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule inet test prerouting ip6 nexthdr 60 counter log prefix "dstopt match" accept
Error: syntax error, unexpected accept
add rule inet test prerouting ip6 nexthdr 60 counter log prefix dstopt match accept
                                                                             ^^^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 














