package pkg

import (
	"fmt"

	"github.com/vishvananda/netlink"
)

func CreateVethPair(namespaceFD uintptr) error {
	// Используем netlink для создания пары интерфейсов veth
	linkAttrs := netlink.NewLinkAttrs()
	linkAttrs.Name = "veth0"
	veth0 := &netlink.Veth{
		LinkAttrs: linkAttrs,
		PeerName:  "veth1",
	}

	// Создаем интерфейс veth в текущем неймспейсе
	if err := netlink.LinkAdd(veth0); err != nil {
		return fmt.Errorf("не удалось создать veth интерфейс: %w", err)
	}

	// Включаем интерфейсы
	if err := netlink.LinkSetUp(veth0); err != nil {
		return fmt.Errorf("не удалось включить интерфейс veth0: %w", err)
	}

	// Включаем интерфейс veth1
	peerLink, err := netlink.LinkByName("veth1")
	if err != nil {
		return fmt.Errorf("не удалось найти интерфейс veth1: %w", err)
	}

	if err := netlink.LinkSetUp(peerLink); err != nil {
		return fmt.Errorf("не удалось включить интерфейс veth1: %w", err)
	}

	return nil
}
