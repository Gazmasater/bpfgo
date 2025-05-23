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
					&expr.Immediate{Register: 1, Data: []byte{10, 1, 2, 3}},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "dup to 10.1.2.3",
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


sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_DupEncoder (0.00s)
    --- FAIL: Test_DupEncoder/Test_DupExprToString (0.00s)
        --- FAIL: Test_DupEncoder/Test_DupExprToString/dup_to_address (0.00s)
            dup_test.go:37: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dup_test.go:37
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "dup to 10.1.2.3"
                                        actual  : "dup to 167838211"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -dup to 10.1.2.3
                                        +dup to 167838211
                        Test:           Test_DupEncoder/Test_DupExprToString/dup_to_address
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.007s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add table ip test
[sudo] password for gaz358: 
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip test prerouting dup to 10.1.2.3







