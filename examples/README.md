## SGX-Ray Demo Examples

### Overview

This directory contains toy SGX applications that are derived based on real world vulerabilties.
Description of each subdirectory is listed as follows.

* `copytohost` -- write to abitratry enclave location due to ineffective locality checks
* `getsockopt` -- overwrite inside enclave due to unchecked ocall return values
* `toctou` -- time-of-check to time-of-use vulnerability

### Run examples

Please refer to the [documentation](../docs/bc-production.md#intel-sgx-sdk-frontend-image) on how to obtain bc files for the examples.
The README file inside each subdirectory contains the invocations to SGX-Ray CLI.
