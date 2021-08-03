## Run SGXRay CLI
```bash
sgx-ray --intel --ecall my_ecall_error enclave.so.bc
sgx-ray --intel --ecall my_ecall_ok enclave.so.bc
```

## Vulnerability Description
This example describes a common pattern where enclave requests an IO via ocall. For this particular example, an enclave asks for a socket and checks its options both using ocalls. Note that the `out` buffers of ocalls are controlled by the untrusted part. ocall `getsockopt_ocall` may modify the `optlen` variable passed to it. This variable is further used in the second call to the ocall, which causes ocall to over-read/write the buffer `optbuf`.

### my_ecall_error
This ecall contains the aforementioned vulnerability.

### my_ecall_ok
This ecall is a fixed version of `my_ecall_error`.
