sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept


sudo nft add rule ip test prerouting add @myset { 10.0.0.1 }

sudo nft add rule ip test prerouting add @myset { 192.168.1.10 timeout 10s }

sudo nft add rule ip test prerouting update @myset { testkey }




{
	name: "jhash with src and offset",
	hash: &expr.Hash{
		Type:           expr.HashTypeJenkins, // jhash (jenkins)
		Modulus:        123,
		Seed:           0xbeef,
		Offset:         42,
		SourceRegister: 7,
		DestRegister:   8,
	},
	srcReg:   7,
	srcHuman: "ip saddr",
	expected: "symhashjhash ip saddr mod 123 seed 0xbeef offset 42", // если твой encoder так строит строку
},


