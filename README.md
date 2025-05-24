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
	name: "ct state multi",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeySTATE,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				// Битовая маска: new = 0x01, established = 0x02, related = 0x04 → 0x07
				// Все остальные байты нули, т.к. state — это uint8, но поле uint64 (8 байт)
				Data: []byte{0x07, 0, 0, 0, 0, 0, 0, 0},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct state new,established,related accept",
},










