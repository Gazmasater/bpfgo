package pkg

import (
	"fmt"

	"github.com/vishvananda/netlink"
)

func CreateVethPair(namespaceFD uintptr) error {
	// Создаем veth-пару
	linkAttrs := netlink.NewLinkAttrs()
	linkAttrs.Name = "veth0"
	veth0 := &netlink.Veth{
		LinkAttrs: linkAttrs,
		PeerName:  "veth1",
	}

	// Добавляем veth-пару в текущий namespace
	if err := netlink.LinkAdd(veth0); err != nil {
		return fmt.Errorf("не удалось создать veth интерфейс: %w", err)
	}

	// Включаем veth0
	if err := netlink.LinkSetUp(veth0); err != nil {
		return fmt.Errorf("не удалось включить veth0: %w", err)
	}

	// Получаем ссылку на veth1
	peerLink, err := netlink.LinkByName("veth1")
	if err != nil {
		return fmt.Errorf("не удалось найти интерфейс veth1: %w", err)
	}

	// Переносим veth1 в другой namespace
	if err := netlink.LinkSetNsFd(peerLink, int(namespaceFD)); err != nil {
		return fmt.Errorf("не удалось переместить veth1 в namespace: %w", err)
	}

	return nil
}
