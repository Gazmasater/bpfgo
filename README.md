sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log




{
			name:      "simple count",
			connlimit: &expr.Connlimit{Count: 5, Flags: 0},
			expected:  "ct count 5",
		},
		{
			name:      "over count (NFT_LIMIT_F_INV)",
			connlimit: &expr.Connlimit{Count: 10, Flags: unix.NFT_LIMIT_F_INV},
			expected:  "ct count over 10",
		},


  nft add rule ip test prerouting ct count 5 accept










