testData := []struct {
    name     string
    exprs    nftables.Rule
    preRun   func()
    expected string
}{
    {
        name: "Only Accept",
        exprs: nftables.Rule{
            Exprs: []expr.Any{
                &expr.Verdict{Kind: expr.VerdictAccept},
            },
        },
        expected: "accept",
    },
    // ... остальные кейсы ...
}





