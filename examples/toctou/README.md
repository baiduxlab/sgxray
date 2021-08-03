## Run SGXRay CLI
```bash
sgx-ray --intel --ecall-name ecall_test_allocate_buffers_error --smack-options="--unroll=3"
```

## Vulnerability Description
This example represents a common pattern in programming enclaves where a large two-dimensional buffer in the untrusted part is requested by an enclave via an ocall and the enclave then writes encrypted data into it. In this particular example, a pointer `buffers` refers to this two-dimensional array. The first dimension is made sure to reside outside the enclave in function `allocate_untrusted_buffers`. While in function `init_buffers`, the second dimension arrays are checked to see if they also reside outside the enclave. However, during the check (i.e., the if statement at line 37), malicious host can set the values of `buffer[i]` such that they point to somewhere outside the enclave whereas during the use (i.e., the assignment at line 41), it set their values such that they point to somewhere within the enclave. Dereferencing these pointers causes the write-to-arbitary-enclave-location vulnerability.
