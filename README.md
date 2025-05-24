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

[{
	"resource": "/home/gaz358/myprog/nft-go/internal/expr-encoders/ct_test.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "IncompatibleAssign",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "IncompatibleAssign"
		}
	},
	"severity": 8,
	"message": "cannot use CtStateBitNEW (constant 8 of uint32 type CtState) as byte value in array or slice literal",
	"source": "compiler",
	"startLineNumber": 35,
	"startColumn": 24,
	"endLineNumber": 35,
	"endColumn": 37
}]


