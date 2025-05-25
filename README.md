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



az358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git add .
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git commit -m "my first commit"
[main (root-commit) 4ee132d] my first commit
 42 files changed, 4467 insertions(+)
 create mode 100644 bitwise.go
 create mode 100644 byteorder.go
 create mode 100644 cmp.go
 create mode 100644 connlimit.go
 create mode 100644 counter.go
 create mode 100644 ct.go
 create mode 100644 ct_test.go
 create mode 100644 dup.go
 create mode 100644 dup_test
 create mode 100644 dynset.go
 create mode 100644 dynset_test.go
 create mode 100644 encoders.go
 create mode 100644 encoders_test.go
 create mode 100644 exthdr.go
 create mode 100644 fib.go
 create mode 100644 flow_offload.go
 create mode 100644 hash.go
 create mode 100644 immediate.go
 create mode 100644 ir.go
 create mode 100644 limit.go
 create mode 100644 log.go
 create mode 100644 lookup.go
 create mode 100644 masq.go
 create mode 100644 match.go
 create mode 100644 meta.go
 create mode 100644 nat.go
 create mode 100644 notrack.go
 create mode 100644 numgen.go
 create mode 100644 objref.go
 create mode 100644 payload.go
 create mode 100644 queue.go
 create mode 100644 quota.go
 create mode 100644 range.go
 create mode 100644 redirect.go
 create mode 100644 registry.go
 create mode 100644 reject.go
 create mode 100644 rt.go
 create mode 100644 set.go
 create mode 100644 socket.go
 create mode 100644 target.go
 create mode 100644 tproxy.go
 create mode 100644 verdict.go
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git push -u origin main
git@github.com: Permission denied (publickey).
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git branch -M main
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git push -u origin main
git@github.com: Permission denied (publickey).
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.





