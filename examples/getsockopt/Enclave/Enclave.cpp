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
#include "Enclave_t.h" /* print_string */

#define SO_BROADCAST 2
#define SO_REUSEADDR 3

int my_ecall_error() {
  sgx_status_t ret = SGX_ERROR_UNEXPECTED;
  int retval = 0;
  int sockfd = 0;
  int optval = 42;
  unsigned optlen = sizeof(int);

  // Get a TCP socket via ocall
  ret = tcp_socket_ocall(&sockfd);
  if (ret != SGX_SUCCESS || sockfd == -1)
    return SGX_ERROR_UNEXPECTED;

  // Check SO_BROADCAST
  if ((ret = getsockopt_ocall(
          &retval, sockfd,
          SO_BROADCAST,
          &optval, optlen, &optlen)) != SGX_SUCCESS
      || retval == -1)
    return SGX_ERROR_UNEXPECTED;

  // Return if transmission of broadcast message is not supported
  if (!optval)
    return SGX_ERROR_UNEXPECTED;

  // Check SO_REUSEADDR
  if ((ret = getsockopt_ocall(
          &retval, sockfd,
          SO_REUSEADDR,
          &optval, optlen, &optlen)) != SGX_SUCCESS
      || retval == -1)
    return SGX_ERROR_UNEXPECTED;

  // Return if socket reuse is not enabled
  if (!optval)
    return SGX_ERROR_UNEXPECTED;

  return SGX_SUCCESS;
}

int my_ecall_ok() {
  sgx_status_t ret = SGX_ERROR_UNEXPECTED;
  int retval = 0;
  int sockfd = 0;
  int optval = 42;
  unsigned optlen = sizeof(int);

  // Get a TCP socket via ocall
  ret = tcp_socket_ocall(&sockfd);
  if (ret != SGX_SUCCESS || sockfd == -1)
    return SGX_ERROR_UNEXPECTED;

  // Check SO_BROADCAST
  if ((ret = getsockopt_ocall(
          &retval, sockfd,
          SO_BROADCAST,
          &optval, optlen, &optlen)) != SGX_SUCCESS
      || retval == -1)
    return SGX_ERROR_UNEXPECTED;

  // Return if transmission of broadcast message is not supported
  if (!optval)
    return SGX_ERROR_UNEXPECTED;

  // Check optlen
  if (optlen > sizeof(int))
    return SGX_ERROR_UNEXPECTED;

  // Check SO_REUSEADDR
  if ((ret = getsockopt_ocall(
          &retval, sockfd,
          SO_REUSEADDR,
          &optval, optlen, &optlen)) != SGX_SUCCESS
      || retval == -1)
    return SGX_ERROR_UNEXPECTED;

  // Return if socket reuse is not enabled 
  if (!optval)
    return SGX_ERROR_UNEXPECTED;

  return SGX_SUCCESS;
}
