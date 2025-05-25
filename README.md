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
	"testing"
	"time"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dynsetEncoderTestSuite struct {
	suite.Suite
}

func (sui *dynsetEncoderTestSuite) Test_DynsetEncodeIR() {
	testData := []struct {
		name     string
		dynset   *expr.Dynset
		srcKey   string
		timeout  time.Duration
		op       DynSetOP
		expected string
	}{
		{
			name:     "add",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPAdd), SetName: "myset", SrcRegKey: 1},
			srcKey:   "10.0.0.1",
			expected: "add @myset { 10.0.0.1 }",
		},
		{
			name:     "add with timeout",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPAdd), SetName: "myset", SrcRegKey: 2, Timeout: 10 * time.Second},
			srcKey:   "192.168.1.10",
			timeout:  10 * time.Second,
			expected: "add @myset { 192.168.1.10 timeout 10s }",
		},
		{
			name:     "update",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPUpdate), SetName: "myset", SrcRegKey: 3},
			srcKey:   "testkey",
			expected: "update @myset { testkey }",
		},
		{
			name:     "delete",
			dynset:   &expr.Dynset{Operation: uint32(DynSetOPDelete), SetName: "myset", SrcRegKey: 4},
			srcKey:   "remove_this",
			expected: "delete @myset { remove_this }",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			// Настраиваем regHolder с нужным regVal по srcRegKey
			var reg regHolder
			reg.Set(regID(tc.dynset.SrcRegKey), regVal{
				HumanExpr: tc.srcKey,
			})
			ctx := &ctx{
				reg: reg,
			}

			enc := &dynsetEncoder{dynset: tc.dynset}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func Test_DynsetEncoder(t *testing.T) {
	suite.Run(t, new(dynsetEncoderTestSuite))
}


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_DynsetEncoder (0.00s)
    --- FAIL: Test_DynsetEncoder/Test_DynsetEncodeIR (0.00s)
        --- FAIL: Test_DynsetEncoder/Test_DynsetEncodeIR/add (0.00s)
            dynset_test.go:64: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:64
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Received unexpected error:
                                        ctx has no rule
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoder).EncodeIR
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset.go:28
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoderTestSuite).Test_DynsetEncodeIR.func1
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:63
                                        github.com/stretchr/testify/suite.(*Suite).Run.func1
                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                                        testing.tRunner
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792
                                        runtime.goexit
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/asm_amd64.s:1700
                        Test:           Test_DynsetEncoder/Test_DynsetEncodeIR/add
        --- FAIL: Test_DynsetEncoder/Test_DynsetEncodeIR/add_with_timeout (0.00s)
            dynset_test.go:64: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:64
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Received unexpected error:
                                        ctx has no rule
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoder).EncodeIR
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset.go:28
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoderTestSuite).Test_DynsetEncodeIR.func1
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:63
                                        github.com/stretchr/testify/suite.(*Suite).Run.func1
                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                                        testing.tRunner
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792
                                        runtime.goexit
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/asm_amd64.s:1700
                        Test:           Test_DynsetEncoder/Test_DynsetEncodeIR/add_with_timeout
        --- FAIL: Test_DynsetEncoder/Test_DynsetEncodeIR/update (0.00s)
            dynset_test.go:64: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:64
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Received unexpected error:
                                        ctx has no rule
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoder).EncodeIR
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset.go:28
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoderTestSuite).Test_DynsetEncodeIR.func1
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:63
                                        github.com/stretchr/testify/suite.(*Suite).Run.func1
                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                                        testing.tRunner
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792
                                        runtime.goexit
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/asm_amd64.s:1700
                        Test:           Test_DynsetEncoder/Test_DynsetEncodeIR/update
        --- FAIL: Test_DynsetEncoder/Test_DynsetEncodeIR/delete (0.00s)
            dynset_test.go:64: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:64
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Received unexpected error:
                                        ctx has no rule
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoder).EncodeIR
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset.go:28
                                        github.com/Morwran/nft-go/internal/expr-encoders.(*dynsetEncoderTestSuite).Test_DynsetEncodeIR.func1
                                                /home/gaz358/myprog/nft-go/internal/expr-encoders/dynset_test.go:63
                                        github.com/stretchr/testify/suite.(*Suite).Run.func1
                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                                        testing.tRunner
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792
                                        runtime.goexit
                                                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/asm_amd64.s:1700
                        Test:           Test_DynsetEncoder/Test_DynsetEncodeIR/delete
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.008s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 



