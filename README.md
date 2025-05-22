Да, это отличный пример теста, в котором заранее задано ожидаемое строковое представление для правила nftables!

Как это устроено:
Ты формируешь Go-структуру (nftables.Rule с набором expr.Any — выражений).

Далее для каждого выражения описываешь, что внутри: например, &expr.Meta, &expr.Cmp, &expr.Counter, &expr.Log, &expr.Verdict и их параметры.

В поле expected — строка, которую реально выдает nft CLI для этого правила:

perl

meta l4proto tcp counter packets 0 bytes 0 log accept
В самом тесте сравниваешь результат своей функции (которая преобразует Rule к строке) с этим эталоном.

Как реализовать такой тест (шаблон)

func TestRuleString(t *testing.T) {
    tests := []struct {
        name     string
        exprs    *nftables.Rule
        expected string
    }{
        {
            name: "Expression 1",
            exprs: &nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Meta{Key: expr.MetaKeyL4PROTO, Register: 1},
                    &expr.Cmp{
                        Op:      expr.CmpOpEq,
                        Register: 1,
                        Data:    []byte{unix.IPPROTO_TCP},
                    },
                    &expr.Counter{},
                    &expr.Log{},
                    &expr.Verdict{Kind: expr.VerdictAccept},
                },
            },
            expected: "meta l4proto tcp counter packets 0 bytes 0 log accept",
        },
    }

    for _, tt := range tests {
        t.Run(tt.name, func(t *testing.T) {
            got := RuleToString(tt.exprs) // твоя функция преобразования rule в строку
            if got != tt.expected {
                t.Errorf("expected %q, got %q", tt.expected, got)
            }
        })
    }
}


sudo nft add table inet filter
sudo nft add chain inet filter input { type filter hook input priority 0 \; policy accept \; }
sudo nft add rule inet filter input meta l4proto tcp counter log accept

Сделай вывод правил:


nft list table inet filter
Скопируй строку из вывода, например:

perl

meta l4proto tcp counter packets 0 bytes 0 log accept
Поставь её в поле expected в тесте.

Как выглядит рабочий пайплайн теста
Описываешь exprs как структуру (Go-код).

Указываешь эталонную строку (поле expected) — копию из вывода nft CLI.

Сравниваешь результат работы своего рендера с этой строкой.

Если не совпало — либо баг в рендере, либо правило не эквивалентно.

Такой подход и есть best practice для unit-test'ов генераторов правил nftables!

Если нужна функция RuleToString, которая превращает Go-структуру в строку, похожую на nft CLI, тоже могу показать пример реализации.
Если нужно сделать автотест для блока с hash (как обсуждали выше), аналогично делай:

Сначала руками через nft CLI,

копируй вывод,

пиши unit-тест по этому шаблону.

func TestRuleString(t *testing.T) {
    tests := []struct {
        name     string
        exprs    *nftables.Rule
        expected string
    }{
        {
            name: "Expression 1",
            exprs: &nftables.Rule{
                Exprs: []expr.Any{
                    &expr.Meta{Key: expr.MetaKeyL4PROTO, Register: 1},
                    &expr.Cmp{
                        Op:      expr.CmpOpEq,
                        Register: 1,
                        Data:    []byte{unix.IPPROTO_TCP},
                    },
                    &expr.Counter{},
                    &expr.Log{},
                    &expr.Verdict{Kind: expr.VerdictAccept},
                },
            },
            expected: "meta l4proto tcp counter packets 0 bytes 0 log accept",
        },
    }

    for _, tt := range tests {
        t.Run(tt.name, func(t *testing.T) {
            got := RuleToString(tt.exprs) // твоя функция преобразования rule в строку
            if got != tt.expected {
                t.Errorf("expected %q, got %q", tt.expected, got)
            }
        })
    }
}



gaz358@gaz358-BOD-WXX9:~/myprog/nft-go$ sudo nft list table inet filter
table inet filter {
        chain input {
                type filter hook input priority filter; policy accept;
                meta l4proto tcp counter packets 22 bytes 1412 log accept
        }
}
gaz358@gaz358-BOD-




