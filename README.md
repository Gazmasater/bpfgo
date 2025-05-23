sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test






package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dupEncoderTestSuite struct {
	suite.Suite
}

func (sui *dupEncoderTestSuite) Test_DupExprToString() {
	testData := []struct {
		name     string
		exprs    nftables.Rule
		expected string
	}{
		{
			name: "dup to address",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("10.1.2.3")},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "dup to 10.1.2.3",
		},
		{
			name: "dup to address and device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
					&expr.Immediate{Register: 2, Data: []byte("eth0")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expected: "dup to 192.168.1.10 device eth0",
		},
		{
			name: "dup only device",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Immediate{Register: 2, Data: []byte("br-lan")},
					&expr.Dup{RegDev: 2},
				},
			},
			expected: "dup",
		},
		{
			name: "dup no params",
			exprs: nftables.Rule{
				Exprs: []expr.Any{
					&expr.Dup{},
				},
			},
			expected: "dup",
		},
	}

	for _, t := range testData {
		sui.Run(t.name, func() {
			str, err := NewRuleExprEncoder(&t.exprs).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(t.expected, str)
		})
	}
}

func Test_DupEncoder(t *testing.T) {
	suite.Run(t, new(dupEncoderTestSuite))
}



1. dup to address
Go-тест:

go
Копировать
Редактировать
&expr.Immediate{Register: 1, Data: []byte("10.1.2.3")},
&expr.Dup{RegAddr: 1},
nftables:

nft
Копировать
Редактировать
dup to 10.1.2.3
2. dup to address and device
Go-тест:

go
Копировать
Редактировать
&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
&expr.Immediate{Register: 2, Data: []byte("eth0")},
&expr.Dup{RegAddr: 1, RegDev: 2},
nftables:

nft
Копировать
Редактировать
dup to 192.168.1.10 device eth0
3. dup only device
Go-тест:

go
Копировать
Редактировать
&expr.Immediate{Register: 2, Data: []byte("br-lan")},
&expr.Dup{RegDev: 2},
nftables:

nft
Копировать
Редактировать
dup
Примечание:
В синтаксисе nftables команда просто dup без адреса не имеет смысла.
Обычно указывается хотя бы to, но в некоторых версиях может быть разрешено правило dup для интерфейса по умолчанию (редко используется, и часто приводит к ошибке при применении через nft).
В реальных правилах стоит явно указывать либо to <ip>, либо интерфейс, либо и то, и другое.

4. dup no params
Go-тест:

go
Копировать
Редактировать
&expr.Dup{},
nftables:

nft
Копировать
Редактировать
dup
Примечание:
Как и выше, это не имеет смысла для реального nftables, но для целей сериализации/теста — OK.

Пример полной таблицы и цепочки
nft
Копировать
Редактировать
table ip test {
    chain prerouting {
        type nat hook prerouting priority 0; policy accept;
        dup to 10.1.2.3
        dup to 192.168.1.10 device eth0
        dup
        dup
    }
}
Внимание: последние два правила, скорее всего, вызовут ошибку при загрузке в реальную систему через nft, потому что ожидается хотя бы адрес.

Если в будущем тебе нужны примеры с исходным интерфейсом или сложными случаями (dup to ... + разные опции), напиши — покажу нужный синтаксис.
Если тестируешь не только сериализацию, но и реальные правила в nft — всегда указывай IP или интерфейс явно!

















