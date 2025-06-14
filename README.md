./chromedriver-linux64/chromedriver --browser-binary=./chrome-linux64/chrome



package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"github.com/chromedp/chromedp"
)

func main() {
	ctx, cancel := chromedp.NewExecAllocator(context.Background(),
		chromedp.Flag("headless", false), // запускаем с GUI
		chromedp.Flag("disable-blink-features", "AutomationControlled"),
	)
	defer cancel()

	ctx, cancel = chromedp.NewContext(ctx)
	defer cancel()

	var html string
	err := chromedp.Run(ctx,
		chromedp.Navigate("https://ozon.ru"),
		chromedp.Sleep(3*time.Second),
		chromedp.OuterHTML("html", &html),
	)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("HTML длина:", len(html))
}


 www.tractorhouse.com.














https://github.com/Morwran/nft-go/tree/main/internal/expr-encoders



https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force


func resolveHost(ip net.IP) string {

	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		if ip.IsLoopback() {
			host = "localhost"
		} else {
			host = pkg.ResolveIP(ip)

		}
	} else {
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor


____________________________________________________________________________________________


Request URL
https://www.avito.ru/web/1/socket/fallback
Request Method
POST
Status Code
200 OK
Remote Address
176.114.122.24:443
Referrer Policy
strict-origin-when-cross-origin
content-length
64
content-type
application/json
date
Sat, 14 Jun 2025 23:08:33 GMT
server
QRATOR
set-cookie
_avisc=7ceb9h2o8zfNAmuNHzrbJD+aA76rRI7Qd+4W9fOFTHw=; path=/; expires=Sat, 14-Jun-25 23:09:33 GMT; HttpOnly; Max-Age=60; secure; domain=.avito.ru; SameSite=Lax
set-cookie
v=1749942115; path=/; expires=Sat, 14-Jun-25 23:38:33 GMT; HttpOnly; Max-Age=1800; secure; domain=.avito.ru; SameSite=Lax
x-content-type-options
nosniff
x-frame-options
allow-from https://pro.avito.ru/
x-xss-protection
1; mode=block
:authority
www.avito.ru
:method
POST
:path
/web/1/socket/fallback
:scheme
https
accept
*/*
accept-encoding
gzip, deflate, br, zstd
accept-language
en-US,en;q=0.9
content-length
87
content-type
application/json
cookie
srv_id=zethYZi1Y3c876-z.9Wh5NT7y5BhvSWmCEFNdMFiXylt4wmrPcxWjPFFFPIQjW7Iwh8boAhlAGyj_kDE=.XjmrT2FoVVOqog2mJPPUWS7KmAhguu1XVviO81ym8iU=.web; gMltIuegZN2COuSe=EOFGWsm50bhh17prLqaIgdir1V0kgrvN; u=374p8pl7.nnzso.ferc99ha7p00; v=1749942115; dfp_group=63; _gcl_au=1.1.275932962.1749942117; ma_cid=1749942117455700366; _ga=GA1.1.208614939.1749942118; cssid=7ad8a044-f3b5-41ba-8e9d-3f3d81f3bdf4; cssid_exp=1749943917787; _ym_uid=1749942120398703865; _ym_d=1749942120; ma_id=1178137741749942117659; tmr_lvid=ed5bd9189191f5b0199bd90d1504d7d3; tmr_lvidTS=1749942120175; _ym_isad=2; uxs_uid=94312200-4973-11f0-aab3-63e80b4e6619; _ym_visorc=b; cookie_consent_shown=1; adrdel=1749942120833; adrdel=1749942120833; adrcid=A6kFCd-Syz2l8YWo08BLUWA; adrcid=A6kFCd-Syz2l8YWo08BLUWA; acs_3=%7B%22hash%22%3A%221aa3f9523ee6c2690cb34fc702d4143056487c0d%22%2C%22nst%22%3A1750028521511%2C%22sl%22%3A%7B%22224%22%3A1749942121511%2C%221228%22%3A1749942121511%7D%7D; acs_3=%7B%22hash%22%3A%221aa3f9523ee6c2690cb34fc702d4143056487c0d%22%2C%22nst%22%3A1750028521511%2C%22sl%22%3A%7B%22224%22%3A1749942121511%2C%221228%22%3A1749942121511%7D%7D; domain_sid=5hXYwspEK7fO0BQVBffys%3A1749942121550; __ai_fp_uuid=478e69f334473e0d%3A1; __upin=NxOcu6lE7GcbvEi8Ejglxw; yandex_monthly_cookie=true; _buzz_aidata=JTdCJTIydWZwJTIyJTNBJTIyTnhPY3U2bEU3R2NidkVpOEVqZ2x4dyUyMiUyQyUyMmJyb3dzZXJWZXJzaW9uJTIyJTNBJTIyMTM3LjAlMjIlMkMlMjJ0c0NyZWF0ZWQlMjIlM0ExNzQ5OTQyMTIyMjI0JTdE; _buzz_mtsa=JTdCJTIydWZwJTIyJTNBJTIyMDQxNDQzMTBkMmUwMjIxMTg0MzdhOTcyMDgzNjZmNGUlMjIlMkMlMjJicm93c2VyVmVyc2lvbiUyMiUzQSUyMjEzNy4wJTIyJTJDJTIydHNDcmVhdGVkJTIyJTNBMTc0OTk0MjEyMjExNiU3RA==; __zzatw-avito=MDA0dBA=Fz2+aQ==; __zzatw-avito=MDA0dBA=Fz2+aQ==; f=5.0c4f4b6d233fb90636b4dd61b04726f147e1eada7172e06c47e1eada7172e06c47e1eada7172e06c47e1eada7172e06cb59320d6eb6303c1b59320d6eb6303c1b59320d6eb6303c147e1eada7172e06c8a38e2c5b3e08b898a38e2c5b3e08b890df103df0c26013a7b0d53c7afc06d0b2ebf3cb6fd35a0ac0df103df0c26013a8b1472fe2f9ba6b9c7f279391b0a3959c7cea19ce9ef44010f7bd04ea141548c78ba5f931b08c66a2a125ecd8b100f4a7b0d53c7afc06d0b03c77801b122405c2da10fb74cac1eab2da10fb74cac1eabdc5322845a0cba1af722fe85c94f7d0c2da10fb74cac1eab2da10fb74cac1eab2da10fb74cac1eab2da10fb74cac1eab3c02ea8f64acc0bd853206102760b3e6de87ad3b397f946b4c41e97fe93686adbf5c86bc0685a4fff93da262b7c8bbdf02c730c0109b9fbb08962791da47acc56d14df90d850b02ec5584122abfc8baed5a57d75e607083e698d3b18dd2807ee2ebf3cb6fd35a0ac0df103df0c26013a28a353c4323c7a3aefcfb0a8b11101951e002d18e88425e73de19da9ed218fe23de19da9ed218fe2ddb881eef125a8703b2a42e40573ac3c567037a1138bc3a59ddcb939a425458a; ft="xdLbPnXEjaoxhQbuI/RlixPEKZiArz7Rlx4seUJ7mCV6pHFPIK6DRDXivVJJQh3bIcVFUqr9CkXIMWfdYFlaF5UDQt/dIugOVuud9y7+1Ih1xU1ib227jUTHvb7H5X2yo/pKKh+iLKSU4AXvSSCzp1iji+6KU6Mn+pHLVSx/3avPrVGm/9ad5EYmP2XSTKby"; cfidsw-avito=CP4inJATz0bqq6O6maDR+LrBIYN7EA4KkPsfS+ZR48vmcCeMjXW6U1auHnI3QbfhIkpYM/kk6SxWub5NKZN4nutAI8HBS1JMbhLMt92t8DDEqzTWLHPAVDioCekkQO2ZBviGO31Y/DrCTUMiKKfkeCzF/PqxtkPB9PUX; cfidsw-avito=CP4inJATz0bqq6O6maDR+LrBIYN7EA4KkPsfS+ZR48vmcCeMjXW6U1auHnI3QbfhIkpYM/kk6SxWub5NKZN4nutAI8HBS1JMbhLMt92t8DDEqzTWLHPAVDioCekkQO2ZBviGO31Y/DrCTUMiKKfkeCzF/PqxtkPB9PUX; auth=1; cfidsw-avito=VDmru3ncLwOMcjjmEskcJOSjCZ+BuA/y6CsPpd/5gDv5S7pivH1zjKtbfg6au0Ged3kOu9Nm4VHzqsJFzP2K7mBP3YXuObKnWCb0BrNmya6m8DWecUCECNVC/gHQK4OPVRzrh88UheqUqJ8J2cgaZeU7ZxObWLoprWwa; sessid=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NTAwMjg2MDMsImlhdCI6MTc0OTk0MjIwMywidSI6MTAxODAwMTcyLCJwIjoxMjI3MTQxOCwicyI6IjJjODdkYjg4ZjE1Njc3YWYwMjZlZDA3OTNlMWM3YzIzLjE3NDk5NDIyMDMiLCJoIjoiZGIzNWUyNTQ5ZTdhYzZlOGQxNzg1NTFhYTdhZWQ0ZmRfMTc0OTk0MjIwMyIsInYiOmZhbHNlLCJleHRyYSI6bnVsbH0._EAQyu-IK7R0DbmHYtixpLAxoeJTlZ6IGfcfSBKlb6Ta10_JqMsFIshcDkFDTK4cqDEv_zCoDhjiUWtLSrYGiw; rt=bbe79669819db10b82691ad9e8f39a46; buyer_laas_location=637440; luri=lipetsk; buyer_location_id=637440; sx=H4sIAAAAAAAC%2F6pWMjMzM0tOMTdLszSzNDUzMbNMNU9KNbZMMTc1SE42T7FUsqpWKlOyUkp2zfMyLfDN9vQrLy0yLLZQ0lFKVbIyNDc1MDCyMDMwrq0FBAAA%2F%2F%2FOXwfbTAAAAA%3D%3D; isLegalPerson=0; buyer_from_page=main; abp=0; tmr_detect=0%7C1749942215927; ma_ss_64a8dba6-67f3-4fe4-8625-257c4adae014=1749942117950832094.1.1749942227.11.1749942117; _ga_M29JC28873=GS2.1.s1749942117$o1$g1$t1749942246$j18$l0$h0; csprefid=36221e61-46ed-42df-8c66-0b3e782a96ca; _ga_WW6Q1STJ8M=GS2.1.s1749942247$o1$g0$t1749942247$j60$l0$h0; _ga_ZJDLBTV49B=GS2.1.s1749942247$o1$g0$t1749942247$j60$l0$h0
origin
https://www.avito.ru
priority
u=1, i
referer
https://www.avito.ru/profile/notifications
sec-ch-ua
"Chromium";v="137", "Not/A)Brand";v="24"
sec-ch-ua-mobile
?0
sec-ch-ua-platform
"Linux"
sec-fetch-dest
empty
sec-fetch-mode
cors
sec-fetch-site
same-origin
user-agent
Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/137.0.0.0 Safari/537.36


package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"github.com/chromedp/chromedp"
)

func main() {
	ctx, cancel := chromedp.NewExecAllocator(context.Background(),
		chromedp.Flag("headless", false), // запускаем с GUI
		chromedp.Flag("disable-blink-features", "AutomationControlled"),
	)
	defer cancel()

	ctx, cancel = chromedp.NewContext(ctx)
	defer cancel()

	var html string
	err := chromedp.Run(ctx,
		chromedp.Navigate("https://www.avito.ru"),
		chromedp.Sleep(5500*time.Second),
		chromedp.OuterHTML("html", &html),
	)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("HTML длина:", len(html))
}






