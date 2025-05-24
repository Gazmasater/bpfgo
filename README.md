sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept




func (c CtState) String() string {
	var st []string

	if c&CtStateBitNEW != 0 {
		st = append(st, "new")
	}
	if c&CtStateBitESTABLISHED != 0 {
		st = append(st, "established")
	}
	if c&CtStateBitRELATED != 0 {
		st = append(st, "related")
	}
	if c&CtStateBitINVALID != 0 {
		st = append(st, "invalid")
	}
	if c&CtStateBitUNTRACKED != 0 {
		st = append(st, "untracked")
	}

	return strings.Join(st, ",")
}



