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
	CtStateBitINVALID     CtState = CtState(expr.CtStateBitINVALID)
	CtStateBitESTABLISHED CtState = CtState(expr.CtStateBitESTABLISHED)
	CtStateBitRELATED     CtState = CtState(expr.CtStateBitRELATED)
	CtStateBitNEW         CtState = CtState(expr.CtStateBitNEW)
	CtStateBitUNTRACKED   CtState = CtState(expr.CtStateBitUNTRACKED)
)

Data: []byte{byte(CtStateBitNEW), 0, 0, 0, 0, 0, 0, 0},



