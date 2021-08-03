## Run SGXRay CLI
```bash
sgx-ray --intel --ecall-name ecall_error1 enclave.so.bc
sgx-ray --intel --ecall-name ecall_error2 enclave.so.bc --smack-options="--unroll=2"
```

## Vulnerability Description
This example represents a common pattern in programming enclaves where a large buffer in the untrusted part is requested by an enclave via an ocall and the enclave then writes encrypted data into it. In this particular example, a pointer `result` is set by an ocall and hypothetically points to an aforementioned buffer. This pointer needs to be checked carefully otherwise security vulnerabilities arise. `ecall_error1` contains a typical error where the check upon `result` is incorrect. `ecall_error2` is variant of `ecall_error1` to demonstrate an important SMACK option `--unroll`.

### ecall_error1
`sgx_is_within_enclave` is not a sufficient check because if `result` points to to an object across enclave boundaries, it can bypass this check and its dereference can write to an arbitrary location inside the enclave.

### ecall_error2
Although `ecall_error2` uses the correct API (`!sgx_is_outside_enclave`), the size argument is not correct (for the purpose of demonstration). Therefore, the accessing the first byte of the memory object `result` points to should not place a security thread. However, the second one would. By default, SGXRay unrolls the loop bound only once and therefore misses the bug in the second iteration. Adding option `--smack-options='--unroll=2'` reveals this bug.
