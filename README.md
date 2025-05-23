nft add table inet filter
nft add chain inet filter input { type filter hook input priority 0\; policy accept\; }
nft add rule inet filter input counter
nft list table inet filter


{
    name: "Only Counter",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Counter{},
        },
    },
    expected: "counter packets 0 bytes 0",
},






