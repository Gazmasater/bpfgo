sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




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



{
    name: "ip daddr 192.168.1.0/24 accept via bitwise",
    exprs: nftables.Rule{
        Table: &nftables.Table{Name: tableName},
        Exprs: []expr.Any{
            &expr.Payload{
                DestRegister: 1,
                Base:         expr.PayloadBaseNetworkHeader,
                Offset:       16,
                Len:          4,
            },
            &expr.Bitwise{
                SourceRegister: 1,
                DestRegister:   1,
                Len:            4,
                Mask:           []byte{255, 255, 255, 0},
                Xor:            []byte{0, 0, 0, 0},
            },
            &expr.Cmp{
                Register: 1,
                Op:       expr.CmpOpEq,
                Data:     []byte{192, 168, 1, 0},
            },
            &expr.Verdict{
                Kind: expr.VerdictAccept,
            },
        },
    },
    expected: "ip daddr 192.168.1.0/24 accept",
},








