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



func (sui *bitwiseEncoderTestSuite) Test_BitwiseEncodeIR() {
	testData := []struct {
		name     string
		srcReg   uint32
		dstReg   uint32
		mask     []byte
		xor      []byte
		srcExpr  expr.Any
		srcHuman string
		expected string
	}{
		{
			name:     "bitwise AND mask",
			srcReg:   1,
			dstReg:   2,
			mask:     []byte{0xf0},
			xor:      nil,
			srcExpr:  nil,
			srcHuman: "ip daddr",
			expected: "(ip daddr) & 0xf0",
		},
		{
			name:     "bitwise AND+XOR mask",
			srcReg:   3,
			dstReg:   4,
			mask:     []byte{0xff},
			xor:      []byte{0x0f},
			srcExpr:  nil,
			srcHuman: "meta mark",
			expected: "((meta mark) & 0xff) ^ 0xf",
		},
		{
			name:     "bitwise with XOR (xor != 0, mask != 0)",
			srcReg:   5,
			dstReg:   6,
			mask:     []byte{0xf0},
			xor:      []byte{0xf0},
			srcExpr:  nil,
			srcHuman: "payload 2",
			expected: "((payload 2) & 0xf0) ^ 0xf0",
		},
		// ---- Расширенные кейсы ----
		{
			name:     "bitwise multi-byte mask AND",
			srcReg:   7,
			dstReg:   8,
			mask:     []byte{0xff, 0x0f},
			xor:      nil,
			srcExpr:  nil,
			srcHuman: "ip saddr",
			expected: "(ip saddr) & 0xff0f",
		},
		{
			name:     "bitwise full mask all ones",
			srcReg:   9,
			dstReg:   10,
			mask:     []byte{0xff, 0xff, 0xff, 0xff},
			xor:      nil,
			srcExpr:  nil,
			srcHuman: "data[0]",
			expected: "(data[0]) & 0xffffffff",
		},
		{
			name:     "bitwise only XOR (mask all ones)",
			srcReg:   11,
			dstReg:   12,
			mask:     []byte{0xff, 0xff},
			xor:      []byte{0x12, 0x34},
			srcExpr:  nil,
			srcHuman: "payload 4",
			expected: "((payload 4) & 0xffff) ^ 0x1234",
		},
		{
			name:     "bitwise only XOR (mask zeros)",
			srcReg:   13,
			dstReg:   14,
			mask:     []byte{0x00, 0x00},
			xor:      []byte{0xff, 0xff},
			srcExpr:  nil,
			srcHuman: "payload 6",
			expected: "(payload 6) & 0x0",
		},
		{
			name:     "bitwise no mask (nil mask, xor)",
			srcReg:   15,
			dstReg:   16,
			mask:     nil,
			xor:      []byte{0x1},
			srcExpr:  nil,
			srcHuman: "meta priority",
			expected: "(meta priority) & 0x0 ^ 0x1",
		},
		{
			name:     "bitwise with srcExpr (Ct)",
			srcReg:   17,
			dstReg:   18,
			mask:     []byte{0xf0},
			xor:      []byte{0x0a},
			srcExpr:  &expr.Ct{}, // Здесь мы проверяем, что buildCtWithMask сработает
			srcHuman: "ct state",
			expected: "ct state 0xf0",
		},
		{
			name:     "bitwise with srcExpr (Payload)",
			srcReg:   19,
			dstReg:   20,
			mask:     []byte{0xa5},
			xor:      nil,
			srcExpr:  &expr.Payload{},
			srcHuman: "payload mark",
			expected: "payload mark 0xa5",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			var reg regHolder
			reg.Set(regID(tc.srcReg), regVal{
				HumanExpr: tc.srcHuman,
				Expr:      tc.srcExpr,
			})
			ctx := &ctx{
				reg:  reg,
				rule: nil,
			}

			bitwise := &expr.Bitwise{
				SourceRegister: tc.srcReg,
				DestRegister:   tc.dstReg,
				Mask:           tc.mask,
				Xor:            tc.xor,
				Len:            uint32(len(tc.mask)),
			}
			enc := &bitwiseEncoder{bitwise: bitwise}
			_, err := enc.EncodeIR(ctx)
			if err != nil && err != ErrNoIR {
				sui.Require().NoError(err)
			}
			res, ok := ctx.reg.Get(regID(tc.dstReg))
			sui.Require().True(ok)
			sui.Require().Equal(tc.expected, res.HumanExpr)
		})
	}
}









