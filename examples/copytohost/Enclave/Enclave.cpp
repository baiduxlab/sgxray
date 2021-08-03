/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "Enclave.h"
#include "Enclave_t.h"
#include "sgx_trts.h"

uint8_t results[2] = {'a', 'b'};

int copy_to_host1(uint8_t* buffer, size_t size)
{
    void *result;

    // Allocate host memory
    sgx_status_t status = ocall_untrusted_host_alloc(&result, size);
    if (status != SGX_SUCCESS || result == NULL)
        return -1;

    // Validate the return value from host
    // Ineffective e.g.,
    // ___________
    // host mem
    // ...
    // -----------   <-- enclave_mem_end + 1 (result)
    // -----------   <-- enclave_mem_end (result)
    // enclave mem
    // ...
    // -----------
    if (sgx_is_within_enclave(result, size))
    {
        return -1;
    }

    // Copy buffer to host memory
    uint8_t *untrusted_buffer = (uint8_t *)result;
    for (size_t i = 0; i < size; ++i)
    {
        untrusted_buffer[i] = buffer[i];
    }

    return 0;
}

int copy_to_host2(uint8_t* buffer, size_t size)
{
    void *result;

    // Allocate host memory
    sgx_status_t status = ocall_untrusted_host_alloc(&result, size);
    if (status != SGX_SUCCESS || result == NULL)
        return -1;

    // Still an invalid check, just for demo
    if (!sgx_is_outside_enclave(result, 1))
    {
        return -1;
    }

    // Copy buffer to host memory
    uint8_t *untrusted_buffer = (uint8_t *)result;
    // Conceptually, this loop is converted into
    // when unroll bound is 1
    // if (i < size) {
    //   untrusted_buffer[i] = buffer[i];
    //   i += 1;
    //   abort();
    // }
    // when unroll bound is 2
    // if (i < size) {
    //   untrusted_buffer[i] = buffer[i];
    //   i += 1;
    //   if (i < size) {
    //     untrusted_buffer[i] = buffer[i];
    //     i += 1;
    //     abort();
    //   }
    // }
    for (size_t i = 0; i < size; ++i)
    {
        untrusted_buffer[i] = buffer[i];
    }

    return 0;
}

void ecall_error1() {
   copy_to_host1(results, 2);
}

void ecall_error2() {
   copy_to_host2(results, 2);
}
