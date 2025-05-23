sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test


{
	name: "oifname ip daddr counter log",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Meta{Key: expr.MetaKeyOIFNAME, Register: 1},
			&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte("eth0")},
			&expr.Payload{DestRegister: 1, Base: 0, Offset: 16, Len: 4},
			&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{192, 168, 1, 10}},
			&expr.Counter{},
			&expr.Log{},
		},
	},
	expected: "meta oifname eth0 ip daddr 192.168.1.10 counter packets 0 bytes 0 log",
}


sudo nft add rule ip test prerouting oifname "eth0" ip daddr 192.168.1.10 counter log












