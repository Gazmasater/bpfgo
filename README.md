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



func (b *dynsetEncoder) EncodeIR(ctx *ctx) (irNode, error) {
    dyn := b.dynset
    sb := strings.Builder{}
    if ctx.rule == nil {
        return nil, errors.New("ctx has no rule")
    }
    srcRegKey, ok := ctx.reg.Get(regID(dyn.SrcRegKey))
    if !ok {
        return nil, errors.Errorf("%T statement has no key expression", dyn)
    }
    exp := srcRegKey.HumanExpr

    // Сначала только exp и timeout
    sb.WriteString(exp)
    if dyn.Timeout != 0 {
        sb.WriteString(fmt.Sprintf(" timeout %s", dyn.Timeout))
    }

    if sb.Len() > 0 {
        exp = sb.String()
    }
    sb.Reset()
    setName := fmt.Sprintf(`@%s`, dyn.SetName)

    // Теперь готовим строку expr-выражений
    rule := *ctx.rule
    rule.Exprs = dyn.Exprs
    str, err := NewRuleExprEncoder(&rule).Format()
    if err != nil {
        return nil, err
    }

    sb.WriteString(fmt.Sprintf("%s %s { %s", DynSetOP(dyn.Operation), setName, exp))
    if str != "" {
        sb.WriteString(" ")
        sb.WriteString(str)
    }

    srcRegData, ok := ctx.reg.Get(regID(dyn.SrcRegData))
    if ok {
        if exprData := srcRegData.HumanExpr; exprData != "" {
            sb.WriteString(fmt.Sprintf(" : %s", exprData))
        }
    }

    sb.WriteString(" }")

    return simpleIR(sb.String()), nil
}





