{
    name: "Only Counter",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Counter{},
        },
    },
    expected: "counter packets 0 bytes 0",
},
{
    name: "Only Log",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Log{},
        },
    },
    expected: "log",
},
{
    name: "Only Limit",
    exprs: nftables.Rule{
        Exprs: []expr.Any{
            &expr.Limit{Rate: 1, Unit: expr.LimitUnitSecond},
        },
    },
    expected: "limit rate 1/second",
},







