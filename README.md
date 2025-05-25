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



package encoders

import (
	"encoding/json"
	"testing"

	"github.com/google/nftables/expr"
)

// Заглушки ошибок, если не экспортируются из основного кода
var (
	ErrNoIR   = errors.New("no IR")
	ErrNoJSON = errors.New("no JSON")
)

func TestHashEncoder_EncodeIR_Symhash(t *testing.T) {
	ctx := &ctx{}
	hash := &expr.Hash{
		Type:         expr.HashTypeSym,
		Modulus:      5,
		Seed:         42,
		Offset:       0,
		DestRegister: 3,
	}
	enc := &hashEncoder{hash: hash}

	ir, err := enc.EncodeIR(ctx)
	if err != ErrNoIR {
		t.Fatalf("expected ErrNoIR, got: %v", err)
	}
	if ir != nil {
		t.Error("expected IR to be nil for symhash")
	}
	val, ok := ctx.reg.Get(regID(3))
	if !ok {
		t.Fatal("expected value in dest register")
	}
	want := "symhash mod 5 seed 0x2a"
	if val.HumanExpr != want {
		t.Errorf("expected HumanExpr %q, got %q", want, val.HumanExpr)
	}
}

func TestHashEncoder_EncodeIR_Jhash_WithSrcReg(t *testing.T) {
	ctx := &ctx{}
	ctx.reg.Set(2, regVal{HumanExpr: "foobar"})
	hash := &expr.Hash{
		Type:           expr.HashTypeJHash,
		Modulus:        100,
		Seed:           0xdead,
		Offset:         7,
		SourceRegister: 2,
		DestRegister:   4,
	}
	enc := &hashEncoder{hash: hash}

	ir, err := enc.EncodeIR(ctx)
	if err != ErrNoIR {
		t.Fatalf("expected ErrNoIR, got: %v", err)
	}
	val, ok := ctx.reg.Get(regID(4))
	if !ok {
		t.Fatal("expected value in dest register")
	}
	want := "symhashjhash foobar mod 100 seed 0xdead offset 7"
	if val.HumanExpr != want {
		t.Errorf("expected HumanExpr %q, got %q", want, val.HumanExpr)
	}
}

func TestHashEncoder_EncodeIR_MissingSrcReg(t *testing.T) {
	ctx := &ctx{}
	hash := &expr.Hash{
		Type:           expr.HashTypeJHash,
		SourceRegister: 7,
		DestRegister:   1,
	}
	enc := &hashEncoder{hash: hash}
	_, err := enc.EncodeIR(ctx)
	if err == nil {
		t.Fatal("expected error due to missing source register, got nil")
	}
}

func TestHashEncoder_EncodeIR_InvalidDestReg(t *testing.T) {
	ctx := &ctx{}
	hash := &expr.Hash{
		Type:    expr.HashTypeSym,
		Modulus: 2,
		Seed:    2,
	}
	enc := &hashEncoder{hash: hash}
	_, err := enc.EncodeIR(ctx)
	if err == nil {
		t.Fatal("expected error due to invalid dest register, got nil")
	}
}

func TestHashEncoder_EncodeJSON_Symhash(t *testing.T) {
	ctx := &ctx{}
	hash := &expr.Hash{
		Type:         expr.HashTypeSym,
		Modulus:      15,
		Seed:         9,
		Offset:       0,
		DestRegister: 8,
	}
	enc := &hashEncoder{hash: hash}
	_, err := enc.EncodeJSON(ctx)
	if err != ErrNoJSON {
		t.Fatalf("expected ErrNoJSON, got: %v", err)
	}
	val, ok := ctx.reg.Get(regID(8))
	if !ok {
		t.Fatal("expected value in dest register")
	}
	js, err := json.Marshal(val.Data)
	if err != nil {
		t.Errorf("could not marshal json: %v", err)
	}
	if !json.Valid(js) {
		t.Errorf("invalid JSON produced: %s", js)
	}
}

func TestHashEncoder_EncodeJSON_Jhash_WithSrcReg(t *testing.T) {
	ctx := &ctx{}
	ctx.reg.Set(1, regVal{Data: "DATA!"})
	hash := &expr.Hash{
		Type:           expr.HashTypeJHash,
		Modulus:        8,
		Seed:           7,
		Offset:         1,
		SourceRegister: 1,
		DestRegister:   6,
	}
	enc := &hashEncoder{hash: hash}
	_, err := enc.EncodeJSON(ctx)
	if err != ErrNoJSON {
		t.Fatalf("expected ErrNoJSON, got: %v", err)
	}
	val, ok := ctx.reg.Get(regID(6))
	if !ok {
		t.Fatal("expected value in dest register")
	}
	js, err := json.Marshal(val.Data)
	if err != nil {
		t.Errorf("could not marshal json: %v", err)
	}
	if !json.Valid(js) {
		t.Errorf("invalid JSON produced: %s", js)
	}
}

func TestHashEncoder_EncodeJSON_InvalidDestReg(t *testing.T) {
	ctx := &ctx{}
	hash := &expr.Hash{
		Type:    expr.HashTypeSym,
		Modulus: 1,
		Seed:    1,
	}
	enc := &hashEncoder{hash: hash}
	_, err := enc.EncodeJSON(ctx)
	if err == nil {
		t.Fatal("expected error due to invalid dest register, got nil")
	}
}









