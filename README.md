sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept

sudo nft list ruleset





func (sui *exthdrEncoderTestSuite) Test_ExthdrComplexCases() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "exthdr with offset/len",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:     expr.ExthdrOpIpv6,
						Type:   43, // Routing Header
						Offset: 4,
						Len:    2,
						Flags:  0,
					},
					&expr.Verdict{Kind: expr.VerdictDrop},
				},
			},
			expected: "ip option @43,4,2 drop",
		},
		{
			name: "exthdr dst exists, with register set",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Exthdr{
						Op:           expr.ExthdrOpIpv6,
						Type:         60,
						Offset:       0,
						Len:          0,
						Flags:        unix.NFT_EXTHDR_F_PRESENT,
						DestRegister: 2,
					},
					&expr.Immediate{Register: 2, Data: []byte{0x42}},
					&expr.Verdict{Kind: expr.VerdictAccept},
				},
			},
			expected: "reset ip option 60 0x42 accept",
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














