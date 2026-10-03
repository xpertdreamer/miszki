FROM docker.io/library/archlinux:base

ENV GOPATH=/root/go
ENV PATH="${PATH}:${GOPATH}/bin"

RUN pacman -Sy --noconfirm && \
    pacman -S --noconfirm \
    gcc \
    make \
    go \
    git \
    && go install go.abhg.dev/doc2go@latest \
    && pacman -Scc --noconfirm

WORKDIR /app

CMD ["/bin/bash"]

ENTRYPOINT ["./build.sh"]
