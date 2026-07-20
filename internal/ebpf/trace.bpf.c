//go:build ignore

/*
 * bpf2go compiles this file as a single BPF translation unit.  The included
 * files are split by responsibility but deliberately share one set of maps
 * and ABI declarations.
 */
#include "abi.h"
#include "maps.h"
#include "helpers.h"

#include "probes/connect.bpf.c"
#include "probes/bind.bpf.c"
#include "probes/io.bpf.c"
#include "probes/lifecycle.bpf.c"
#include "probes/netdev.bpf.c"
