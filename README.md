{
    name: "Empty Cmp",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Cmp{},
        },
    },
    expected: "cmp reg0 == 0x0", // или что-то похожее по encoder'у
},






