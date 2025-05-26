sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log



sudo nft add table inet test
sudo nft add chain inet test mychain '{ type filter hook input priority 0; }'
sudo nft add rule inet test mychain tcp option 5

sudo nft add rule inet test mychain ip option @10,1,8 set someval
sudo nft add rule inet test mychain reset ip option @4,2,1
sudo nft add rule inet test mychain tcp option 5
sudo nft add rule inet test mychain ip option @7,1,2
sudo nft add rule inet test mychain ip option @4,8,5 set custom
sudo nft add rule inet test mychain reset tcp option @7,2,4


sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting tcp option 5

sudo nft add rule ip test prerouting ip option @10,1,8 set someval

sudo nft add rule ip test prerouting reset ip option @4,2,1




package encoders

import (
	"encoding/json"
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

// ---- IR TEST ----
type exthdrEncoderIRTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderIRTestSuite) Test_ExthdrExprToIR() {
	testData := []struct {
		name     string
		exthdr   *expr.Exthdr
		regSetup func(*ctx)
		expected string
	}{
		{
			name: "DestRegister exthdr",
			exthdr: &expr.Exthdr{
				DestRegister: 1,
				Type:         5,
				Offset:       0,
				Len:          4,
				Op:           expr.ExthdrOpTcpopt,
				Flags:        unix.NFT_EXTHDR_F_PRESENT,
			},
			expected: "tcp option 5",
		},
		{
			name: "SourceRegister exthdr set",
			exthdr: &expr.Exthdr{
				SourceRegister: 2,
				Type:           10,
				Offset:         1,
				Len:            8,
				Op:             expr.ExthdrOpIpv6,
			},
			regSetup: func(ctx *ctx) {
				ctx.reg.Set(regID(2), regVal{HumanExpr: "someval"})
			},
			expected: "ip option @10,1,8 set someval",
		},
		{
			name: "reset exthdr",
			exthdr: &expr.Exthdr{
				Type:   4,
				Op:     expr.ExthdrOpIpv6,
				Offset: 2,
				Len:    1,
			},
			expected: "reset ip option @4,2,1",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{reg: regHolder{}}
			if tc.regSetup != nil {
				tc.regSetup(ctx)
			}
			enc := &exthdrEncoder{extdhdr: tc.exthdr}
			ir, err := enc.EncodeIR(ctx)
			if tc.exthdr.DestRegister != 0 {
				sui.Require().ErrorIs(err, ErrNoIR)
				val, ok := ctx.reg.Get(regID(tc.exthdr.DestRegister))
				sui.Require().True(ok)
				sui.Require().Contains(val.HumanExpr, tc.expected)
				sui.Require().Nil(ir)
			} else {
				sui.Require().NoError(err)
				sui.Require().Equal(tc.expected, ir.Format())
			}
		})
	}
}

func Test_ExthdrEncoderIR(t *testing.T) {
	suite.Run(t, new(exthdrEncoderIRTestSuite))
}

