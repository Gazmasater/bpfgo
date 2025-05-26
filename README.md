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


  sudo nft add rule ip test prerouting ct count 5 accept


ls ~/.ssh/id_rsa.pub
ls ~/.ssh/*.pub
cat ~/.ssh/id_rsa.pub
ssh -T git@github.com
git remote -v
git remote set-url origin git@github.com:username/repo.git


ssh-keygen -t ed25519 -C "tolkoplus@gmail.com"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/5419_rsa


ssh-add ~/.ssh/id_ed25519
cat ~/.ssh/id_ed25519.pub











