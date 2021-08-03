# A Tutorial on SGXRay

In this tutorial, we will use an enclave application [copytohost](../examples/copytohost) as the target.
We also provide a Docker image such that this tutorial can be fully reproducible.


## Preparation

Pleae pull the following image.

```bash
docker pull baiduxlab/sgx-ray-distro:tutorial
```

Once it's pulled, let's start a Docker container based on it.

```bash
docker run -it --rm baiduxlab/sgx-ray-distro:tutorial
```

## Generate the BC File

The first step of applying SGXRay is to generate the bc file for the trusted part of an enclave application.
This step can be done via the following commands,

```bash
cd examples/copytohost
make
extract-bc -l llvm-link-11 enclave.so
```

Now we should be able to get the BC file (`enclave.so.bc`).

## Verification

Since we have the bc file (i.e., the input to SGX-Ray CLI). We can proceed to perform the verification via SGX-Ray CLI.

```
sgx-ray --intel --ecall-name ecall_error1 --verifier corral enclave.so.bc
```

Then we should be able to see an output like,
```
SMACK program verifier version 2.7.0
SMACK found an error: invalid pointer dereference at Enclave/Enclave.cpp, line 54, column 29.
```