// ---- JSON TEST ----
type exthdrEncoderTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderTestSuite) Test_ExthdrExprToJSON() {
	testData := []struct {
		name           string
		exthdr         *expr.Exthdr
		regSetup       func(*ctx)
		expectedKey    string
		expectedBase   uint8
		expectedOffset uint32
		expectedLen    uint32
		mangleCheck    bool
		mangleValue    string
	}{
		{
			name: "DestRegister json tcp option",
			exthdr: &expr.Exthdr{
				DestRegister: 1,
				Type:         5,
				Offset:       0,
				Len:          4,
				Op:           expr.ExthdrOpTcpopt,
			},
			expectedKey:  "tcp option",
			expectedBase: 5, expectedOffset: 0, expectedLen: 4,
		},
		{
			name: "DestRegister json ip option",
			exthdr: &expr.Exthdr{
				DestRegister: 3,
				Type:         7,
				Offset:       1,
				Len:          2,
				Op:           expr.ExthdrOpIpv6,
			},
			expectedKey:  "ip option",
			expectedBase: 7, expectedOffset: 1, expectedLen: 2,
		},
		{
			name: "SourceRegister json with mangle",
			exthdr: &expr.Exthdr{
				SourceRegister: 2,
				Type:           4,
				Offset:         8,
				Len:            5,
				Op:             expr.ExthdrOpIpv6,
			},
			regSetup: func(ctx *ctx) {
				ctx.reg.Set(regID(2), regVal{Data: "custom"})
			},
			expectedKey:    "ip option",
			expectedBase:   4,
			expectedOffset: 8,
			expectedLen:    5,
			mangleCheck:    true,
			mangleValue:    "custom",
		},
		{
			name: "reset exthdr to json",
			exthdr: &expr.Exthdr{
				Type:   7,
				Offset: 2,
				Len:    4,
				Op:     expr.ExthdrOpTcpopt,
			},
			expectedKey:  "tcp option",
			expectedBase: 7, expectedOffset: 2, expectedLen: 4,
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{reg: regHolder{}}
			if tc.regSetup != nil {
				tc.regSetup(ctx)
			}
			enc := &exthdrEncoder{extdhdr: tc.exthdr}
			b, err := enc.EncodeJSON(ctx)
			if tc.exthdr.DestRegister != 0 {
				sui.Require().ErrorIs(err, ErrNoJSON)
				val, ok := ctx.reg.Get(regID(tc.exthdr.DestRegister))
				sui.Require().True(ok)
				hdr, ok := val.Data.(map[string]interface{})
				sui.Require().True(ok)
				v := hdr[tc.expectedKey]
				var hdrVal map[string]interface{}
				if m, ok := v.(map[string]interface{}); ok {
					hdrVal = m
				} else {
					data, _ := json.Marshal(v)
					json.Unmarshal(data, &hdrVal)
				}
				sui.Require().Equal(float64(tc.expectedBase), hdrVal["base"])
				sui.Require().Equal(float64(tc.expectedOffset), hdrVal["offset"])
				sui.Require().Equal(float64(tc.expectedLen), hdrVal["len"])
				sui.Require().Nil(b)
			} else if tc.exthdr.SourceRegister != 0 {
				if tc.mangleCheck {
					var mangle map[string]map[string]interface{}
					sui.Require().NoError(err)
					sui.Require().NoError(json.Unmarshal(b, &mangle))
					mval := mangle["mangle"]
					keyObj := mval["key"]
					var keyMap map[string]interface{}
					if m, ok := keyObj.(map[string]interface{}); ok {
						keyMap = m[tc.expectedKey].(map[string]interface{})
					} else {
						tmp, _ := json.Marshal(keyObj)
						var tmpMap map[string]interface{}
						json.Unmarshal(tmp, &tmpMap)
						keyMap = tmpMap[tc.expectedKey].(map[string]interface{})
					}
					sui.Require().Equal(float64(tc.expectedBase), keyMap["base"])
					sui.Require().Equal(float64(tc.expectedOffset), keyMap["offset"])
					sui.Require().Equal(float64(tc.expectedLen), keyMap["len"])
					sui.Require().Equal(tc.mangleValue, mval["value"])
				}
			} else {
				var hdr map[string]interface{}
				sui.Require().NoError(err)
				sui.Require().NoError(json.Unmarshal(b, &hdr))
				v := hdr[tc.expectedKey]
				var hdrVal map[string]interface{}
				if m, ok := v.(map[string]interface{}); ok {
					hdrVal = m
				} else {
					data, _ := json.Marshal(v)
					json.Unmarshal(data, &hdrVal)
				}
				sui.Require().Equal(float64(tc.expectedBase), hdrVal["base"])
				sui.Require().Equal(float64(tc.expectedOffset), hdrVal["offset"])
				sui.Require().Equal(float64(tc.expectedLen), hdrVal["len"])
			}
		})
	}
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}






