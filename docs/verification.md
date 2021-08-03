## Verification

Currently, the verification step can only be done inside the Docker container we provide.
We recommend run it interactively on our demo machine using the following command,

```shell
cd <project/enclave> # go to the enclave directory that contains the bc file generated in the last step
docker run --rm -it -v $(pwd):/sgx -w /sgx --user $UID baiduxlab/sgx-ray-distro
```

Inside the container, you should be able to invoke the SGXRay CLI `sgx-ray`.

### Verification CLI Overview

We provide a binary `sgx-ray` that acts as a wrapper to SMACK for verifying SGX applications. The options can be shown via `sgx-ray --help`.
Currently, there are two backend verifiers -- Boogie and Corral. Boogie tends to be faster whereas Corral offers better error traces.
We recommend trying Boogie first. If a counterexample is found, one can use Corral to get a better understanding of it.

### Basic Usage of Verification CLI

The basic usage of the SGXRay CLI is pretty simple. Let's assume the SDK used to build your app is Intel SGX SDK.

```shell
sgx-ray --intel --ecall-name <ecallname> <enclave.so.bc>
```

Ecall names can be found in the EDL file of your app.

### SMACK Options

The important SMACK options can be found at the end of the demo video. We also reiterate the important ones here.

#### Loop Unrolling Bound

Recall that in the tutorial, we see SGXRay can successfully detect an invalid pointer usage in the `ecall_error1` function of `copytohost`. If we apply the same command to `ecall_error2`, we will fail to see the error to be reported even though it exists.
This is because the error shows up in the second iteration of the loop whereas SGXRay only examines the first iteration of the loop, after which it assumes the program exits. To find this error, we need SMACK option `--unroll=2` which can be enabled using SGXRay's option `--smack-options='--unroll=2'`.

```shell
sgx-ray enclave.so.bc --intel --ecall-name ecall_error2 --smack-options="--unroll=2"
```

#### NULL-Pointer Check

There are two SMACK options related to null checks -- `--enable-null-check` and `--enable-failing-malloc`. The former enables null checks inside the enclave and the the latter specifies that malloc can fail and return a null pointer. SGXRay does not enable null checks by default because null pointer dereferences are considered less destructive as opposed to other vulnerabilities such as writing to arbitary locations inside an enclave.
