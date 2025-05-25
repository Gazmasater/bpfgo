sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept

const (
	IPPROTO_ICMP = 1
	IPPROTO_TCP  = 6
	IPPROTO_UDP  = 17
	IPPROTO_GRE  = 47
	IPPROTO_ESP  = 50
	IPPROTO_AH   = 51
	IPPROTO_SCTP = 132
)



{
	name: "ct protocol tcp",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyPROTOCOL,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{IPPROTO_TCP},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct protocol tcp accept",
},
{
	name: "ct protocol udp",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyPROTOCOL,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{IPPROTO_UDP},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct protocol udp accept",
},
{
	name: "ct protocol icmp",
	exprs: nftables.Rule{
		Exprs: []expr.Any{
			&expr.Ct{
				Key:      expr.CtKeyPROTOCOL,
				Register: 1,
			},
			&expr.Cmp{
				Register: 1,
				Op:       expr.CmpOpEq,
				Data:     []byte{IPPROTO_ICMP},
			},
			&expr.Verdict{Kind: expr.VerdictAccept},
		},
	},
	expected: "ct protocol icmp accept",
},



