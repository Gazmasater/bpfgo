sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept



{
	name: "ct state new",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATE,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data: []byte{0x01, 0, 0, 0, 0, 0, 0, 0}, // только new
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state new accept",
},
{
	name: "ct state established",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATE,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data: []byte{0x02, 0, 0, 0, 0, 0, 0, 0}, // только established
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state established accept",
},
{
	name: "ct state related",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATE,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data: []byte{0x04, 0, 0, 0, 0, 0, 0, 0}, // только related
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state related accept",
},
{
	name: "ct state invalid",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATE,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data: []byte{0x08, 0, 0, 0, 0, 0, 0, 0}, // только invalid
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state invalid accept",
},











