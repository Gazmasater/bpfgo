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
	name:     "bitwise AND+XOR mask",
	srcReg:   3,
	dstReg:   4,
	mask:     []byte{0xff},
	xor:      []byte{0x0f},
	srcExpr:  nil,
	srcHuman: "meta mark",
	expected: "((meta mark) & 0xff) ^ 0xf",
},
{
	name:     "bitwise with OR (edge)",
	srcReg:   5,
	dstReg:   6,
	mask:     []byte{0xf0},
	xor:      []byte{0xf0},
	srcExpr:  nil,
	srcHuman: "payload 2",
	expected: "((payload 2) & 0xf0) ^ 0xf0",
},






