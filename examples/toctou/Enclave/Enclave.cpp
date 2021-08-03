/*
 * Copyright 2021 Baidu 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "Enclave.h"
#include "Enclave_t.h"
#include "sgx_trts.h"

#define SIZE 2

void **allocate_untrusted_buffers(size_t count, size_t size);
void free_buffers(void *buffer_pool_[], int kPoolNumber);

void init_buffers(size_t kPoolNumber, size_t kPoolEntrySize, void * buffer_pool_[])
{
    void **buffers = NULL;

    buffers = allocate_untrusted_buffers(kPoolNumber, kPoolEntrySize);
    if(buffers == NULL){
        abort();
    }
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        if (!buffers[i] || !sgx_is_outside_enclave(buffers[i], kPoolEntrySize))
        {
            abort();
        }
        buffer_pool_[i] = buffers[i];
    }

    ocall_untrusted_local_free(buffers);
}

void ecall_test_allocate_buffers_error()
{
    void * buffer_pool_[SIZE];
    const size_t kPoolEntrySize = sizeof(uint64_t);
    const size_t kPoolNumber = SIZE;
    init_buffers(kPoolNumber, kPoolEntrySize, buffer_pool_);
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        uint64_t *untrusted_ptr = (uint64_t *)buffer_pool_[i];
        *untrusted_ptr = i;
    }

    free_buffers(buffer_pool_, kPoolNumber);
    return;
}

void **allocate_untrusted_buffers(size_t count, size_t size)
{
    void **buffers;
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ocall_enc_untrusted_allocate_buffers(
        &buffers, (uint64_t)count, size);
    if (ret != SGX_SUCCESS || !sgx_is_outside_enclave(buffers, count * sizeof(void *)))
    {
        abort();
    }
    return buffers;
}

void free_buffers(void *buffer_pool_[], int kPoolNumber)
{
    for (int i = 0; i < kPoolNumber; ++i)
    {
        ocall_untrusted_local_free(buffer_pool_[i]);
    }
}
