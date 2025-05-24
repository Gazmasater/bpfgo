sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

sudo nft add rule ip test prerouting ct state established,related accept
sudo nft add rule ip test prerouting ct direction original accept
sudo nft add rule ip test prerouting ct protocol tcp accept
sudo nft add rule ip test prerouting ct mark 1 accept




gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git branch -M main
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git remote add origin git@github.com:Gazmasater/nft.git
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ git push -u origin main
error: src refspec main does not match any
error: failed to push some refs to 'github.com:Gazmasater/nft.git'





