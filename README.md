sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test



{
	name: "dup to address",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Immediate{Register: 1, Data: []byte("10.1.2.3")},
			&expr.Dup{RegAddr: 1},
		},
	},
	expected: "dup to 10.1.2.3",
},

sudo nft add rule ip test prerouting dup to 10.1.2.3


{
	name: "dup to address and device",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
			&expr.Immediate{Register: 2, Data: []byte("lo")},
			&expr.Dup{RegAddr: 1, RegDev: 2},
		},
	},
	expected: "dup to 192.168.1.10 device lo",
},


sudo nft add rule ip test prerouting dup to 192.168.1.10 device lo








