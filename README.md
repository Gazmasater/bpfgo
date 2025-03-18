
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

type RawAttachProgramOptions struct {
    // Target to query. This is usually a file descriptor but may refer to
    // something else based on the attach type.
    Target int
    // Program to attach.
    Program *ebpf.Program
    // Attach must match the attach type of Program.
    Attach ebpf.AttachType
    // Attach relative to an anchor. Optional.
    Anchor Anchor
    // Flags control the attach behaviour. Specify an Anchor instead of
    // F_LINK, F_ID, F_BEFORE, F_AFTER and F_REPLACE. Optional.
    Flags uint32
    // Only attach if the internal revision matches the given value.
    ExpectedRevision uint64
}
