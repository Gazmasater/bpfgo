sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

{
	name:      "simple count",
	connlimit: &expr.Connlimit{Count: 7, Flags: 0},
	expected: map[string]interface{}{
		"ct count": map[string]interface{}{
			"val": float64(7),
			// "inv": false, // этого не должно быть!
		},
	},
},







