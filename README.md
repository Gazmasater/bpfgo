sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset





{
    name: "complex exthdr: reg, cmp, counter, log, accept",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Exthdr{
                Op:           expr.ExthdrOpIpv6,
                Type:         60,
                Offset:       0,
                Len:          0,
                Flags:        unix.NFT_EXTHDR_F_PRESENT,
                DestRegister: 1,
            },
            &expr.Cmp{
                Register: 1,
                Op:       expr.CmpOpEq,
                Data:     []byte{0x42},
            },
            &expr.Counter{},
            &expr.Log{Data: []byte("dstopt match\x00"), Key: 1},
            &expr.Verdict{Kind: expr.VerdictAccept},
        },
    },
    expected: `reset ip option 60 == 66 counter packets 0 bytes 0 log prefix "dstopt match" accept`, // "66" это 0x42
},



sudo nft add rule inet test prerouting ip6 nexthdr 60 counter log prefix "dstopt match" accept













