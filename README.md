sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log

package encoders

import (
	"encoding/json"
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type connlimitEncoderTestSuite struct {
	suite.Suite
}

func (sui *connlimitEncoderTestSuite) Test_ConnlimitEncodeIR() {
	testData := []struct {
		name      string
		connlimit *expr.Connlimit
		expected  string
	}{
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
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			enc := &connlimitEncoder{connlimit: tc.connlimit}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func (sui *connlimitEncoderTestSuite) Test_ConnlimitEncodeJSON() {
	testData := []struct {
		name      string
		connlimit *expr.Connlimit
		expected  map[string]interface{}
	}{
		{
			name:      "simple count",
			connlimit: &expr.Connlimit{Count: 7, Flags: 0},
			expected: map[string]interface{}{
				"ct count": map[string]interface{}{
					"val": float64(7), // json.Unmarshal превращает в float64
					"inv": false,
				},
			},
		},
		{
			name:      "over count (NFT_LIMIT_F_INV)",
			connlimit: &expr.Connlimit{Count: 20, Flags: unix.NFT_LIMIT_F_INV},
			expected: map[string]interface{}{
				"ct count": map[string]interface{}{
					"val": float64(20),
					"inv": true,
				},
			},
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			enc := &connlimitEncoder{connlimit: tc.connlimit}
			data, err := enc.EncodeJSON(ctx)
			sui.Require().NoError(err)

			var got map[string]interface{}
			sui.Require().NoError(json.Unmarshal(data, &got))
			sui.Require().Equal(tc.expected, got)
		})
	}
}

func Test_ConnlimitEncoder(t *testing.T) {
	suite.Run(t, new(connlimitEncoderTestSuite))
}


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go  test
--- FAIL: Test_ConnlimitEncoder (0.00s)
    --- FAIL: Test_ConnlimitEncoder/Test_ConnlimitEncodeJSON (0.00s)
        --- FAIL: Test_ConnlimitEncoder/Test_ConnlimitEncodeJSON/simple_count (0.00s)
            connlimit_test.go:82: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/connlimit_test.go:82
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: map[string]interface {}{"ct count":map[string]interface {}{"inv":false, "val":7}}
                                        actual  : map[string]interface {}{"ct count":map[string]interface {}{"val":7}}
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1,4 +1,3 @@
                                         (map[string]interface {}) (len=1) {
                                        - (string) (len=8) "ct count": (map[string]interface {}) (len=2) {
                                        -  (string) (len=3) "inv": (bool) false,
                                        + (string) (len=8) "ct count": (map[string]interface {}) (len=1) {
                                           (string) (len=3) "val": (float64) 7
                        Test:           Test_ConnlimitEncoder/Test_ConnlimitEncodeJSON/simple_count
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.008s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 







