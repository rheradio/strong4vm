# Build stage: compiles all dependencies and the strong4vm binary
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    cmake \
    make \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .
# Full clean so nothing pre-compiled on the host (with a potentially different
# glibc/toolchain) is reused; Docker builds everything with this container's GCC.
RUN make clean-all && make

# Runtime stage: minimal image containing only the compiled binaries
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    zlib1g \
    && rm -rf /var/lib/apt/lists/*

# Preserve the relative layout so the backbone_solver binary is found
# at <exe_dir>/../backbone_solver/bin/backbone_solver if ever needed
COPY --from=builder /src/bin/strong4vm                                          /app/bin/strong4vm
COPY --from=builder /src/uvl2dimacs/backbone_solver/bin/backbone_solver /app/backbone_solver/bin/backbone_solver

ENV PATH="/app/bin:$PATH"

# /data is the working directory; mount your model files here
WORKDIR /data

ENTRYPOINT ["strong4vm"]
