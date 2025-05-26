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


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ ssh-keygen -t ed25519 -C "tolkoplus@gmail.com"
Generating public/private ed25519 key pair.
Enter file in which to save the key (/home/gaz358/.ssh/id_ed25519): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/gaz358/.ssh/id_ed25519
Your public key has been saved in /home/gaz358/.ssh/id_ed25519.pub
The key fingerprint is:
SHA256:Mx2OoIcFApstGAPgFLTtWukH8nYHFfm3Ao3qia/C0pk tolkoplus@gmail.com
The key's randomart image is:
+--[ED25519 256]--+
|X=o .  ..        |
|+*o. . ..        |
|=o..  o.+ .      |
| .. .+.+ * o     |
| . =o.o S + .    |
|  * .o.  + .     |
|.o *oo..  .      |
|.oE.oo.          |
|. ..o.           |
+----[SHA256]-----+
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ eval "$(ssh-agent -s)"
Agent pid 5419










