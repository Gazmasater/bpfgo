package main

import (
	"fmt"
	"log"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

type tracepointProbe struct {
	category string
	name     string
	program  *ebpf.Program
}

type probeGroup struct {
	name     string
	required bool
	probes   []tracepointProbe
}

func tracepoint(category, name string, program *ebpf.Program) tracepointProbe {
	return tracepointProbe{category: category, name: name, program: program}
}

func buildProbeGroups(objects *bpfObjects, includeMmsg, includeRW, includeL3 bool) []probeGroup {
	groups := []probeGroup{
		{
			name:     "core socket syscalls",
			required: true,
			probes: []tracepointProbe{
				tracepoint("syscalls", "sys_enter_bind", objects.TraceBindEnter),
				tracepoint("syscalls", "sys_exit_bind", objects.TraceBindExit),
				tracepoint("syscalls", "sys_enter_connect", objects.TraceConnectEnter),
				tracepoint("syscalls", "sys_exit_connect", objects.TraceConnectExit),
				tracepoint("syscalls", "sys_enter_accept4", objects.TraceAccept4Enter),
				tracepoint("syscalls", "sys_exit_accept4", objects.TraceAccept4Exit),
				tracepoint("syscalls", "sys_enter_accept", objects.TraceAcceptEnter),
				tracepoint("syscalls", "sys_exit_accept", objects.TraceAcceptExit),
				tracepoint("syscalls", "sys_enter_close", objects.TraceCloseEnter),
				tracepoint("syscalls", "sys_enter_sendto", objects.TraceSendtoEnter),
				tracepoint("syscalls", "sys_exit_sendto", objects.TraceSendtoExit),
				tracepoint("syscalls", "sys_enter_recvfrom", objects.TraceRecvfromEnter),
				tracepoint("syscalls", "sys_exit_recvfrom", objects.TraceRecvfromExit),
				tracepoint("syscalls", "sys_enter_sendmsg", objects.TraceSendmsgEnter),
				tracepoint("syscalls", "sys_exit_sendmsg", objects.TraceSendmsgExit),
				tracepoint("syscalls", "sys_enter_recvmsg", objects.TraceRecvmsgEnter),
				tracepoint("syscalls", "sys_exit_recvmsg", objects.TraceRecvmsgExit),
			},
		},
	}

	if includeMmsg {
		groups = append(groups, probeGroup{
			name: "mmsg syscalls",
			probes: []tracepointProbe{
				tracepoint("syscalls", "sys_enter_sendmmsg", objects.TraceSendmmsgEnter),
				tracepoint("syscalls", "sys_exit_sendmmsg", objects.TraceSendmmsgExit),
				tracepoint("syscalls", "sys_enter_recvmmsg", objects.TraceRecvmmsgEnter),
				tracepoint("syscalls", "sys_exit_recvmmsg", objects.TraceRecvmmsgExit),
			},
		})
	}

	if includeRW {
		groups = append(groups, probeGroup{
			name: "socket read/write syscalls",
			probes: []tracepointProbe{
				tracepoint("syscalls", "sys_enter_write", objects.TraceWriteEnter),
				tracepoint("syscalls", "sys_exit_write", objects.TraceWriteExit),
				tracepoint("syscalls", "sys_enter_read", objects.TraceReadEnter),
				tracepoint("syscalls", "sys_exit_read", objects.TraceReadExit),
			},
		})
	}

	if includeL3 {
		groups = append(groups, probeGroup{
			name: "L3 socket hints",
			probes: []tracepointProbe{
				tracepoint("net", "net_dev_queue", objects.TraceNetDevQueue),
			},
		})
	}

	return groups
}

func closeLinks(links []link.Link) {
	for _, link := range links {
		_ = link.Close()
	}
}

func attachProbeGroups(groups []probeGroup) ([]link.Link, error) {
	links := make([]link.Link, 0, 26)
	for _, group := range groups {
		groupLinks := make([]link.Link, 0, len(group.probes))
		for _, probe := range group.probes {
			if probe.program == nil {
				closeLinks(groupLinks)
				if group.required {
					closeLinks(links)
					return nil, fmt.Errorf("required probe group %q has no program for %s/%s", group.name, probe.category, probe.name)
				}
				log.Printf("probe group %q disabled: missing program for %s/%s", group.name, probe.category, probe.name)
				groupLinks = nil
				break
			}

			attached, err := link.Tracepoint(probe.category, probe.name, probe.program, nil)
			if err != nil {
				closeLinks(groupLinks)
				if group.required {
					closeLinks(links)
					return nil, fmt.Errorf("attach required probe group %q at %s/%s: %w", group.name, probe.category, probe.name, err)
				}
				log.Printf("probe group %q disabled at %s/%s: %v", group.name, probe.category, probe.name, err)
				groupLinks = nil
				break
			}
			groupLinks = append(groupLinks, attached)
		}
		links = append(links, groupLinks...)
		if len(groupLinks) != 0 {
			log.Printf("probe group enabled: name=%q tracepoints=%d", group.name, len(groupLinks))
		}
	}
	return links, nil
}
