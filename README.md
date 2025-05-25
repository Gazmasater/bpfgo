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

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type hashEncoderTestSuite struct {
	suite.Suite
}

func (sui *hashEncoderTestSuite) Test_HashEncodeIR() {
	testData := []struct {
		name     string
		hash     *expr.Hash
		srcReg   uint32
		srcHuman string
		expected string
	}{
		{
			name: "simple symhash",
			hash: &expr.Hash{
				Type:         expr.HashTypeSym,
				Modulus:      10,
				Seed:         0x1234,
				Offset:       0,
				DestRegister: 1,
			},
			expected: "symhash mod 10 seed 0x1234",
		},
		{
			name: "symhash with offset",
			hash: &expr.Hash{
				Type:         expr.HashTypeSym,
				Modulus:      16,
				Seed:         0xdead,
				Offset:       7,
				DestRegister: 2,
			},
			expected: "symhash mod 16 seed 0xdead offset 7",
		},
		{
			name: "jhash with src",
			hash: &expr.Hash{
				Type:           expr.HashTypeJenkins, // используем jhash (jenkins)
				Modulus:        5,
				Seed:           0x1111,
				Offset:         0,
				SourceRegister: 3,
				DestRegister:   4,
			},
			srcReg:   3,
			srcHuman: "meta mark",
			expected: "symhashjhash meta mark mod 5 seed 0x1111", // если твой encoder так формирует строку
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			var reg regHolder
			if tc.srcReg != 0 {
				reg.Set(regID(tc.srcReg), regVal{
					HumanExpr: tc.srcHuman,
				})
			}
			ctx := &ctx{
				reg:  reg,
				rule: nil, // не нужен
			}

			enc := &hashEncoder{hash: tc.hash}
			_, err := enc.EncodeIR(ctx)
			if err != nil && err != ErrNoIR {
				sui.Require().NoError(err)
			}

			res, ok := ctx.reg.Get(regID(tc.hash.DestRegister))
			sui.Require().True(ok)
			sui.Require().Equal(tc.expected, res.HumanExpr)
		})
	}
}

func Test_HashEncoder(t *testing.T) {
	suite.Run(t, new(hashEncoderTestSuite))
}

gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_BitwiseEncoder (0.00s)
    --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR (0.00s)
        --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_full_mask_all_ones (0.00s)
            bitwise_test.go:154: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:154
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "(data[0]) & 0xffffffff"
                                        actual  : "data[0] & 0xffffffff"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -(data[0]) & 0xffffffff
                                        +data[0] & 0xffffffff
                        Test:           Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_full_mask_all_ones
        --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_only_XOR_(mask_zeros) (0.00s)
            bitwise_test.go:154: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:154
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "(payload 6) & 0x0"
                                        actual  : "((payload 6) & 0xffff) | 0xffff"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -(payload 6) & 0x0
                                        +((payload 6) & 0xffff) | 0xffff
                        Test:           Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_only_XOR_(mask_zeros)
        --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_no_mask_(nil_mask,_xor) (0.00s)
            bitwise_test.go:154: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:154
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "(meta priority) & 0x0 ^ 0x1"
                                        actual  : "((meta priority) & 0x1) | 0x1"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -(meta priority) & 0x0 ^ 0x1
                                        +((meta priority) & 0x1) | 0x1
                        Test:           Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_no_mask_(nil_mask,_xor)
        --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_with_srcExpr_(Ct) (0.00s)
            bitwise_test.go:154: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:154
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ct state 0xf0"
                                        actual  : "ct state untracked"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ct state 0xf0
                                        +ct state untracked
                        Test:           Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_with_srcExpr_(Ct)
        --- FAIL: Test_BitwiseEncoder/Test_BitwiseEncodeIR/bitwise_with_srcExpr_(Payload) (0.00s)
            panic.go:262: test panicked: runtime error: invalid memory address or nil pointer dereference
                goroutine 31 [running]:
                runtime/debug.Stack()
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/debug/stack.go:26 +0x5e
                github.com/stretchr/testify/suite.failOnPanic(0xc00024e000, {0x8850e0, 0xcbe540})
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:89 +0x37
                github.com/stretchr/testify/suite.recoverAndFailOnPanic(0xc00024e000)
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:83 +0x35
                panic({0x8850e0?, 0xcbe540?})
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/panic.go:792 +0x132
                github.com/Morwran/nft-go/internal/expr-encoders.(*payloadEncoder).buildPlWithMask(0xc000080cf0, 0xc000080e48, {0xc0001e98f3, 0x1, 0x1})
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/payload.go:87 +0x82
                github.com/Morwran/nft-go/internal/expr-encoders.(*bitwiseEncoder).EncodeIR(0x892880?, 0xc000080e48)
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise.go:46 +0x1d0
                github.com/Morwran/nft-go/internal/expr-encoders.(*bitwiseEncoderTestSuite).Test_BitwiseEncodeIR.func1()
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/bitwise_test.go:148 +0x214
                github.com/stretchr/testify/suite.(*Suite).Run.func1(0xc00024e000)
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115 +0x169
                testing.tRunner(0xc00024e000, 0xc000214b00)
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792 +0xf4
                created by testing.(*T).Run in goroutine 21
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1851 +0x413
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.010s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 


