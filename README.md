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
    name: "add with log and counter",
    dynset: &expr.Dynset{
        Operation: uint32(DynSetOPAdd),
        SetName:   "myset",
        SrcRegKey: 8,
        Exprs: []expr.Any{
            &expr.Log{},
            &expr.Counter{},
        },
    },
    srcKey:   "8.8.8.8",
    expected: "add @myset { 8.8.8.8 log counter packets 0 bytes 0 }",
},





