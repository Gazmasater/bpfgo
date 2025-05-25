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
	name: "jhash with src",
	hash: &expr.Hash{
		Type:           expr.HashTypeJenkins, // используем jhash (jenkins)
		Modulus:        5,
		Seed:           0x1111,
		Offset:         0,
		SourceRegister: 3,
		DestRegister:   4,
	},
	srcReg:   3,
	srcHuman: "meta mark",
	expected: "symhashjhash meta mark mod 5 seed 0x1111", // если твой encoder так формирует строку
},


