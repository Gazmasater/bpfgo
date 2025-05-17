package firewall

import (
	"github.com/google/nftables/expr"
	"golang.org/x/sys/unix"
)

// BuildExprsL4protoTCP возвращает выражения для правила:
// meta l4proto tcp counter log accept
func BuildExprsL4protoTCP() []expr.Any {
	return []expr.Any{
		&expr.Meta{
			Key:      expr.MetaKeyL4PROTO,
			Register: 1,
		},
		&expr.Cmp{
			Op:       expr.CmpOpEq,
			Register: 1,
			Data:     []byte{unix.IPPROTO_TCP},
		},
		&expr.Counter{},
		&expr.Log{},
		&expr.Verdict{
			Kind: expr.VerdictAccept,
		},
	}
}


package firewall

import (
	"bytes"
	"os/exec"
	"testing"

	"github.com/google/nftables"
)

func TestBuildExprsL4protoTCP(t *testing.T) {
	c := &nftables.Conn{}
	table := &nftables.Table{Name: "testtbl", Family: nftables.TableFamilyINet}
	c.AddTable(table)

	chain := &nftables.Chain{
		Name:     "testchain",
		Table:    table,
		Type:     nftables.ChainTypeFilter,
		Hooknum:  nftables.ChainHookInput,
		Priority: nftables.ChainPriorityFilter,
	}
	c.AddChain(chain)

	rule := &nftables.Rule{
		Table: table,
		Chain: chain,
		Exprs: BuildExprsL4protoTCP(),
	}
	c.AddRule(rule)
	if err := c.Flush(); err != nil {
		t.Fatalf("flush failed: %v", err)
	}

	out, err := exec.Command("nft", "-nn", "list", "ruleset").CombinedOutput()
	if err != nil {
		t.Fatalf("nft list ruleset failed: %v\n%s", err, out)
	}

	expected := []byte("meta l4proto tcp counter packets 0 bytes 0 log accept")
	if !bytes.Contains(out, expected) {
		t.Errorf("expected rule not found in output\nExpected: %q\nGot:\n%s", expected, out)
	}
}
